/*
 * Copyright © 2014-2017 Broadcom
 * Copyright (C) 2012 Rob Clark <robclark@freedesktop.org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <sys/sysinfo.h>

#include "common/v3d_device_info.h"
#include "util/os_misc.h"
#include "pipe/p_defines.h"
#include "pipe/p_screen.h"
#include "pipe/p_state.h"

#include "util/u_debug.h"
#include "util/u_memory.h"
#include "util/u_format.h"
#include "util/u_hash_table.h"
#include "util/u_screen.h"
#include "util/u_transfer_helper.h"
#include "util/ralloc.h"
#include "util/xmlconfig.h"

#include <xf86drm.h>
#include "v3d_screen.h"
#include "v3d_context.h"
#include "v3d_resource.h"
#include "compiler/v3d_compiler.h"
#include "drm-uapi/drm_fourcc.h"

static const char *
v3d_screen_get_name(struct pipe_screen *pscreen)
{
        struct v3d_screen *screen = v3d_screen(pscreen);

        if (!screen->name) {
                screen->name = ralloc_asprintf(screen,
                                               "V3D %d.%d",
                                               screen->devinfo.ver / 10,
                                               screen->devinfo.ver % 10);
        }

        return screen->name;
}

static const char *
v3d_screen_get_vendor(struct pipe_screen *pscreen)
{
        return "Broadcom";
}

static void
v3d_screen_destroy(struct pipe_screen *pscreen)
{
        struct v3d_screen *screen = v3d_screen(pscreen);

        util_hash_table_destroy(screen->bo_handles);
        v3d_bufmgr_destroy(pscreen);
        slab_destroy_parent(&screen->transfer_pool);
        free(screen->ro);

        if (using_v3d_simulator)
                v3d_simulator_destroy(screen);

        v3d_compiler_free(screen->compiler);
        u_transfer_helper_destroy(pscreen->transfer_helper);

        close(screen->fd);
        ralloc_free(pscreen);
}

static bool
v3d_has_feature(struct v3d_screen *screen, enum drm_v3d_param feature)
{
        struct drm_v3d_get_param p = {
                .param = feature,
        };
        int ret = v3d_ioctl(screen->fd, DRM_IOCTL_V3D_GET_PARAM, &p);

        if (ret != 0)
                return false;

        return p.value;
}

static int
v3d_screen_get_param(struct pipe_screen *pscreen, enum pipe_cap param)
{
        struct v3d_screen *screen = v3d_screen(pscreen);

        switch (param) {
                /* Supported features (boolean caps). */
        case PIPE_CAP_VERTEX_COLOR_CLAMPED:
        case PIPE_CAP_VERTEX_COLOR_UNCLAMPED:
        case PIPE_CAP_FRAGMENT_COLOR_CLAMPED:
        case PIPE_CAP_BUFFER_MAP_PERSISTENT_COHERENT:
        case PIPE_CAP_NPOT_TEXTURES:
        case PIPE_CAP_SHAREABLE_SHADERS:
        case PIPE_CAP_BLEND_EQUATION_SEPARATE:
        case PIPE_CAP_TEXTURE_MULTISAMPLE:
        case PIPE_CAP_TEXTURE_SWIZZLE:
        case PIPE_CAP_VERTEX_ELEMENT_INSTANCE_DIVISOR:
        case PIPE_CAP_START_INSTANCE:
        case PIPE_CAP_TGSI_INSTANCEID:
        case PIPE_CAP_FRAGMENT_SHADER_TEXTURE_LOD:
        case PIPE_CAP_FRAGMENT_SHADER_DERIVATIVES:
        case PIPE_CAP_VERTEX_SHADER_SATURATE:
        case PIPE_CAP_TEXTURE_QUERY_LOD:
        case PIPE_CAP_PRIMITIVE_RESTART:
        case PIPE_CAP_OCCLUSION_QUERY:
        case PIPE_CAP_POINT_SPRITE:
        case PIPE_CAP_STREAM_OUTPUT_PAUSE_RESUME:
        case PIPE_CAP_DRAW_INDIRECT:
        case PIPE_CAP_MULTI_DRAW_INDIRECT:
        case PIPE_CAP_QUADS_FOLLOW_PROVOKING_VERTEX_CONVENTION:
        case PIPE_CAP_SIGNED_VERTEX_BUFFER_OFFSET:
        case PIPE_CAP_TGSI_CAN_READ_OUTPUTS:
        case PIPE_CAP_TGSI_PACK_HALF_FLOAT:
        case PIPE_CAP_TEXTURE_HALF_FLOAT_LINEAR:
        case PIPE_CAP_FRAMEBUFFER_NO_ATTACHMENT:
                return 1;

        case PIPE_CAP_PACKED_UNIFORMS:
                /* We can't enable this flag, because it results in load_ubo
                 * intrinsics across a 16b boundary, but v3d's TMU general
                 * memory accesses wrap on 16b boundaries.
                 */
                return 0;

        case PIPE_CAP_PREFER_BLIT_BASED_TEXTURE_TRANSFER:
                /* XXX perf: we don't want to emit these extra blits for
                 * glReadPixels(), since we still have to do an uncached read
                 * from the GPU of the result after waiting for the TFU blit
                 * to happen.  However, disabling this introduces instability
                 * in
                 * dEQP-GLES31.functional.image_load_store.early_fragment_tests.*
                 * and corruption in chromium's rendering.
                 */
                return 1;

        case PIPE_CAP_COMPUTE:
                return screen->has_csd && screen->devinfo.ver >= 41;

        case PIPE_CAP_GENERATE_MIPMAP:
                return v3d_has_feature(screen, DRM_V3D_PARAM_SUPPORTS_TFU);

        case PIPE_CAP_INDEP_BLEND_ENABLE:
                return screen->devinfo.ver >= 40;

        case PIPE_CAP_CONSTANT_BUFFER_OFFSET_ALIGNMENT:
                return 256;

        case PIPE_CAP_MAX_TEXTURE_GATHER_COMPONENTS:
                if (screen->devinfo.ver < 40)
                        return 0;
                return 4;

        case PIPE_CAP_SHADER_BUFFER_OFFSET_ALIGNMENT:
                return 4;

        case PIPE_CAP_GLSL_FEATURE_LEVEL:
                return 330;

	case PIPE_CAP_GLSL_FEATURE_LEVEL_COMPATIBILITY:
		return 140;

        case PIPE_CAP_TGSI_FS_COORD_ORIGIN_UPPER_LEFT:
                return 1;
        case PIPE_CAP_TGSI_FS_COORD_ORIGIN_LOWER_LEFT:
                return 0;
        case PIPE_CAP_TGSI_FS_COORD_PIXEL_CENTER_INTEGER:
                if (screen->devinfo.ver >= 40)
                        return 0;
                else
                        return 1;
        case PIPE_CAP_TGSI_FS_COORD_PIXEL_CENTER_HALF_INTEGER:
                if (screen->devinfo.ver >= 40)
                        return 1;
                else
                        return 0;

        case PIPE_CAP_MIXED_FRAMEBUFFER_SIZES:
        case PIPE_CAP_MIXED_COLORBUFFER_FORMATS:
        case PIPE_CAP_MIXED_COLOR_DEPTH_BITS:
                return 1;

        case PIPE_CAP_MAX_STREAM_OUTPUT_BUFFERS:
                return 4;

        case PIPE_CAP_MAX_VARYINGS:
                return V3D_MAX_FS_INPUTS / 4;

                /* Texturing. */
        case PIPE_CAP_MAX_TEXTURE_2D_SIZE:
                if (screen->devinfo.ver < 40)
                        return 2048;
                else if (screen->nonmsaa_texture_size_limit)
                        return 7680;
                else
                        return 4096;
        case PIPE_CAP_MAX_TEXTURE_CUBE_LEVELS:
        case PIPE_CAP_MAX_TEXTURE_3D_LEVELS:
                if (screen->devinfo.ver < 40)
                        return 12;
                else
                        return V3D_MAX_MIP_LEVELS;
        case PIPE_CAP_MAX_TEXTURE_ARRAY_LAYERS:
                return 2048;

                /* Render targets. */
        case PIPE_CAP_MAX_RENDER_TARGETS:
                return 4;

        case PIPE_CAP_VENDOR_ID:
                return 0x14E4;
        case PIPE_CAP_ACCELERATED:
                return 1;
        case PIPE_CAP_VIDEO_MEMORY: {
                uint64_t system_memory;

                if (!os_get_total_physical_memory(&system_memory))
                        return 0;

                return (int)(system_memory >> 20);
        }
        case PIPE_CAP_UMA:
                return 1;

        default:
                return u_pipe_screen_get_param_defaults(pscreen, param);
        }
}

