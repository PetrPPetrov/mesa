/**************************************************************************
 *
 * Copyright 2007 VMware, Inc.
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
 * IN NO EVENT SHALL VMWARE AND/OR ITS SUPPLIERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 **************************************************************************/

#include "main/imports.h"
#include "main/accum.h"
#include "main/api_exec.h"
#include "main/context.h"
#include "main/glthread.h"
#include "main/samplerobj.h"
#include "main/shaderobj.h"
#include "main/version.h"
#include "main/vtxfmt.h"
#include "main/hash.h"
#include "program/prog_cache.h"
#include "vbo/vbo.h"
#include "glapi/glapi.h"
#include "st_manager.h"
#include "st_context.h"
#include "st_debug.h"
#include "st_cb_bitmap.h"
#include "st_cb_blit.h"
#include "st_cb_bufferobjects.h"
#include "st_cb_clear.h"
#include "st_cb_compute.h"
#include "st_cb_condrender.h"
#include "st_cb_copyimage.h"
#include "st_cb_drawpixels.h"
#include "st_cb_rasterpos.h"
#include "st_cb_drawtex.h"
#include "st_cb_eglimage.h"
#include "st_cb_fbo.h"
#include "st_cb_feedback.h"
#include "st_cb_memoryobjects.h"
#include "st_cb_msaa.h"
#include "st_cb_perfmon.h"
#include "st_cb_program.h"
#include "st_cb_queryobj.h"
#include "st_cb_readpixels.h"
#include "st_cb_semaphoreobjects.h"
#include "st_cb_texture.h"
#include "st_cb_xformfb.h"
#include "st_cb_flush.h"
#include "st_cb_syncobj.h"
#include "st_cb_strings.h"
#include "st_cb_texturebarrier.h"
#include "st_cb_viewport.h"
#include "st_atom.h"
#include "st_draw.h"
#include "st_extensions.h"
#include "st_gen_mipmap.h"
#include "st_pbo.h"
#include "st_program.h"
#include "st_sampler_view.h"
#include "st_shader_cache.h"
#include "st_vdpau.h"
#include "st_texture.h"
#include "st_util.h"
#include "pipe/p_context.h"
#include "util/u_cpu_detect.h"
#include "util/u_inlines.h"
#include "util/u_upload_mgr.h"
#include "util/u_vbuf.h"
#include "cso_cache/cso_context.h"
#include "compiler/glsl/glsl_parser_extras.h"


DEBUG_GET_ONCE_BOOL_OPTION(mesa_mvp_dp4, "MESA_MVP_DP4", FALSE)


/**
 * Called via ctx->Driver.Enable()
 */
static void
st_Enable(struct gl_context *ctx, GLenum cap, GLboolean state)
{
   struct st_context *st = st_context(ctx);

   switch (cap) {
   case GL_DEBUG_OUTPUT:
   case GL_DEBUG_OUTPUT_SYNCHRONOUS:
      st_update_debug_callback(st);
      break;
   default:
      break;
   }
}


/**
 * Called via ctx->Driver.QueryMemoryInfo()
 */
static void
st_query_memory_info(struct gl_context *ctx, struct gl_memory_info *out)
{
   struct pipe_screen *screen = st_context(ctx)->pipe->screen;
   struct pipe_memory_info info;

   assert(screen->query_memory_info);
   if (!screen->query_memory_info)
      return;

   screen->query_memory_info(screen, &info);

   out->total_device_memory = info.total_device_memory;
   out->avail_device_memory = info.avail_device_memory;
   out->total_staging_memory = info.total_staging_memory;
   out->avail_staging_memory = info.avail_staging_memory;
   out->device_memory_evicted = info.device_memory_evicted;
   out->nr_device_memory_evictions = info.nr_device_memory_evictions;
}


static uint64_t
st_get_active_states(struct gl_context *ctx)
{
   struct st_vertex_program *vp =
      st_vertex_program(ctx->VertexProgram._Current);
   struct st_common_program *tcp =
      st_common_program(ctx->TessCtrlProgram._Current);
   struct st_common_program *tep =
      st_common_program(ctx->TessEvalProgram._Current);
   struct st_common_program *gp =
      st_common_program(ctx->GeometryProgram._Current);
   struct st_fragment_program *fp =
      st_fragment_program(ctx->FragmentProgram._Current);
   struct st_compute_program *cp =
      st_compute_program(ctx->ComputeProgram._Current);
   uint64_t active_shader_states = 0;

   if (vp)
      active_shader_states |= vp->affected_states;
   if (tcp)
      active_shader_states |= tcp->affected_states;
   if (tep)
      active_shader_states |= tep->affected_states;
   if (gp)
      active_shader_states |= gp->affected_states;
   if (fp)
      active_shader_states |= fp->affected_states;
   if (cp)
      active_shader_states |= cp->affected_states;

   /* Mark non-shader-resource shader states as "always active". */
   return active_shader_states | ~ST_ALL_SHADER_RESOURCES;
}