static float
v3d_screen_get_paramf(struct pipe_screen *pscreen, enum pipe_capf param)
{
        switch (param) {
        case PIPE_CAPF_MAX_LINE_WIDTH:
        case PIPE_CAPF_MAX_LINE_WIDTH_AA:
                return 32;

        case PIPE_CAPF_MAX_POINT_WIDTH:
        case PIPE_CAPF_MAX_POINT_WIDTH_AA:
                return 512.0f;

        case PIPE_CAPF_MAX_TEXTURE_ANISOTROPY:
                return 0.0f;
        case PIPE_CAPF_MAX_TEXTURE_LOD_BIAS:
                return 16.0f;

        case PIPE_CAPF_MIN_CONSERVATIVE_RASTER_DILATE:
        case PIPE_CAPF_MAX_CONSERVATIVE_RASTER_DILATE:
        case PIPE_CAPF_CONSERVATIVE_RASTER_DILATE_GRANULARITY:
                return 0.0f;
        default:
                fprintf(stderr, "unknown paramf %d\n", param);
                return 0;
        }
}

static int
v3d_screen_get_shader_param(struct pipe_screen *pscreen, unsigned shader,
                           enum pipe_shader_cap param)
{
        struct v3d_screen *screen = v3d_screen(pscreen);

        switch (shader) {
        case PIPE_SHADER_VERTEX:
        case PIPE_SHADER_FRAGMENT:
                break;
        case PIPE_SHADER_COMPUTE:
                if (!screen->has_csd)
                        return 0;
                break;
        default:
                return 0;
        }