void
st_invalidate_buffers(struct st_context *st)
{
   st->dirty |= ST_NEW_BLEND |
                ST_NEW_DSA |
                ST_NEW_FB_STATE |
                ST_NEW_SAMPLE_STATE |
                ST_NEW_SAMPLE_SHADING |
                ST_NEW_FS_STATE |
                ST_NEW_POLY_STIPPLE |
                ST_NEW_VIEWPORT |
                ST_NEW_RASTERIZER |
                ST_NEW_SCISSOR |
                ST_NEW_WINDOW_RECTANGLES;
}


static inline bool
st_vp_uses_current_values(const struct gl_context *ctx)
{
   const uint64_t inputs = ctx->VertexProgram._Current->info.inputs_read;
   return _mesa_draw_current_bits(ctx) & inputs;
}


/**
 * Called via ctx->Driver.UpdateState()
 */
static void
st_invalidate_state(struct gl_context *ctx)
{
   GLbitfield new_state = ctx->NewState;
   struct st_context *st = st_context(ctx);

   if (new_state & _NEW_BUFFERS) {
      st_invalidate_buffers(st);
   } else {
      /* These set a subset of flags set by _NEW_BUFFERS, so we only have to
       * check them when _NEW_BUFFERS isn't set.
       */
      if (new_state & _NEW_PROGRAM)
         st->dirty |= ST_NEW_RASTERIZER;

      if (new_state & _NEW_FOG)
         st->dirty |= ST_NEW_FS_STATE;

      if (new_state & _NEW_FRAG_CLAMP) {
         if (st->clamp_frag_color_in_shader)
            st->dirty |= ST_NEW_FS_STATE;
         else
            st->dirty |= ST_NEW_RASTERIZER;
      }
   }

   if (new_state & (_NEW_LIGHT |
                    _NEW_POINT))
      st->dirty |= ST_NEW_RASTERIZER;

   if (new_state & _NEW_PROJECTION &&
       st_user_clip_planes_enabled(ctx))
      st->dirty |= ST_NEW_CLIP_STATE;

   if (new_state & _NEW_PIXEL)
      st->dirty |= ST_NEW_PIXEL_TRANSFER;

   if (new_state & _NEW_CURRENT_ATTRIB && st_vp_uses_current_values(ctx))
      st->dirty |= ST_NEW_VERTEX_ARRAYS;

   /* Update the vertex shader if ctx->Light._ClampVertexColor was changed. */
   if (st->clamp_vert_color_in_shader && (new_state & _NEW_LIGHT)) {
      st->dirty |= ST_NEW_VS_STATE;
      if (st->ctx->API == API_OPENGL_COMPAT && ctx->Version >= 32) {
         st->dirty |= ST_NEW_GS_STATE | ST_NEW_TES_STATE;
      }
   }

   /* Which shaders are dirty will be determined manually. */
   if (new_state & _NEW_PROGRAM) {
      st->gfx_shaders_may_be_dirty = true;
      st->compute_shader_may_be_dirty = true;
      /* This will mask out unused shader resources. */
      st->active_states = st_get_active_states(ctx);
   }

   if (new_state & _NEW_TEXTURE_OBJECT) {
      st->dirty |= st->active_states &
                   (ST_NEW_SAMPLER_VIEWS |
                    ST_NEW_SAMPLERS |
                    ST_NEW_IMAGE_UNITS);
      if (ctx->FragmentProgram._Current &&
          ctx->FragmentProgram._Current->ExternalSamplersUsed) {
         st->dirty |= ST_NEW_FS_STATE;
      }
   }
}


/*
 * In some circumstances (such as running google-chrome) the state
 * tracker may try to delete a resource view from a context different
 * than when it was created.  We don't want to do that.
 *
 * In that situation, st_texture_release_all_sampler_views() calls this
 * function to transfer the sampler view reference to this context (expected
 * to be the context which created the view.)
 */
void
st_save_zombie_sampler_view(struct st_context *st,
                            struct pipe_sampler_view *view)
{
   struct st_zombie_sampler_view_node *entry;

   assert(view->context == st->pipe);

   entry = MALLOC_STRUCT(st_zombie_sampler_view_node);
   if (!entry)
      return;

   entry->view = view;

   /* We need a mutex since this function may be called from one thread
    * while free_zombie_resource_views() is called from another.
    */
   mtx_lock(&st->zombie_sampler_views.mutex);
   LIST_ADDTAIL(&entry->node, &st->zombie_sampler_views.list.node);
   mtx_unlock(&st->zombie_sampler_views.mutex);
}


/*
 * Since OpenGL shaders may be shared among contexts, we can wind up
 * with variants of a shader created with different contexts.
 * When we go to destroy a gallium shader, we want to free it with the
 * same context that it was created with, unless the driver reports
 * PIPE_CAP_SHAREABLE_SHADERS = TRUE.
 */
void
st_save_zombie_shader(struct st_context *st,
                      enum pipe_shader_type type,
                      struct pipe_shader_state *shader)
{
   struct st_zombie_shader_node *entry;

   /* we shouldn't be here if the driver supports shareable shaders */
   assert(!st->has_shareable_shaders);

   entry = MALLOC_STRUCT(st_zombie_shader_node);
   if (!entry)
      return;

   entry->shader = shader;
   entry->type = type;

   /* We need a mutex since this function may be called from one thread
    * while free_zombie_shaders() is called from another.
    */
   mtx_lock(&st->zombie_shaders.mutex);
   LIST_ADDTAIL(&entry->node, &st->zombie_shaders.list.node);
   mtx_unlock(&st->zombie_shaders.mutex);
}


/*
 * Free any zombie sampler views that may be attached to this context.
 */
static void
free_zombie_sampler_views(struct st_context *st)
{
   struct st_zombie_sampler_view_node *entry, *next;

   if (LIST_IS_EMPTY(&st->zombie_sampler_views.list.node)) {
      return;
   }

   mtx_lock(&st->zombie_sampler_views.mutex);

   LIST_FOR_EACH_ENTRY_SAFE(entry, next,
                            &st->zombie_sampler_views.list.node, node) {
      LIST_DEL(&entry->node);  // remove this entry from the list

      assert(entry->view->context == st->pipe);
      pipe_sampler_view_reference(&entry->view, NULL);

      free(entry);
   }

   assert(LIST_IS_EMPTY(&st->zombie_sampler_views.list.node));

   mtx_unlock(&st->zombie_sampler_views.mutex);
}


/*
 * Free any zombie shaders that may be attached to this context.
 */
static void
free_zombie_shaders(struct st_context *st)
{
   struct st_zombie_shader_node *entry, *next;

   if (LIST_IS_EMPTY(&st->zombie_shaders.list.node)) {
      return;
   }

   mtx_lock(&st->zombie_shaders.mutex);

   LIST_FOR_EACH_ENTRY_SAFE(entry, next,
                            &st->zombie_shaders.list.node, node) {
      LIST_DEL(&entry->node);  // remove this entry from the list

      switch (entry->type) {
      case PIPE_SHADER_VERTEX:
         cso_delete_vertex_shader(st->cso_context, entry->shader);
         break;
      case PIPE_SHADER_FRAGMENT:
         cso_delete_fragment_shader(st->cso_context, entry->shader);
         break;
      case PIPE_SHADER_GEOMETRY:
         cso_delete_geometry_shader(st->cso_context, entry->shader);
         break;
      case PIPE_SHADER_TESS_CTRL:
         cso_delete_tessctrl_shader(st->cso_context, entry->shader);
         break;
      case PIPE_SHADER_TESS_EVAL:
         cso_delete_tesseval_shader(st->cso_context, entry->shader);
         break;
      case PIPE_SHADER_COMPUTE:
         cso_delete_compute_shader(st->cso_context, entry->shader);
         break;
      default:
         unreachable("invalid shader type in free_zombie_shaders()");
      }
      free(entry);
   }

   assert(LIST_IS_EMPTY(&st->zombie_shaders.list.node));

   mtx_unlock(&st->zombie_shaders.mutex);
}


/*
 * This function is called periodically to free any zombie objects
 * which are attached to this context.
 */
void
st_context_free_zombie_objects(struct st_context *st)
{
   free_zombie_sampler_views(st);
   free_zombie_shaders(st);
}


static void
st_destroy_context_priv(struct st_context *st, bool destroy_pipe)
{
   uint i;

   st_destroy_atoms(st);
   st_destroy_draw(st);
   st_destroy_clear(st);
   st_destroy_bitmap(st);
   st_destroy_drawpix(st);
   st_destroy_drawtex(st);
   st_destroy_perfmon(st);
   st_destroy_pbo_helpers(st);
   st_destroy_bound_texture_handles(st);
   st_destroy_bound_image_handles(st);

   for (i = 0; i < ARRAY_SIZE(st->state.frag_sampler_views); i++) {
      pipe_sampler_view_reference(&st->state.frag_sampler_views[i], NULL);
   }

   /* free glReadPixels cache data */
   st_invalidate_readpix_cache(st);
   util_throttle_deinit(st->pipe->screen, &st->throttle);

   cso_destroy_context(st->cso_context);

   if (st->pipe && destroy_pipe)
      st->pipe->destroy(st->pipe);

   free(st);
}