        /* this is probably not totally correct.. but it's a start: */
        switch (param) {
        case PIPE_SHADER_CAP_MAX_INSTRUCTIONS:
        case PIPE_SHADER_CAP_MAX_ALU_INSTRUCTIONS:
        case PIPE_SHADER_CAP_MAX_TEX_INSTRUCTIONS:
        case PIPE_SHADER_CAP_MAX_TEX_INDIRECTIONS:
                return 16384;

        case PIPE_SHADER_CAP_MAX_CONTROL_FLOW_DEPTH:
                return UINT_MAX;

        case PIPE_SHADER_CAP_MAX_INPUTS:
                if (shader == PIPE_SHADER_FRAGMENT)
                        return V3D_MAX_FS_INPUTS / 4;
                else
                        return V3D_MAX_VS_INPUTS / 4;
        case PIPE_SHADER_CAP_MAX_OUTPUTS:
                if (shader == PIPE_SHADER_FRAGMENT)
                        return 4;
                else
                        return V3D_MAX_FS_INPUTS / 4;
        case PIPE_SHADER_CAP_MAX_TEMPS:
                return 256; /* GL_MAX_PROGRAM_TEMPORARIES_ARB */
        case PIPE_SHADER_CAP_MAX_CONST_BUFFER_SIZE:
                /* Note: Limited by the offset size in
                 * v3d_unit_data_create().
                 */
                return 16 * 1024 * sizeof(float);
        case PIPE_SHADER_CAP_MAX_CONST_BUFFERS:
                return 16;
        case PIPE_SHADER_CAP_TGSI_CONT_SUPPORTED:
                return 0;
        case PIPE_SHADER_CAP_INDIRECT_INPUT_ADDR:
        case PIPE_SHADER_CAP_INDIRECT_OUTPUT_ADDR:
                return 0;
        case PIPE_SHADER_CAP_INDIRECT_TEMP_ADDR:
                return 1;
        case PIPE_SHADER_CAP_INDIRECT_CONST_ADDR:
                return 1;
        case PIPE_SHADER_CAP_SUBROUTINES:
                return 0;
        case PIPE_SHADER_CAP_INTEGERS:
                return 1;
        case PIPE_SHADER_CAP_FP16:
        case PIPE_SHADER_CAP_TGSI_DROUND_SUPPORTED:
        case PIPE_SHADER_CAP_TGSI_DFRACEXP_DLDEXP_SUPPORTED:
        case PIPE_SHADER_CAP_TGSI_LDEXP_SUPPORTED:
        case PIPE_SHADER_CAP_TGSI_FMA_SUPPORTED:
        case PIPE_SHADER_CAP_TGSI_ANY_INOUT_DECL_RANGE:
        case PIPE_SHADER_CAP_TGSI_SQRT_SUPPORTED:
        case PIPE_SHADER_CAP_MAX_HW_ATOMIC_COUNTERS:
        case PIPE_SHADER_CAP_MAX_HW_ATOMIC_COUNTER_BUFFERS:
                return 0;
        case PIPE_SHADER_CAP_SCALAR_ISA:
                return 1;
        case PIPE_SHADER_CAP_MAX_TEXTURE_SAMPLERS:
        case PIPE_SHADER_CAP_MAX_SAMPLER_VIEWS:
                return V3D_MAX_TEXTURE_SAMPLERS;

        case PIPE_SHADER_CAP_MAX_SHADER_BUFFERS:
                if (shader == PIPE_SHADER_VERTEX)
                        return 0;

                return PIPE_MAX_SHADER_BUFFERS;

        case PIPE_SHADER_CAP_MAX_SHADER_IMAGES:
                if (screen->devinfo.ver < 41)
                        return 0;
                else
                        return PIPE_MAX_SHADER_IMAGES;

        case PIPE_SHADER_CAP_PREFERRED_IR:
                return PIPE_SHADER_IR_NIR;
        case PIPE_SHADER_CAP_SUPPORTED_IRS:
                return 1 << PIPE_SHADER_IR_NIR;
        case PIPE_SHADER_CAP_MAX_UNROLL_ITERATIONS_HINT:
                return 32;
        case PIPE_SHADER_CAP_LOWER_IF_THRESHOLD:
        case PIPE_SHADER_CAP_TGSI_SKIP_MERGE_REGISTERS:
                return 0;
        default:
                fprintf(stderr, "unknown shader param %d\n", param);
                return 0;
        }
        return 0;
}

static int
v3d_get_compute_param(struct pipe_screen *pscreen, enum pipe_shader_ir ir_type,
                      enum pipe_compute_cap param, void *ret)
{
        struct v3d_screen *screen = v3d_screen(pscreen);

        if (!screen->has_csd)
                return 0;

#define RET(x) do {                                     \
                if (ret)                                \
                        memcpy(ret, x, sizeof(x));      \
                return sizeof(x);                       \
        } while (0)

        switch (param) {
        case PIPE_COMPUTE_CAP_ADDRESS_BITS:
                RET((uint32_t []) { 32 });
                break;

        case PIPE_COMPUTE_CAP_IR_TARGET:
                sprintf(ret, "v3d");
                return strlen(ret);

        case PIPE_COMPUTE_CAP_GRID_DIMENSION:
                RET((uint64_t []) { 3 });

        case PIPE_COMPUTE_CAP_MAX_GRID_SIZE:
                /* GL_MAX_COMPUTE_SHADER_WORK_GROUP_COUNT: The CSD has a
                 * 16-bit field for the number of workgroups in each
                 * dimension.
                 */
                RET(((uint64_t []) { 65535, 65535, 65535 }));

        case PIPE_COMPUTE_CAP_MAX_BLOCK_SIZE:
                /* GL_MAX_COMPUTE_WORK_GROUP_SIZE */
                RET(((uint64_t []) { 256, 256, 256 }));

        case PIPE_COMPUTE_CAP_MAX_THREADS_PER_BLOCK:
        case PIPE_COMPUTE_CAP_MAX_VARIABLE_THREADS_PER_BLOCK:
                /* GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS: This is
                 * limited by WG_SIZE in the CSD.
                 */
                RET((uint64_t []) { 256 });

        case PIPE_COMPUTE_CAP_MAX_GLOBAL_SIZE:
                RET((uint64_t []) { 1024 * 1024 * 1024 });

        case PIPE_COMPUTE_CAP_MAX_LOCAL_SIZE:
                /* GL_MAX_COMPUTE_SHARED_MEMORY_SIZE */
                RET((uint64_t []) { 32768 });

        case PIPE_COMPUTE_CAP_MAX_PRIVATE_SIZE:
        case PIPE_COMPUTE_CAP_MAX_INPUT_SIZE:
                RET((uint64_t []) { 4096 });

        case PIPE_COMPUTE_CAP_MAX_MEM_ALLOC_SIZE: {
                struct sysinfo si;
                sysinfo(&si);
                RET((uint64_t []) { si.totalram });
        }

        case PIPE_COMPUTE_CAP_MAX_CLOCK_FREQUENCY:
                /* OpenCL only */
                RET((uint32_t []) { 0 });

        case PIPE_COMPUTE_CAP_MAX_COMPUTE_UNITS:
                RET((uint32_t []) { 1 });

        case PIPE_COMPUTE_CAP_IMAGES_SUPPORTED:
                RET((uint32_t []) { 1 });

        case PIPE_COMPUTE_CAP_SUBGROUP_SIZE:
                RET((uint32_t []) { 16 });

        }

        return 0;
}