static void
st_init_driver_flags(struct st_context *st)
{
   struct gl_driver_flags *f = &st->ctx->DriverFlags;

   f->NewArray = ST_NEW_VERTEX_ARRAYS;
   f->NewRasterizerDiscard = ST_NEW_RASTERIZER;
   f->NewTileRasterOrder = ST_NEW_RASTERIZER;
   f->NewUniformBuffer = ST_NEW_UNIFORM_BUFFER;
   f->NewDefaultTessLevels = ST_NEW_TESS_STATE;

   /* Shader resources */
   f->NewTextureBuffer = ST_NEW_SAMPLER_VIEWS;
   if (st->has_hw_atomics)
      f->NewAtomicBuffer = ST_NEW_HW_ATOMICS | ST_NEW_CS_ATOMICS;
   else
      f->NewAtomicBuffer = ST_NEW_ATOMIC_BUFFER;
   f->NewShaderStorageBuffer = ST_NEW_STORAGE_BUFFER;
   f->NewImageUnits = ST_NEW_IMAGE_UNITS;

   f->NewShaderConstants[MESA_SHADER_VERTEX] = ST_NEW_VS_CONSTANTS;
   f->NewShaderConstants[MESA_SHADER_TESS_CTRL] = ST_NEW_TCS_CONSTANTS;
   f->NewShaderConstants[MESA_SHADER_TESS_EVAL] = ST_NEW_TES_CONSTANTS;
   f->NewShaderConstants[MESA_SHADER_GEOMETRY] = ST_NEW_GS_CONSTANTS;
   f->NewShaderConstants[MESA_SHADER_FRAGMENT] = ST_NEW_FS_CONSTANTS;
   f->NewShaderConstants[MESA_SHADER_COMPUTE] = ST_NEW_CS_CONSTANTS;

   f->NewWindowRectangles = ST_NEW_WINDOW_RECTANGLES;
   f->NewFramebufferSRGB = ST_NEW_FB_STATE;
   f->NewScissorRect = ST_NEW_SCISSOR;
   f->NewScissorTest = ST_NEW_SCISSOR | ST_NEW_RASTERIZER;
   f->NewAlphaTest = ST_NEW_DSA;
   f->NewBlend = ST_NEW_BLEND;
   f->NewBlendColor = ST_NEW_BLEND_COLOR;
   f->NewColorMask = ST_NEW_BLEND;
   f->NewDepth = ST_NEW_DSA;
   f->NewLogicOp = ST_NEW_BLEND;
   f->NewStencil = ST_NEW_DSA;
   f->NewMultisampleEnable = ST_NEW_BLEND | ST_NEW_RASTERIZER |
                             ST_NEW_SAMPLE_STATE | ST_NEW_SAMPLE_SHADING;
   f->NewSampleAlphaToXEnable = ST_NEW_BLEND;
   f->NewSampleMask = ST_NEW_SAMPLE_STATE;
   f->NewSampleLocations = ST_NEW_SAMPLE_STATE;
   f->NewSampleShading = ST_NEW_SAMPLE_SHADING;

   /* This depends on what the gallium driver wants. */
   if (st->force_persample_in_shader) {
      f->NewMultisampleEnable |= ST_NEW_FS_STATE;
      f->NewSampleShading |= ST_NEW_FS_STATE;
   } else {
      f->NewSampleShading |= ST_NEW_RASTERIZER;
   }

   f->NewClipControl = ST_NEW_VIEWPORT | ST_NEW_RASTERIZER;
   f->NewClipPlane = ST_NEW_CLIP_STATE;
   f->NewClipPlaneEnable = ST_NEW_RASTERIZER;
   f->NewDepthClamp = ST_NEW_RASTERIZER;
   f->NewLineState = ST_NEW_RASTERIZER;
   f->NewPolygonState = ST_NEW_RASTERIZER;
   f->NewPolygonStipple = ST_NEW_POLY_STIPPLE;
   f->NewViewport = ST_NEW_VIEWPORT;
   f->NewNvConservativeRasterization = ST_NEW_RASTERIZER;
   f->NewNvConservativeRasterizationParams = ST_NEW_RASTERIZER;
   f->NewIntelConservativeRasterization = ST_NEW_RASTERIZER;
}


static struct st_context *
st_create_context_priv(struct gl_context *ctx, struct pipe_context *pipe,
                       const struct st_config_options *options, bool no_error)
{
   struct pipe_screen *screen = pipe->screen;
   uint i;
   struct st_context *st = ST_CALLOC_STRUCT( st_context);

   st->options = *options;

   ctx->st = st;

   st->ctx = ctx;
   st->pipe = pipe;

   /* state tracker needs the VBO module */
   _vbo_CreateContext(ctx);

   st->dirty = ST_ALL_STATES_MASK;

   st->can_bind_const_buffer_as_vertex =
      screen->get_param(screen, PIPE_CAP_CAN_BIND_CONST_BUFFER_AS_VERTEX);

   /* st/mesa always uploads zero-stride vertex attribs, and other user
    * vertex buffers are only possible with a compatibility profile.
    * So tell the u_vbuf module that user VBOs are not possible with the Core
    * profile, so that u_vbuf is bypassed completely if there is nothing else
    * to do.
    */
   unsigned vbuf_flags =
      ctx->API == API_OPENGL_CORE ? U_VBUF_FLAG_NO_USER_VBOS : 0;
   st->cso_context = cso_create_context(pipe, vbuf_flags);

   st_init_atoms(st);
   st_init_clear(st);
   st_init_pbo_helpers(st);

   /* Choose texture target for glDrawPixels, glBitmap, renderbuffers */
   if (pipe->screen->get_param(pipe->screen, PIPE_CAP_NPOT_TEXTURES))
      st->internal_target = PIPE_TEXTURE_2D;
   else
      st->internal_target = PIPE_TEXTURE_RECT;

   /* Setup vertex element info for 'struct st_util_vertex'.
    */
   {
      STATIC_ASSERT(sizeof(struct st_util_vertex) == 9 * sizeof(float));

      memset(&st->util_velems, 0, sizeof(st->util_velems));
      st->util_velems[0].src_offset = 0;
      st->util_velems[0].vertex_buffer_index = 0;
      st->util_velems[0].src_format = PIPE_FORMAT_R32G32B32_FLOAT;
      st->util_velems[1].src_offset = 3 * sizeof(float);
      st->util_velems[1].vertex_buffer_index = 0;
      st->util_velems[1].src_format = PIPE_FORMAT_R32G32B32A32_FLOAT;
      st->util_velems[2].src_offset = 7 * sizeof(float);
      st->util_velems[2].vertex_buffer_index = 0;
      st->util_velems[2].src_format = PIPE_FORMAT_R32G32_FLOAT;
   }

   /* we want all vertex data to be placed in buffer objects */
   vbo_use_buffer_objects(ctx);


   /* make sure that no VBOs are left mapped when we're drawing. */
   vbo_always_unmap_buffers(ctx);

   /* Need these flags:
    */
   ctx->FragmentProgram._MaintainTexEnvProgram = GL_TRUE;

   ctx->VertexProgram._MaintainTnlProgram = GL_TRUE;

   if (no_error)
      ctx->Const.ContextFlags |= GL_CONTEXT_FLAG_NO_ERROR_BIT_KHR;

   ctx->Const.PackedDriverUniformStorage =
      screen->get_param(screen, PIPE_CAP_PACKED_UNIFORMS);

   st->has_stencil_export =
      screen->get_param(screen, PIPE_CAP_SHADER_STENCIL_EXPORT);
   st->has_etc1 = screen->is_format_supported(screen, PIPE_FORMAT_ETC1_RGB8,
                                              PIPE_TEXTURE_2D, 0, 0,
                                              PIPE_BIND_SAMPLER_VIEW);
   st->has_etc2 = screen->is_format_supported(screen, PIPE_FORMAT_ETC2_RGB8,
                                              PIPE_TEXTURE_2D, 0, 0,
                                              PIPE_BIND_SAMPLER_VIEW);
   st->has_astc_2d_ldr =
      screen->is_format_supported(screen, PIPE_FORMAT_ASTC_4x4_SRGB,
                                  PIPE_TEXTURE_2D, 0, 0, PIPE_BIND_SAMPLER_VIEW);
   st->prefer_blit_based_texture_transfer = screen->get_param(screen,
                              PIPE_CAP_PREFER_BLIT_BASED_TEXTURE_TRANSFER);
   st->force_persample_in_shader =
      screen->get_param(screen, PIPE_CAP_SAMPLE_SHADING) &&
      !screen->get_param(screen, PIPE_CAP_FORCE_PERSAMPLE_INTERP);
   st->has_shareable_shaders = screen->get_param(screen,
                                                 PIPE_CAP_SHAREABLE_SHADERS);
   st->needs_texcoord_semantic =
      screen->get_param(screen, PIPE_CAP_TGSI_TEXCOORD);
   st->apply_texture_swizzle_to_border_color =
      !!(screen->get_param(screen, PIPE_CAP_TEXTURE_BORDER_COLOR_QUIRK) &
         (PIPE_QUIRK_TEXTURE_BORDER_COLOR_SWIZZLE_NV50 |
          PIPE_QUIRK_TEXTURE_BORDER_COLOR_SWIZZLE_R600));
   st->has_time_elapsed =
      screen->get_param(screen, PIPE_CAP_QUERY_TIME_ELAPSED);
   st->has_half_float_packing =
      screen->get_param(screen, PIPE_CAP_TGSI_PACK_HALF_FLOAT);
   st->has_multi_draw_indirect =
      screen->get_param(screen, PIPE_CAP_MULTI_DRAW_INDIRECT);
   st->has_single_pipe_stat =
      screen->get_param(screen, PIPE_CAP_QUERY_PIPELINE_STATISTICS_SINGLE);
   st->has_indep_blend_func =
      screen->get_param(screen, PIPE_CAP_INDEP_BLEND_FUNC);
   st->needs_rgb_dst_alpha_override =
      screen->get_param(screen, PIPE_CAP_RGB_OVERRIDE_DST_ALPHA_BLEND);
   st->has_signed_vertex_buffer_offset =
      screen->get_param(screen, PIPE_CAP_SIGNED_VERTEX_BUFFER_OFFSET);

   st->has_hw_atomics =
      screen->get_shader_param(screen, PIPE_SHADER_FRAGMENT,
                               PIPE_SHADER_CAP_MAX_HW_ATOMIC_COUNTERS)
      ? true : false;

   util_throttle_init(&st->throttle,
                      screen->get_param(screen,
                                        PIPE_CAP_MAX_TEXTURE_UPLOAD_MEMORY_BUDGET));

   /* GL limits and extensions */
   st_init_limits(pipe->screen, &ctx->Const, &ctx->Extensions);
   st_init_extensions(pipe->screen, &ctx->Const,
                      &ctx->Extensions, &st->options, ctx->API);

   if (st_have_perfmon(st)) {
      ctx->Extensions.AMD_performance_monitor = GL_TRUE;
   }

   /* Enable shader-based fallbacks for ARB_color_buffer_float if needed. */
   if (screen->get_param(screen, PIPE_CAP_VERTEX_COLOR_UNCLAMPED)) {
      if (!screen->get_param(screen, PIPE_CAP_VERTEX_COLOR_CLAMPED)) {
         st->clamp_vert_color_in_shader = GL_TRUE;
      }

      if (!screen->get_param(screen, PIPE_CAP_FRAGMENT_COLOR_CLAMPED)) {
         st->clamp_frag_color_in_shader = GL_TRUE;
      }

      /* For drivers which cannot do color clamping, it's better to just
       * disable ARB_color_buffer_float in the core profile, because
       * the clamping is deprecated there anyway. */
      if (ctx->API == API_OPENGL_CORE &&
          (st->clamp_frag_color_in_shader || st->clamp_vert_color_in_shader)) {
         st->clamp_vert_color_in_shader = GL_FALSE;
         st->clamp_frag_color_in_shader = GL_FALSE;
         ctx->Extensions.ARB_color_buffer_float = GL_FALSE;
      }
   }

   /* called after _mesa_create_context/_mesa_init_point, fix default user
    * settable max point size up
    */
   ctx->Point.MaxSize = MAX2(ctx->Const.MaxPointSize,
                             ctx->Const.MaxPointSizeAA);
   /* For vertex shaders, make sure not to emit saturate when SM 3.0
    * is not supported
    */
   ctx->Const.ShaderCompilerOptions[MESA_SHADER_VERTEX].EmitNoSat =
      !screen->get_param(screen, PIPE_CAP_VERTEX_SHADER_SATURATE);

   if (ctx->Const.GLSLVersion < 400) {
      for (i = 0; i < MESA_SHADER_STAGES; i++)
         ctx->Const.ShaderCompilerOptions[i].EmitNoIndirectSampler = true;
   }

   /* Set which shader types can be compiled at link time. */
   st->shader_has_one_variant[MESA_SHADER_VERTEX] =
         st->has_shareable_shaders &&
         !st->clamp_vert_color_in_shader;

   st->shader_has_one_variant[MESA_SHADER_FRAGMENT] =
         st->has_shareable_shaders &&
         !st->clamp_frag_color_in_shader &&
         !st->force_persample_in_shader;

   st->shader_has_one_variant[MESA_SHADER_TESS_CTRL] = st->has_shareable_shaders;
   st->shader_has_one_variant[MESA_SHADER_TESS_EVAL] =
         st->has_shareable_shaders &&
         !st->clamp_vert_color_in_shader;
   st->shader_has_one_variant[MESA_SHADER_GEOMETRY] =
         st->has_shareable_shaders &&
         !st->clamp_vert_color_in_shader;
   st->shader_has_one_variant[MESA_SHADER_COMPUTE] = st->has_shareable_shaders;

   st->bitmap.cache.empty = true;

   _mesa_override_extensions(ctx);
   _mesa_compute_version(ctx);

   if (ctx->Version == 0) {
      /* This can happen when a core profile was requested, but the driver
       * does not support some features of GL 3.1 or later.
       */
      st_destroy_context_priv(st, false);
      return NULL;
   }

   _mesa_initialize_dispatch_tables(ctx);
   _mesa_initialize_vbo_vtxfmt(ctx);
   st_init_driver_flags(st);

   /* Initialize context's winsys buffers list */
   LIST_INITHEAD(&st->winsys_buffers);

   LIST_INITHEAD(&st->zombie_sampler_views.list.node);
   mtx_init(&st->zombie_sampler_views.mutex, mtx_plain);
   LIST_INITHEAD(&st->zombie_shaders.list.node);
   mtx_init(&st->zombie_shaders.mutex, mtx_plain);

   return st;
}