static boolean
v3d_screen_is_format_supported(struct pipe_screen *pscreen,
                               enum pipe_format format,
                               enum pipe_texture_target target,
                               unsigned sample_count,
                               unsigned storage_sample_count,
                               unsigned usage)
{
        struct v3d_screen *screen = v3d_screen(pscreen);

        if (MAX2(1, sample_count) != MAX2(1, storage_sample_count))
                return false;

        if (sample_count > 1 && sample_count != V3D_MAX_SAMPLES)
                return FALSE;

        if (target >= PIPE_MAX_TEXTURE_TYPES) {
                return FALSE;
        }

        if (usage & PIPE_BIND_VERTEX_BUFFER) {
                switch (format) {
                case PIPE_FORMAT_R32G32B32A32_FLOAT:
                case PIPE_FORMAT_R32G32B32_FLOAT:
                case PIPE_FORMAT_R32G32_FLOAT:
                case PIPE_FORMAT_R32_FLOAT:
                case PIPE_FORMAT_R32G32B32A32_SNORM:
                case PIPE_FORMAT_R32G32B32_SNORM:
                case PIPE_FORMAT_R32G32_SNORM:
                case PIPE_FORMAT_R32_SNORM:
                case PIPE_FORMAT_R32G32B32A32_SSCALED:
                case PIPE_FORMAT_R32G32B32_SSCALED:
                case PIPE_FORMAT_R32G32_SSCALED:
                case PIPE_FORMAT_R32_SSCALED:
                case PIPE_FORMAT_R16G16B16A16_UNORM:
                case PIPE_FORMAT_R16G16B16_UNORM:
                case PIPE_FORMAT_R16G16_UNORM:
                case PIPE_FORMAT_R16_UNORM:
                case PIPE_FORMAT_R16G16B16A16_SNORM:
                case PIPE_FORMAT_R16G16B16_SNORM:
                case PIPE_FORMAT_R16G16_SNORM:
                case PIPE_FORMAT_R16_SNORM:
                case PIPE_FORMAT_R16G16B16A16_USCALED:
                case PIPE_FORMAT_R16G16B16_USCALED:
                case PIPE_FORMAT_R16G16_USCALED:
                case PIPE_FORMAT_R16_USCALED:
                case PIPE_FORMAT_R16G16B16A16_SSCALED:
                case PIPE_FORMAT_R16G16B16_SSCALED:
                case PIPE_FORMAT_R16G16_SSCALED:
                case PIPE_FORMAT_R16_SSCALED:
                case PIPE_FORMAT_R8G8B8A8_UNORM:
                case PIPE_FORMAT_R8G8B8_UNORM:
                case PIPE_FORMAT_R8G8_UNORM:
                case PIPE_FORMAT_R8_UNORM:
                case PIPE_FORMAT_R8G8B8A8_SNORM:
                case PIPE_FORMAT_R8G8B8_SNORM:
                case PIPE_FORMAT_R8G8_SNORM:
                case PIPE_FORMAT_R8_SNORM:
                case PIPE_FORMAT_R8G8B8A8_USCALED:
                case PIPE_FORMAT_R8G8B8_USCALED:
                case PIPE_FORMAT_R8G8_USCALED:
                case PIPE_FORMAT_R8_USCALED:
                case PIPE_FORMAT_R8G8B8A8_SSCALED:
                case PIPE_FORMAT_R8G8B8_SSCALED:
                case PIPE_FORMAT_R8G8_SSCALED:
                case PIPE_FORMAT_R8_SSCALED:
                case PIPE_FORMAT_R10G10B10A2_UNORM:
                case PIPE_FORMAT_B10G10R10A2_UNORM:
                case PIPE_FORMAT_R10G10B10A2_SNORM:
                case PIPE_FORMAT_B10G10R10A2_SNORM:
                case PIPE_FORMAT_R10G10B10A2_USCALED:
                case PIPE_FORMAT_B10G10R10A2_USCALED:
                case PIPE_FORMAT_R10G10B10A2_SSCALED:
                case PIPE_FORMAT_B10G10R10A2_SSCALED:
                        break;
                default:
                        return FALSE;
                }
        }

        /* FORMAT_NONE gets allowed for ARB_framebuffer_no_attachments's probe
         * of FRAMEBUFFER_MAX_SAMPLES
         */
        if ((usage & PIPE_BIND_RENDER_TARGET) &&
            format != PIPE_FORMAT_NONE &&
            !v3d_rt_format_supported(&screen->devinfo, format)) {
                return FALSE;
        }

        if ((usage & PIPE_BIND_SAMPLER_VIEW) &&
            !v3d_tex_format_supported(&screen->devinfo, format)) {
                return FALSE;
        }

        if ((usage & PIPE_BIND_DEPTH_STENCIL) &&
            !(format == PIPE_FORMAT_S8_UINT_Z24_UNORM ||
              format == PIPE_FORMAT_X8Z24_UNORM ||
              format == PIPE_FORMAT_Z16_UNORM ||
              format == PIPE_FORMAT_Z32_FLOAT ||
              format == PIPE_FORMAT_Z32_FLOAT_S8X24_UINT)) {
                return FALSE;
        }

        if ((usage & PIPE_BIND_INDEX_BUFFER) &&
            !(format == PIPE_FORMAT_I8_UINT ||
              format == PIPE_FORMAT_I16_UINT ||
              format == PIPE_FORMAT_I32_UINT)) {
                return FALSE;
        }

        return TRUE;
}