static void
st_emit_string_marker(struct gl_context *ctx, const GLchar *string, GLsizei len)
{
   struct st_context *st = ctx->st;
   st->pipe->emit_string_marker(st->pipe, string, len);
}


static void
st_set_background_context(struct gl_context *ctx,
                          struct util_queue_monitoring *queue_info)
{
   struct st_context *st = ctx->st;
   struct st_manager *smapi =
      (struct st_manager *) st->iface.st_context_private;

   assert(smapi->set_background_context);
   smapi->set_background_context(&st->iface, queue_info);
}


static void
st_get_device_uuid(struct gl_context *ctx, char *uuid)
{
   struct pipe_screen *screen = st_context(ctx)->pipe->screen;

   assert(GL_UUID_SIZE_EXT >= PIPE_UUID_SIZE);
   memset(uuid, 0, GL_UUID_SIZE_EXT);
   screen->get_device_uuid(screen, uuid);
}


static void
st_get_driver_uuid(struct gl_context *ctx, char *uuid)
{
   struct pipe_screen *screen = st_context(ctx)->pipe->screen;

   assert(GL_UUID_SIZE_EXT >= PIPE_UUID_SIZE);
   memset(uuid, 0, GL_UUID_SIZE_EXT);
   screen->get_driver_uuid(screen, uuid);
}


static void
st_init_driver_functions(struct pipe_screen *screen,
                         struct dd_function_table *functions)
{
   _mesa_init_sampler_object_functions(functions);

   st_init_draw_functions(functions);
   st_init_blit_functions(functions);
   st_init_bufferobject_functions(screen, functions);
   st_init_clear_functions(functions);
   st_init_bitmap_functions(functions);
   st_init_copy_image_functions(functions);
   st_init_drawpixels_functions(functions);
   st_init_rasterpos_functions(functions);

   st_init_drawtex_functions(functions);

   st_init_eglimage_functions(functions);

   st_init_fbo_functions(functions);
   st_init_feedback_functions(functions);
   st_init_memoryobject_functions(functions);
   st_init_msaa_functions(functions);
   st_init_perfmon_functions(functions);
   st_init_program_functions(functions);
   st_init_query_functions(functions);
   st_init_cond_render_functions(functions);
   st_init_readpixels_functions(functions);
   st_init_semaphoreobject_functions(functions);
   st_init_texture_functions(functions);
   st_init_texture_barrier_functions(functions);
   st_init_flush_functions(screen, functions);
   st_init_string_functions(functions);
   st_init_viewport_functions(functions);
   st_init_compute_functions(functions);

   st_init_xformfb_functions(functions);
   st_init_syncobj_functions(functions);

   st_init_vdpau_functions(functions);

   if (screen->get_param(screen, PIPE_CAP_STRING_MARKER))
      functions->EmitStringMarker = st_emit_string_marker;

   functions->Enable = st_Enable;
   functions->UpdateState = st_invalidate_state;
   functions->QueryMemoryInfo = st_query_memory_info;
   functions->SetBackgroundContext = st_set_background_context;
   functions->GetDriverUuid = st_get_driver_uuid;
   functions->GetDeviceUuid = st_get_device_uuid;

   /* GL_ARB_get_program_binary */
   functions->GetProgramBinaryDriverSHA1 = st_get_program_binary_driver_sha1;

   enum pipe_shader_ir preferred_ir = (enum pipe_shader_ir)
      screen->get_shader_param(screen, PIPE_SHADER_VERTEX,
                               PIPE_SHADER_CAP_PREFERRED_IR);
   if (preferred_ir == PIPE_SHADER_IR_NIR) {
      functions->ShaderCacheSerializeDriverBlob =  st_serialise_nir_program;
      functions->ProgramBinarySerializeDriverBlob =
         st_serialise_nir_program_binary;
      functions->ProgramBinaryDeserializeDriverBlob =
         st_deserialise_nir_program;
   } else {
      functions->ShaderCacheSerializeDriverBlob =  st_serialise_tgsi_program;
      functions->ProgramBinarySerializeDriverBlob =
         st_serialise_tgsi_program_binary;
      functions->ProgramBinaryDeserializeDriverBlob =
         st_deserialise_tgsi_program;
   }
}


struct st_context *
st_create_context(gl_api api, struct pipe_context *pipe,
                  const struct gl_config *visual,
                  struct st_context *share,
                  const struct st_config_options *options,
                  bool no_error)
{
   struct gl_context *ctx;
   struct gl_context *shareCtx = share ? share->ctx : NULL;
   struct dd_function_table funcs;
   struct st_context *st;

   util_cpu_detect();

   memset(&funcs, 0, sizeof(funcs));
   st_init_driver_functions(pipe->screen, &funcs);

   ctx = calloc(1, sizeof(struct gl_context));
   if (!ctx)
      return NULL;

   if (!_mesa_initialize_context(ctx, api, visual, shareCtx, &funcs)) {
      free(ctx);
      return NULL;
   }

   st_debug_init();

   if (pipe->screen->get_disk_shader_cache &&
       !(ST_DEBUG & DEBUG_TGSI))
      ctx->Cache = pipe->screen->get_disk_shader_cache(pipe->screen);

   /* XXX: need a capability bit in gallium to query if the pipe
    * driver prefers DP4 or MUL/MAD for vertex transformation.
    */
   if (debug_get_option_mesa_mvp_dp4())
      ctx->Const.ShaderCompilerOptions[MESA_SHADER_VERTEX].OptimizeForAOS = GL_TRUE;

   st = st_create_context_priv(ctx, pipe, options, no_error);
   if (!st) {
      _mesa_destroy_context(ctx);
   }

   return st;
}