#define PTR_TO_UINT(x) ((unsigned)((intptr_t)(x)))

static unsigned handle_hash(void *key)
{
    return PTR_TO_UINT(key);
}

static int handle_compare(void *key1, void *key2)
{
    return PTR_TO_UINT(key1) != PTR_TO_UINT(key2);
}

static const void *
v3d_screen_get_compiler_options(struct pipe_screen *pscreen,
                                enum pipe_shader_ir ir, unsigned shader)
{
        return &v3d_nir_options;
}

static void
v3d_screen_query_dmabuf_modifiers(struct pipe_screen *pscreen,
                                  enum pipe_format format, int max,
                                  uint64_t *modifiers,
                                  unsigned int *external_only,
                                  int *count)
{
        int i;
        uint64_t available_modifiers[] = {
                DRM_FORMAT_MOD_BROADCOM_UIF,
                DRM_FORMAT_MOD_LINEAR,
        };
        int num_modifiers = ARRAY_SIZE(available_modifiers);

        if (!modifiers) {
                *count = num_modifiers;
                return;
        }

        *count = MIN2(max, num_modifiers);
        for (i = 0; i < *count; i++) {
                modifiers[i] = available_modifiers[i];
                if (external_only)
                        external_only[i] = false;
       }
}

struct pipe_screen *
v3d_screen_create(int fd, const struct pipe_screen_config *config,
                  struct renderonly *ro)
{
        struct v3d_screen *screen = rzalloc(NULL, struct v3d_screen);
        struct pipe_screen *pscreen;