/**
 * When we destroy a context, we must examine all texture objects to
 * find/release any sampler views created by that context.
 *
 * This callback is called per-texture object.  It releases all the
 * texture's sampler views which belong to the context.
 */
static void
destroy_tex_sampler_cb(GLuint id, void *data, void *userData)
{
   struct gl_texture_object *texObj = (struct gl_texture_object *) data;
   struct st_context *st = (struct st_context *) userData;

   st_texture_release_context_sampler_view(st, st_texture_object(texObj));
}

static void
destroy_framebuffer_attachment_sampler_cb(GLuint id, void *data, void *userData)
{
   struct gl_framebuffer* glfb = (struct gl_framebuffer*) data;
   struct st_context *st = (struct st_context *) userData;

    for (unsigned i = 0; i < BUFFER_COUNT; i++) {
      struct gl_renderbuffer_attachment *att = &glfb->Attachment[i];
      if (att->Texture) {
        st_texture_release_context_sampler_view(st, st_texture_object(att->Texture));
      }
   }
}

void
st_destroy_context(struct st_context *st)
{
   struct gl_context *ctx = st->ctx;
   struct st_framebuffer *stfb, *next;
   struct gl_framebuffer *save_drawbuffer;
   struct gl_framebuffer *save_readbuffer;

   /* Save the current context and draw/read buffers*/
   GET_CURRENT_CONTEXT(save_ctx);
   if (save_ctx) {
      save_drawbuffer = save_ctx->WinSysDrawBuffer;
      save_readbuffer = save_ctx->WinSysReadBuffer;
   } else {
      save_drawbuffer = save_readbuffer = NULL;
   }

   /*
    * We need to bind the context we're deleting so that
    * _mesa_reference_texobj_() uses this context when deleting textures.
    * Similarly for framebuffer objects, etc.
    */
   _mesa_make_current(ctx, NULL, NULL);

   /* This must be called first so that glthread has a chance to finish */
   _mesa_glthread_destroy(ctx);

   _mesa_HashWalk(ctx->Shared->TexObjects, destroy_tex_sampler_cb, st);

   /* For the fallback textures, free any sampler views belonging to this
    * context.
    */
   for (unsigned i = 0; i < NUM_TEXTURE_TARGETS; i++) {
      struct st_texture_object *stObj =
         st_texture_object(ctx->Shared->FallbackTex[i]);
      if (stObj) {
         st_texture_release_context_sampler_view(st, stObj);
      }
   }

   st_context_free_zombie_objects(st);

   mtx_destroy(&st->zombie_sampler_views.mutex);
   mtx_destroy(&st->zombie_shaders.mutex);

   st_reference_fragprog(st, &st->fp, NULL);
   st_reference_prog(st, &st->gp, NULL);
   st_reference_vertprog(st, &st->vp, NULL);
   st_reference_prog(st, &st->tcp, NULL);
   st_reference_prog(st, &st->tep, NULL);
   st_reference_compprog(st, &st->cp, NULL);

   /* release framebuffer in the winsys buffers list */
   LIST_FOR_EACH_ENTRY_SAFE_REV(stfb, next, &st->winsys_buffers, head) {
      st_framebuffer_reference(&stfb, NULL);
   }

   _mesa_HashWalk(ctx->Shared->FrameBuffers, destroy_framebuffer_attachment_sampler_cb, st);

   pipe_sampler_view_reference(&st->pixel_xfer.pixelmap_sampler_view, NULL);
   pipe_resource_reference(&st->pixel_xfer.pixelmap_texture, NULL);

   _vbo_DestroyContext(ctx);

   st_destroy_program_variants(st);

   _mesa_free_context_data(ctx, false);

   /* This will free the st_context too, so 'st' must not be accessed
    * afterwards. */
   st_destroy_context_priv(st, true);
   st = NULL;

   /* This must be called after st_destroy_context_priv() to avoid a race
    * condition between any shader compiler threads and context destruction.
    */
   _mesa_destroy_shader_compiler_types();

   free(ctx);

   if (save_ctx == ctx) {
      /* unbind the context we just deleted */
      _mesa_make_current(NULL, NULL, NULL);
   } else {
      /* Restore the current context and draw/read buffers (may be NULL) */
      _mesa_make_current(save_ctx, save_drawbuffer, save_readbuffer);
   }
}