        pscreen = &screen->base;

        pscreen->destroy = v3d_screen_destroy;
        pscreen->get_param = v3d_screen_get_param;
        pscreen->get_paramf = v3d_screen_get_paramf;
        pscreen->get_shader_param = v3d_screen_get_shader_param;
        pscreen->get_compute_param = v3d_get_compute_param;
        pscreen->context_create = v3d_context_create;
        pscreen->is_format_supported = v3d_screen_is_format_supported;

        screen->fd = fd;
        if (ro) {
                screen->ro = renderonly_dup(ro);
                if (!screen->ro) {
                        fprintf(stderr, "Failed to dup renderonly object\n");
                        ralloc_free(screen);
                        return NULL;
                }
        }
        list_inithead(&screen->bo_cache.time_list);
        (void)mtx_init(&screen->bo_handles_mutex, mtx_plain);
        screen->bo_handles = util_hash_table_create(handle_hash, handle_compare);

#if defined(USE_V3D_SIMULATOR)
        v3d_simulator_init(screen);
#endif

        if (!v3d_get_device_info(screen->fd, &screen->devinfo, &v3d_ioctl))
                goto fail;

        /* We have to driCheckOption for the simulator mode to not assertion
         * fail on not having our XML config.
         */
        const char *nonmsaa_name = "v3d_nonmsaa_texture_size_limit";
        screen->nonmsaa_texture_size_limit =
                driCheckOption(config->options, nonmsaa_name, DRI_BOOL) &&
                driQueryOptionb(config->options, nonmsaa_name);

        slab_create_parent(&screen->transfer_pool, sizeof(struct v3d_transfer), 16);

        screen->has_csd = false; /* until the UABI is enabled. */

        v3d_fence_init(screen);

        v3d_process_debug_variable();

        v3d_resource_screen_init(pscreen);

        screen->compiler = v3d_compiler_init(&screen->devinfo);

        pscreen->get_name = v3d_screen_get_name;
        pscreen->get_vendor = v3d_screen_get_vendor;
        pscreen->get_device_vendor = v3d_screen_get_vendor;
        pscreen->get_compiler_options = v3d_screen_get_compiler_options;
        pscreen->query_dmabuf_modifiers = v3d_screen_query_dmabuf_modifiers;

        return pscreen;

fail:
        close(fd);
        ralloc_free(pscreen);
        return NULL;
}
