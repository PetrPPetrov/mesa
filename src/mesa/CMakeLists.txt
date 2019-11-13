# Copyright © 2017 Intel Corporation

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

project(mesa_src_mesa)

cmake_minimum_required(VERSION 3.4)

set(files_libmesa_common
${mesa_src_mesa_SOURCE_DIR}/program/arbprogparse.c
${mesa_src_mesa_SOURCE_DIR}/program/arbprogparse.h
${mesa_src_mesa_SOURCE_DIR}/program/ir_to_mesa.cpp
${mesa_src_mesa_SOURCE_DIR}/program/ir_to_mesa.h
${mesa_src_mesa_SOURCE_DIR}/program/prog_cache.c
${mesa_src_mesa_SOURCE_DIR}/program/prog_cache.h
${mesa_src_mesa_SOURCE_DIR}/program/prog_execute.c
${mesa_src_mesa_SOURCE_DIR}/program/prog_execute.h
${mesa_src_mesa_SOURCE_DIR}/program/prog_instruction.c
${mesa_src_mesa_SOURCE_DIR}/program/prog_instruction.h
${mesa_src_mesa_SOURCE_DIR}/program/prog_noise.c
${mesa_src_mesa_SOURCE_DIR}/program/prog_noise.h
${mesa_src_mesa_SOURCE_DIR}/program/prog_opt_constant_fold.c
${mesa_src_mesa_SOURCE_DIR}/program/prog_optimize.c
${mesa_src_mesa_SOURCE_DIR}/program/prog_optimize.h
${mesa_src_mesa_SOURCE_DIR}/program/prog_parameter.c
${mesa_src_mesa_SOURCE_DIR}/program/prog_parameter.h
${mesa_src_mesa_SOURCE_DIR}/program/prog_parameter_layout.c
${mesa_src_mesa_SOURCE_DIR}/program/prog_parameter_layout.h
${mesa_src_mesa_SOURCE_DIR}/program/prog_print.c
${mesa_src_mesa_SOURCE_DIR}/program/prog_print.h
${mesa_src_mesa_SOURCE_DIR}/program/program.c
${mesa_src_mesa_SOURCE_DIR}/program/program.h
${mesa_src_mesa_SOURCE_DIR}/program/programopt.c
${mesa_src_mesa_SOURCE_DIR}/program/programopt.h
${mesa_src_mesa_SOURCE_DIR}/program/program_parse_extra.c
${mesa_src_mesa_SOURCE_DIR}/program/program_parser.h
${mesa_src_mesa_SOURCE_DIR}/program/prog_statevars.c
${mesa_src_mesa_SOURCE_DIR}/program/prog_statevars.h
${mesa_src_mesa_SOURCE_DIR}/program/symbol_table.c
${mesa_src_mesa_SOURCE_DIR}/program/symbol_table.h
${mesa_src_mesa_SOURCE_DIR}/program/prog_to_nir.c
${mesa_src_mesa_SOURCE_DIR}/program/prog_to_nir.h
${mesa_src_mesa_SOURCE_DIR}/main/accum.c
${mesa_src_mesa_SOURCE_DIR}/main/accum.h
${mesa_src_mesa_SOURCE_DIR}/main/api_arrayelt.c
${mesa_src_mesa_SOURCE_DIR}/main/api_arrayelt.h
${mesa_src_mesa_SOURCE_DIR}/main/api_exec.h
${mesa_src_mesa_SOURCE_DIR}/main/api_loopback.c
${mesa_src_mesa_SOURCE_DIR}/main/api_loopback.h
${mesa_src_mesa_SOURCE_DIR}/main/arbprogram.c
${mesa_src_mesa_SOURCE_DIR}/main/arbprogram.h
${mesa_src_mesa_SOURCE_DIR}/main/arrayobj.c
${mesa_src_mesa_SOURCE_DIR}/main/arrayobj.h
${mesa_src_mesa_SOURCE_DIR}/main/atifragshader.c
${mesa_src_mesa_SOURCE_DIR}/main/atifragshader.h
${mesa_src_mesa_SOURCE_DIR}/main/attrib.c
${mesa_src_mesa_SOURCE_DIR}/main/attrib.h
${mesa_src_mesa_SOURCE_DIR}/main/barrier.c
${mesa_src_mesa_SOURCE_DIR}/main/barrier.h
${mesa_src_mesa_SOURCE_DIR}/main/bbox.c
${mesa_src_mesa_SOURCE_DIR}/main/bbox.h
${mesa_src_mesa_SOURCE_DIR}/main/blend.c
${mesa_src_mesa_SOURCE_DIR}/main/blend.h
${mesa_src_mesa_SOURCE_DIR}/main/blit.c
${mesa_src_mesa_SOURCE_DIR}/main/blit.h
${mesa_src_mesa_SOURCE_DIR}/main/bufferobj.c
${mesa_src_mesa_SOURCE_DIR}/main/bufferobj.h
${mesa_src_mesa_SOURCE_DIR}/main/buffers.c
${mesa_src_mesa_SOURCE_DIR}/main/buffers.h
${mesa_src_mesa_SOURCE_DIR}/main/clear.c
${mesa_src_mesa_SOURCE_DIR}/main/clear.h
${mesa_src_mesa_SOURCE_DIR}/main/clip.c
${mesa_src_mesa_SOURCE_DIR}/main/clip.h
${mesa_src_mesa_SOURCE_DIR}/main/colormac.h
${mesa_src_mesa_SOURCE_DIR}/main/colortab.c
${mesa_src_mesa_SOURCE_DIR}/main/colortab.h
${mesa_src_mesa_SOURCE_DIR}/main/compute.c
${mesa_src_mesa_SOURCE_DIR}/main/compute.h
${mesa_src_mesa_SOURCE_DIR}/main/compiler.h
${mesa_src_mesa_SOURCE_DIR}/main/condrender.c
${mesa_src_mesa_SOURCE_DIR}/main/condrender.h
${mesa_src_mesa_SOURCE_DIR}/main/config.h
${mesa_src_mesa_SOURCE_DIR}/main/conservativeraster.c
${mesa_src_mesa_SOURCE_DIR}/main/conservativeraster.h
${mesa_src_mesa_SOURCE_DIR}/main/context.c
${mesa_src_mesa_SOURCE_DIR}/main/context.h
${mesa_src_mesa_SOURCE_DIR}/main/convolve.c
${mesa_src_mesa_SOURCE_DIR}/main/convolve.h
${mesa_src_mesa_SOURCE_DIR}/main/copyimage.c
${mesa_src_mesa_SOURCE_DIR}/main/copyimage.h
${mesa_src_mesa_SOURCE_DIR}/main/cpuinfo.c
${mesa_src_mesa_SOURCE_DIR}/main/cpuinfo.h
${mesa_src_mesa_SOURCE_DIR}/main/dd.h
${mesa_src_mesa_SOURCE_DIR}/main/debug.c
${mesa_src_mesa_SOURCE_DIR}/main/debug.h
${mesa_src_mesa_SOURCE_DIR}/main/debug_output.c
${mesa_src_mesa_SOURCE_DIR}/main/debug_output.h
${mesa_src_mesa_SOURCE_DIR}/main/depth.c
${mesa_src_mesa_SOURCE_DIR}/main/depth.h
${mesa_src_mesa_SOURCE_DIR}/main/dlist.c
${mesa_src_mesa_SOURCE_DIR}/main/dlist.h
${mesa_src_mesa_SOURCE_DIR}/main/draw.c
${mesa_src_mesa_SOURCE_DIR}/main/draw.h
${mesa_src_mesa_SOURCE_DIR}/main/drawpix.c
${mesa_src_mesa_SOURCE_DIR}/main/drawpix.h
${mesa_src_mesa_SOURCE_DIR}/main/drawtex.c
${mesa_src_mesa_SOURCE_DIR}/main/drawtex.h
${mesa_src_mesa_SOURCE_DIR}/main/draw_validate.c
${mesa_src_mesa_SOURCE_DIR}/main/draw_validate.h
${mesa_src_mesa_SOURCE_DIR}/main/enable.c
${mesa_src_mesa_SOURCE_DIR}/main/enable.h
${mesa_src_mesa_SOURCE_DIR}/main/enums.h
${mesa_src_mesa_SOURCE_DIR}/main/errors.c
${mesa_src_mesa_SOURCE_DIR}/main/errors.h
${mesa_src_mesa_SOURCE_DIR}/main/eval.c
${mesa_src_mesa_SOURCE_DIR}/main/eval.h
${mesa_src_mesa_SOURCE_DIR}/main/execmem.c
${mesa_src_mesa_SOURCE_DIR}/main/extensions.c
${mesa_src_mesa_SOURCE_DIR}/main/extensions.h
${mesa_src_mesa_SOURCE_DIR}/main/extensions_table.c
${mesa_src_mesa_SOURCE_DIR}/main/extensions_table.h
${mesa_src_mesa_SOURCE_DIR}/main/externalobjects.c
${mesa_src_mesa_SOURCE_DIR}/main/externalobjects.h
${mesa_src_mesa_SOURCE_DIR}/main/fbobject.c
${mesa_src_mesa_SOURCE_DIR}/main/fbobject.h
${mesa_src_mesa_SOURCE_DIR}/main/feedback.c
${mesa_src_mesa_SOURCE_DIR}/main/feedback.h
${mesa_src_mesa_SOURCE_DIR}/main/ff_fragment_shader.cpp
${mesa_src_mesa_SOURCE_DIR}/main/ffvertex_prog.c
${mesa_src_mesa_SOURCE_DIR}/main/ffvertex_prog.h
${mesa_src_mesa_SOURCE_DIR}/main/fog.c
${mesa_src_mesa_SOURCE_DIR}/main/fog.h
${mesa_src_mesa_SOURCE_DIR}/main/format_pack.h
${mesa_src_mesa_SOURCE_DIR}/main/format_unpack.h
${mesa_src_mesa_SOURCE_DIR}/main/formatquery.c
${mesa_src_mesa_SOURCE_DIR}/main/formatquery.h
${mesa_src_mesa_SOURCE_DIR}/main/formats.c
${mesa_src_mesa_SOURCE_DIR}/main/formats.h
${mesa_src_mesa_SOURCE_DIR}/main/format_utils.c
${mesa_src_mesa_SOURCE_DIR}/main/format_utils.h
${mesa_src_mesa_SOURCE_DIR}/main/framebuffer.c
${mesa_src_mesa_SOURCE_DIR}/main/framebuffer.h
${mesa_src_mesa_SOURCE_DIR}/main/get.c
${mesa_src_mesa_SOURCE_DIR}/main/get.h
${mesa_src_mesa_SOURCE_DIR}/main/genmipmap.c
${mesa_src_mesa_SOURCE_DIR}/main/genmipmap.h
${mesa_src_mesa_SOURCE_DIR}/main/getstring.c
${mesa_src_mesa_SOURCE_DIR}/main/glformats.c
${mesa_src_mesa_SOURCE_DIR}/main/glformats.h
${mesa_src_mesa_SOURCE_DIR}/main/glspirv.c
${mesa_src_mesa_SOURCE_DIR}/main/glspirv.h
${mesa_src_mesa_SOURCE_DIR}/main/glthread.c
${mesa_src_mesa_SOURCE_DIR}/main/glthread.h
${mesa_src_mesa_SOURCE_DIR}/main/glheader.h
${mesa_src_mesa_SOURCE_DIR}/main/hash.c
${mesa_src_mesa_SOURCE_DIR}/main/hash.h
${mesa_src_mesa_SOURCE_DIR}/main/hint.c
${mesa_src_mesa_SOURCE_DIR}/main/hint.h
${mesa_src_mesa_SOURCE_DIR}/main/histogram.c
${mesa_src_mesa_SOURCE_DIR}/main/histogram.h
${mesa_src_mesa_SOURCE_DIR}/main/image.c
${mesa_src_mesa_SOURCE_DIR}/main/image.h
${mesa_src_mesa_SOURCE_DIR}/main/imports.c
${mesa_src_mesa_SOURCE_DIR}/main/imports.h
${mesa_src_mesa_SOURCE_DIR}/main/light.c
${mesa_src_mesa_SOURCE_DIR}/main/light.h
${mesa_src_mesa_SOURCE_DIR}/main/lines.c
${mesa_src_mesa_SOURCE_DIR}/main/lines.h
${mesa_src_mesa_SOURCE_DIR}/main/macros.h
${mesa_src_mesa_SOURCE_DIR}/main/marshal.c
${mesa_src_mesa_SOURCE_DIR}/main/marshal.h
${mesa_src_mesa_SOURCE_DIR}/main/matrix.c
${mesa_src_mesa_SOURCE_DIR}/main/matrix.h
${mesa_src_mesa_SOURCE_DIR}/main/mipmap.c
${mesa_src_mesa_SOURCE_DIR}/main/mipmap.h
${mesa_src_mesa_SOURCE_DIR}/main/mm.c
${mesa_src_mesa_SOURCE_DIR}/main/mm.h
${mesa_src_mesa_SOURCE_DIR}/main/menums.h
${mesa_src_mesa_SOURCE_DIR}/main/mtypes.h
${mesa_src_mesa_SOURCE_DIR}/main/multisample.c
${mesa_src_mesa_SOURCE_DIR}/main/multisample.h
${mesa_src_mesa_SOURCE_DIR}/main/objectlabel.c
${mesa_src_mesa_SOURCE_DIR}/main/objectlabel.h
${mesa_src_mesa_SOURCE_DIR}/main/objectpurge.c
${mesa_src_mesa_SOURCE_DIR}/main/objectpurge.h
${mesa_src_mesa_SOURCE_DIR}/main/pack.c
${mesa_src_mesa_SOURCE_DIR}/main/pack.h
${mesa_src_mesa_SOURCE_DIR}/main/pbo.c
${mesa_src_mesa_SOURCE_DIR}/main/pbo.h
${mesa_src_mesa_SOURCE_DIR}/main/performance_monitor.c
${mesa_src_mesa_SOURCE_DIR}/main/performance_monitor.h
${mesa_src_mesa_SOURCE_DIR}/main/performance_query.c
${mesa_src_mesa_SOURCE_DIR}/main/performance_query.h
${mesa_src_mesa_SOURCE_DIR}/main/pipelineobj.c
${mesa_src_mesa_SOURCE_DIR}/main/pipelineobj.h
${mesa_src_mesa_SOURCE_DIR}/main/pixel.c
${mesa_src_mesa_SOURCE_DIR}/main/pixel.h
${mesa_src_mesa_SOURCE_DIR}/main/pixelstore.c
${mesa_src_mesa_SOURCE_DIR}/main/pixelstore.h
${mesa_src_mesa_SOURCE_DIR}/main/pixeltransfer.c
${mesa_src_mesa_SOURCE_DIR}/main/pixeltransfer.h
${mesa_src_mesa_SOURCE_DIR}/main/points.c
${mesa_src_mesa_SOURCE_DIR}/main/points.h
${mesa_src_mesa_SOURCE_DIR}/main/polygon.c
${mesa_src_mesa_SOURCE_DIR}/main/polygon.h
${mesa_src_mesa_SOURCE_DIR}/main/program_binary.c
${mesa_src_mesa_SOURCE_DIR}/main/program_binary.h
${mesa_src_mesa_SOURCE_DIR}/main/program_resource.c
${mesa_src_mesa_SOURCE_DIR}/main/program_resource.h
${mesa_src_mesa_SOURCE_DIR}/main/querymatrix.c
${mesa_src_mesa_SOURCE_DIR}/main/querymatrix.h
${mesa_src_mesa_SOURCE_DIR}/main/queryobj.c
${mesa_src_mesa_SOURCE_DIR}/main/queryobj.h
${mesa_src_mesa_SOURCE_DIR}/main/rastpos.c
${mesa_src_mesa_SOURCE_DIR}/main/rastpos.h
${mesa_src_mesa_SOURCE_DIR}/main/readpix.c
${mesa_src_mesa_SOURCE_DIR}/main/readpix.h
${mesa_src_mesa_SOURCE_DIR}/main/remap.c
${mesa_src_mesa_SOURCE_DIR}/main/remap.h
${mesa_src_mesa_SOURCE_DIR}/main/renderbuffer.c
${mesa_src_mesa_SOURCE_DIR}/main/renderbuffer.h
${mesa_src_mesa_SOURCE_DIR}/main/robustness.c
${mesa_src_mesa_SOURCE_DIR}/main/samplerobj.c
${mesa_src_mesa_SOURCE_DIR}/main/samplerobj.h
${mesa_src_mesa_SOURCE_DIR}/main/scissor.c
${mesa_src_mesa_SOURCE_DIR}/main/scissor.h
${mesa_src_mesa_SOURCE_DIR}/main/shaderapi.c
${mesa_src_mesa_SOURCE_DIR}/main/shaderapi.h
${mesa_src_mesa_SOURCE_DIR}/main/shaderimage.c
${mesa_src_mesa_SOURCE_DIR}/main/shaderimage.h
${mesa_src_mesa_SOURCE_DIR}/main/shaderobj.c
${mesa_src_mesa_SOURCE_DIR}/main/shaderobj.h
${mesa_src_mesa_SOURCE_DIR}/main/shader_query.cpp
${mesa_src_mesa_SOURCE_DIR}/main/shared.c
${mesa_src_mesa_SOURCE_DIR}/main/shared.h
${mesa_src_mesa_SOURCE_DIR}/main/state.c
${mesa_src_mesa_SOURCE_DIR}/main/state.h
${mesa_src_mesa_SOURCE_DIR}/main/stencil.c
${mesa_src_mesa_SOURCE_DIR}/main/stencil.h
${mesa_src_mesa_SOURCE_DIR}/main/syncobj.c
${mesa_src_mesa_SOURCE_DIR}/main/syncobj.h
${mesa_src_mesa_SOURCE_DIR}/main/texcompress.c
${mesa_src_mesa_SOURCE_DIR}/main/texcompress_astc.cpp
${mesa_src_mesa_SOURCE_DIR}/main/texcompress_astc.h
${mesa_src_mesa_SOURCE_DIR}/main/texcompress_bptc.c
${mesa_src_mesa_SOURCE_DIR}/main/texcompress_bptc.h
${mesa_src_mesa_SOURCE_DIR}/main/texcompress_cpal.c
${mesa_src_mesa_SOURCE_DIR}/main/texcompress_cpal.h
${mesa_src_mesa_SOURCE_DIR}/main/texcompress_etc.c
${mesa_src_mesa_SOURCE_DIR}/main/texcompress_etc.h
${mesa_src_mesa_SOURCE_DIR}/main/texcompress_etc_tmp.h
${mesa_src_mesa_SOURCE_DIR}/main/texcompress_fxt1.c
${mesa_src_mesa_SOURCE_DIR}/main/texcompress_fxt1.h
${mesa_src_mesa_SOURCE_DIR}/main/texcompress.h
${mesa_src_mesa_SOURCE_DIR}/main/texcompress_rgtc.c
${mesa_src_mesa_SOURCE_DIR}/main/texcompress_rgtc.h
${mesa_src_mesa_SOURCE_DIR}/main/texcompress_s3tc.c
${mesa_src_mesa_SOURCE_DIR}/main/texcompress_s3tc.h
${mesa_src_mesa_SOURCE_DIR}/main/texenv.c
${mesa_src_mesa_SOURCE_DIR}/main/texenv.h
${mesa_src_mesa_SOURCE_DIR}/main/texenvprogram.h
${mesa_src_mesa_SOURCE_DIR}/main/texformat.c
${mesa_src_mesa_SOURCE_DIR}/main/texformat.h
${mesa_src_mesa_SOURCE_DIR}/main/texgen.c
${mesa_src_mesa_SOURCE_DIR}/main/texgen.h
${mesa_src_mesa_SOURCE_DIR}/main/texgetimage.c
${mesa_src_mesa_SOURCE_DIR}/main/texgetimage.h
${mesa_src_mesa_SOURCE_DIR}/main/teximage.c
${mesa_src_mesa_SOURCE_DIR}/main/teximage.h
${mesa_src_mesa_SOURCE_DIR}/main/texobj.c
${mesa_src_mesa_SOURCE_DIR}/main/texobj.h
${mesa_src_mesa_SOURCE_DIR}/main/texparam.c
${mesa_src_mesa_SOURCE_DIR}/main/texparam.h
${mesa_src_mesa_SOURCE_DIR}/main/texstate.c
${mesa_src_mesa_SOURCE_DIR}/main/texstate.h
${mesa_src_mesa_SOURCE_DIR}/main/texstorage.c
${mesa_src_mesa_SOURCE_DIR}/main/texstorage.h
${mesa_src_mesa_SOURCE_DIR}/main/texstore.c
${mesa_src_mesa_SOURCE_DIR}/main/texstore.h
${mesa_src_mesa_SOURCE_DIR}/main/texturebindless.c
${mesa_src_mesa_SOURCE_DIR}/main/texturebindless.h
${mesa_src_mesa_SOURCE_DIR}/main/textureview.c
${mesa_src_mesa_SOURCE_DIR}/main/textureview.h
${mesa_src_mesa_SOURCE_DIR}/main/transformfeedback.c
${mesa_src_mesa_SOURCE_DIR}/main/transformfeedback.h
${mesa_src_mesa_SOURCE_DIR}/main/uniform_query.cpp
${mesa_src_mesa_SOURCE_DIR}/main/uniforms.c
${mesa_src_mesa_SOURCE_DIR}/main/uniforms.h
${mesa_src_mesa_SOURCE_DIR}/main/varray.c
${mesa_src_mesa_SOURCE_DIR}/main/varray.h
${mesa_src_mesa_SOURCE_DIR}/main/vdpau.c
${mesa_src_mesa_SOURCE_DIR}/main/vdpau.h
${mesa_src_mesa_SOURCE_DIR}/main/version.c
${mesa_src_mesa_SOURCE_DIR}/main/version.h
${mesa_src_mesa_SOURCE_DIR}/main/viewport.c
${mesa_src_mesa_SOURCE_DIR}/main/viewport.h
${mesa_src_mesa_SOURCE_DIR}/main/vtxfmt.c
${mesa_src_mesa_SOURCE_DIR}/main/vtxfmt.h
${mesa_src_mesa_SOURCE_DIR}/main/es1_conversion.c
${mesa_src_mesa_SOURCE_DIR}/main/es1_conversion.h
${mesa_src_mesa_SOURCE_DIR}/math/m_debug.h
${mesa_src_mesa_SOURCE_DIR}/math/m_debug_clip.c
${mesa_src_mesa_SOURCE_DIR}/math/m_debug_norm.c
${mesa_src_mesa_SOURCE_DIR}/math/m_debug_util.h
${mesa_src_mesa_SOURCE_DIR}/math/m_debug_xform.c
${mesa_src_mesa_SOURCE_DIR}/math/m_eval.c
${mesa_src_mesa_SOURCE_DIR}/math/m_eval.h
${mesa_src_mesa_SOURCE_DIR}/math/m_matrix.c
${mesa_src_mesa_SOURCE_DIR}/math/m_matrix.h
${mesa_src_mesa_SOURCE_DIR}/math/m_trans_tmp.h
${mesa_src_mesa_SOURCE_DIR}/math/m_translate.c
${mesa_src_mesa_SOURCE_DIR}/math/m_translate.h
${mesa_src_mesa_SOURCE_DIR}/math/m_vector.c
${mesa_src_mesa_SOURCE_DIR}/math/m_vector.h
${mesa_src_mesa_SOURCE_DIR}/vbo/vbo_attrib.h
${mesa_src_mesa_SOURCE_DIR}/vbo/vbo_attrib_tmp.h
${mesa_src_mesa_SOURCE_DIR}/vbo/vbo_context.c
${mesa_src_mesa_SOURCE_DIR}/vbo/vbo_exec_api.c
${mesa_src_mesa_SOURCE_DIR}/vbo/vbo_exec.c
${mesa_src_mesa_SOURCE_DIR}/vbo/vbo_exec_draw.c
${mesa_src_mesa_SOURCE_DIR}/vbo/vbo_exec_eval.c
${mesa_src_mesa_SOURCE_DIR}/vbo/vbo_exec.h
${mesa_src_mesa_SOURCE_DIR}/vbo/vbo.h
${mesa_src_mesa_SOURCE_DIR}/vbo/vbo_minmax_index.c
${mesa_src_mesa_SOURCE_DIR}/vbo/vbo_noop.c
${mesa_src_mesa_SOURCE_DIR}/vbo/vbo_noop.h
${mesa_src_mesa_SOURCE_DIR}/vbo/vbo_primitive_restart.c
${mesa_src_mesa_SOURCE_DIR}/vbo/vbo_save_api.c
${mesa_src_mesa_SOURCE_DIR}/vbo/vbo_save.c
${mesa_src_mesa_SOURCE_DIR}/vbo/vbo_save_draw.c
${mesa_src_mesa_SOURCE_DIR}/vbo/vbo_save.h
${mesa_src_mesa_SOURCE_DIR}/vbo/vbo_save_loopback.c
${mesa_src_mesa_SOURCE_DIR}/x86/common_x86.c
)

set(files_libmesa_classic
${mesa_src_mesa_SOURCE_DIR}/math/m_clip_tmp.h
${mesa_src_mesa_SOURCE_DIR}/math/m_copy_tmp.h
${mesa_src_mesa_SOURCE_DIR}/math/m_dotprod_tmp.h
${mesa_src_mesa_SOURCE_DIR}/math/m_norm_tmp.h
${mesa_src_mesa_SOURCE_DIR}/math/m_xform.c
${mesa_src_mesa_SOURCE_DIR}/math/m_xform.h
${mesa_src_mesa_SOURCE_DIR}/math/m_xform_tmp.h
${mesa_src_mesa_SOURCE_DIR}/tnl/t_context.c
${mesa_src_mesa_SOURCE_DIR}/tnl/t_context.h
${mesa_src_mesa_SOURCE_DIR}/tnl/t_draw.c
${mesa_src_mesa_SOURCE_DIR}/tnl/tnl.h
${mesa_src_mesa_SOURCE_DIR}/tnl/t_pipeline.c
${mesa_src_mesa_SOURCE_DIR}/tnl/t_pipeline.h
${mesa_src_mesa_SOURCE_DIR}/tnl/t_rebase.c
${mesa_src_mesa_SOURCE_DIR}/tnl/t_split.c
${mesa_src_mesa_SOURCE_DIR}/tnl/t_split_copy.c
${mesa_src_mesa_SOURCE_DIR}/tnl/t_split.h
${mesa_src_mesa_SOURCE_DIR}/tnl/t_split_inplace.c
${mesa_src_mesa_SOURCE_DIR}/tnl/t_vb_cliptmp.h
${mesa_src_mesa_SOURCE_DIR}/tnl/t_vb_fog.c
${mesa_src_mesa_SOURCE_DIR}/tnl/t_vb_light.c
${mesa_src_mesa_SOURCE_DIR}/tnl/t_vb_lighttmp.h
${mesa_src_mesa_SOURCE_DIR}/tnl/t_vb_normals.c
${mesa_src_mesa_SOURCE_DIR}/tnl/t_vb_points.c
${mesa_src_mesa_SOURCE_DIR}/tnl/t_vb_program.c
${mesa_src_mesa_SOURCE_DIR}/tnl/t_vb_render.c
${mesa_src_mesa_SOURCE_DIR}/tnl/t_vb_rendertmp.h
${mesa_src_mesa_SOURCE_DIR}/tnl/t_vb_texgen.c
${mesa_src_mesa_SOURCE_DIR}/tnl/t_vb_texmat.c
${mesa_src_mesa_SOURCE_DIR}/tnl/t_vb_vertex.c
${mesa_src_mesa_SOURCE_DIR}/tnl/t_vertex.c
${mesa_src_mesa_SOURCE_DIR}/tnl/t_vertex_generic.c
${mesa_src_mesa_SOURCE_DIR}/tnl/t_vertex.h
${mesa_src_mesa_SOURCE_DIR}/tnl/t_vertex_sse.c
${mesa_src_mesa_SOURCE_DIR}/tnl/t_vp_build.c
${mesa_src_mesa_SOURCE_DIR}/tnl/t_vp_build.h
${mesa_src_mesa_SOURCE_DIR}/swrast/s_aaline.c
${mesa_src_mesa_SOURCE_DIR}/swrast/s_aaline.h
${mesa_src_mesa_SOURCE_DIR}/swrast/s_aalinetemp.h
${mesa_src_mesa_SOURCE_DIR}/swrast/s_aatriangle.c
${mesa_src_mesa_SOURCE_DIR}/swrast/s_aatriangle.h
${mesa_src_mesa_SOURCE_DIR}/swrast/s_aatritemp.h
${mesa_src_mesa_SOURCE_DIR}/swrast/s_alpha.c
${mesa_src_mesa_SOURCE_DIR}/swrast/s_alpha.h
${mesa_src_mesa_SOURCE_DIR}/swrast/s_atifragshader.c
${mesa_src_mesa_SOURCE_DIR}/swrast/s_atifragshader.h
${mesa_src_mesa_SOURCE_DIR}/swrast/s_bitmap.c
${mesa_src_mesa_SOURCE_DIR}/swrast/s_blend.c
${mesa_src_mesa_SOURCE_DIR}/swrast/s_blend.h
${mesa_src_mesa_SOURCE_DIR}/swrast/s_blit.c
${mesa_src_mesa_SOURCE_DIR}/swrast/s_chan.h
${mesa_src_mesa_SOURCE_DIR}/swrast/s_clear.c
${mesa_src_mesa_SOURCE_DIR}/swrast/s_context.c
${mesa_src_mesa_SOURCE_DIR}/swrast/s_context.h
${mesa_src_mesa_SOURCE_DIR}/swrast/s_copypix.c
${mesa_src_mesa_SOURCE_DIR}/swrast/s_depth.c
${mesa_src_mesa_SOURCE_DIR}/swrast/s_depth.h
${mesa_src_mesa_SOURCE_DIR}/swrast/s_drawpix.c
${mesa_src_mesa_SOURCE_DIR}/swrast_setup/ss_tritmp.h
${mesa_src_mesa_SOURCE_DIR}/swrast_setup/ss_vb.h
${mesa_src_mesa_SOURCE_DIR}/swrast_setup/swrast_setup.h
${mesa_src_mesa_SOURCE_DIR}/swrast/s_feedback.c
${mesa_src_mesa_SOURCE_DIR}/swrast/s_feedback.h
${mesa_src_mesa_SOURCE_DIR}/swrast/s_fog.c
${mesa_src_mesa_SOURCE_DIR}/swrast/s_fog.h
${mesa_src_mesa_SOURCE_DIR}/swrast/s_fragprog.c
${mesa_src_mesa_SOURCE_DIR}/swrast/s_fragprog.h
${mesa_src_mesa_SOURCE_DIR}/swrast/s_lines.c
${mesa_src_mesa_SOURCE_DIR}/swrast/s_lines.h
${mesa_src_mesa_SOURCE_DIR}/swrast/s_linetemp.h
${mesa_src_mesa_SOURCE_DIR}/swrast/s_logic.c
${mesa_src_mesa_SOURCE_DIR}/swrast/s_logic.h
${mesa_src_mesa_SOURCE_DIR}/swrast/s_masking.c
${mesa_src_mesa_SOURCE_DIR}/swrast/s_masking.h
${mesa_src_mesa_SOURCE_DIR}/swrast/s_points.c
${mesa_src_mesa_SOURCE_DIR}/swrast/s_points.h
${mesa_src_mesa_SOURCE_DIR}/swrast/s_renderbuffer.c
${mesa_src_mesa_SOURCE_DIR}/swrast/s_renderbuffer.h
${mesa_src_mesa_SOURCE_DIR}/swrast/s_span.c
${mesa_src_mesa_SOURCE_DIR}/swrast/s_span.h
${mesa_src_mesa_SOURCE_DIR}/swrast/s_stencil.c
${mesa_src_mesa_SOURCE_DIR}/swrast/s_stencil.h
${mesa_src_mesa_SOURCE_DIR}/swrast/s_texcombine.c
${mesa_src_mesa_SOURCE_DIR}/swrast/s_texcombine.h
${mesa_src_mesa_SOURCE_DIR}/swrast/s_texfetch.c
${mesa_src_mesa_SOURCE_DIR}/swrast/s_texfetch.h
${mesa_src_mesa_SOURCE_DIR}/swrast/s_texfetch_tmp.h
${mesa_src_mesa_SOURCE_DIR}/swrast/s_texfilter.c
${mesa_src_mesa_SOURCE_DIR}/swrast/s_texfilter.h
${mesa_src_mesa_SOURCE_DIR}/swrast/s_texrender.c
${mesa_src_mesa_SOURCE_DIR}/swrast/s_texture.c
${mesa_src_mesa_SOURCE_DIR}/swrast/s_triangle.c
${mesa_src_mesa_SOURCE_DIR}/swrast/s_triangle.h
${mesa_src_mesa_SOURCE_DIR}/swrast/s_tritemp.h
${mesa_src_mesa_SOURCE_DIR}/swrast/swrast.h
${mesa_src_mesa_SOURCE_DIR}/swrast/s_zoom.c
${mesa_src_mesa_SOURCE_DIR}/swrast/s_zoom.h
${mesa_src_mesa_SOURCE_DIR}/swrast_setup/ss_context.c
${mesa_src_mesa_SOURCE_DIR}/swrast_setup/ss_context.h
${mesa_src_mesa_SOURCE_DIR}/swrast_setup/ss_triangle.c
${mesa_src_mesa_SOURCE_DIR}/swrast_setup/ss_triangle.h
${mesa_src_mesa_SOURCE_DIR}/drivers/common/driverfuncs.c
${mesa_src_mesa_SOURCE_DIR}/drivers/common/driverfuncs.h
${mesa_src_mesa_SOURCE_DIR}/drivers/common/meta_blit.c
${mesa_src_mesa_SOURCE_DIR}/drivers/common/meta_generate_mipmap.c
${mesa_src_mesa_SOURCE_DIR}/drivers/common/meta.c
${mesa_src_mesa_SOURCE_DIR}/drivers/common/meta.h
${mesa_src_mesa_SOURCE_DIR}/x86/x86_xform.c
${mesa_src_mesa_SOURCE_DIR}/x86/3dnow.c
${mesa_src_mesa_SOURCE_DIR}/x86/sse.c
${mesa_src_mesa_SOURCE_DIR}/x86/rtasm/x86sse.c
${mesa_src_mesa_SOURCE_DIR}/x86/rtasm/x86sse.h
${mesa_src_mesa_SOURCE_DIR}/sparc/sparc.c
${mesa_src_mesa_SOURCE_DIR}/x86-64/x86-64.c
)

set(files_libmesa_gallium
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_atifs_to_tgsi.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_atifs_to_tgsi.h
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_atom_array.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_atom_atomicbuf.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_atom_blend.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_atom.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_atom_clip.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_atom_constbuf.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_atom_constbuf.h
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_atom_depth.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_atom_framebuffer.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_atom.h
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_atom_list.h
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_atom_image.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_atom_msaa.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_atom_pixeltransfer.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_atom_rasterizer.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_atom_sampler.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_atom_scissor.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_atom_shader.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_atom_stipple.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_atom_storagebuf.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_atom_tess.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_atom_texture.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_atom_viewport.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_cb_bitmap.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_cb_bitmap.h
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_cb_bitmap_shader.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_cb_blit.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_cb_blit.h
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_cb_bufferobjects.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_cb_bufferobjects.h
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_cb_clear.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_cb_clear.h
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_cb_compute.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_cb_compute.h
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_cb_condrender.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_cb_condrender.h
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_cb_copyimage.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_cb_copyimage.h
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_cb_drawpixels.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_cb_drawpixels.h
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_cb_drawpixels_shader.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_cb_drawtex.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_cb_drawtex.h
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_cb_eglimage.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_cb_eglimage.h
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_cb_fbo.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_cb_fbo.h
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_cb_feedback.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_cb_feedback.h
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_cb_flush.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_cb_flush.h
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_cb_memoryobjects.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_cb_memoryobjects.h
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_cb_msaa.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_cb_msaa.h
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_cb_perfmon.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_cb_perfmon.h
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_cb_program.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_cb_program.h
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_cb_queryobj.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_cb_queryobj.h
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_cb_rasterpos.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_cb_rasterpos.h
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_cb_readpixels.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_cb_readpixels.h
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_cb_strings.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_cb_strings.h
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_cb_semaphoreobjects.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_cb_semaphoreobjects.h
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_cb_syncobj.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_cb_syncobj.h
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_cb_texturebarrier.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_cb_texturebarrier.h
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_cb_texture.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_cb_texture.h
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_cb_viewport.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_cb_viewport.h
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_cb_xformfb.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_cb_xformfb.h
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_context.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_context.h
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_copytex.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_copytex.h
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_debug.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_debug.h
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_draw.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_draw_feedback.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_draw.h
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_extensions.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_extensions.h
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_format.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_format.h
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_gen_mipmap.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_gen_mipmap.h
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_gl_api.h
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_glsl_to_ir.cpp
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_glsl_to_ir.h
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_glsl_to_nir.cpp
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_glsl_to_tgsi.cpp
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_glsl_to_tgsi.h
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_glsl_to_tgsi_array_merge.cpp
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_glsl_to_tgsi_array_merge.h
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_glsl_to_tgsi_private.cpp
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_glsl_to_tgsi_private.h
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_glsl_to_tgsi_temprename.cpp
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_glsl_to_tgsi_temprename.h
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_glsl_types.cpp
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_glsl_types.h
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_manager.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_manager.h
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_mesa_to_tgsi.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_mesa_to_tgsi.h
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_nir.h
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_nir_builtins.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_nir_lower_builtin.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_nir_lower_tex_src_plane.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_pbo.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_pbo.h
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_program.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_program.h
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_sampler_view.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_sampler_view.h
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_scissor.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_scissor.h
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_shader_cache.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_shader_cache.h
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_texture.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_texture.h
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_tgsi_lower_yuv.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_tgsi_lower_yuv.h
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_util.h
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_vdpau.c
${mesa_src_mesa_SOURCE_DIR}/state_tracker/st_vdpau.h
)

execute_process(
COMMAND ${PYTHON_EXECUTABLE} ${mesa_src_mesa_SOURCE_DIR}/main/format_fallback.py ${mesa_src_mesa_SOURCE_DIR}/main/formats.csv ${mesa_src_mesa_SOURCE_DIR}/format_fallback.c
WORKING_DIRECTORY ${mesa_src_mesa_SOURCE_DIR}
)

execute_process(
COMMAND ${PYTHON_EXECUTABLE} ${mesa_src_mesa_SOURCE_DIR}/main/get_hash_generator.py -f ${gl_and_es_api_files}
OUTPUT_FILE ${mesa_src_mesa_SOURCE_DIR}/get_hash.h
WORKING_DIRECTORY ${mesa_src_mesa_SOURCE_DIR}
)

execute_process(
COMMAND ${PYTHON_EXECUTABLE} ${mesa_src_mesa_SOURCE_DIR}/main/format_info.py ${mesa_src_mesa_SOURCE_DIR}/main/formats.csv
OUTPUT_FILE ${mesa_src_mesa_SOURCE_DIR}/format_info.h
WORKING_DIRECTORY ${mesa_src_mesa_SOURCE_DIR}
)

execute_process(
COMMAND ${PYTHON_EXECUTABLE} ${mesa_src_mesa_SOURCE_DIR}/main/format_pack.py ${mesa_src_mesa_SOURCE_DIR}/main/formats.csv
OUTPUT_FILE ${mesa_src_mesa_SOURCE_DIR}/format_pack.c
WORKING_DIRECTORY ${mesa_src_mesa_SOURCE_DIR}
)

execute_process(
COMMAND ${PYTHON_EXECUTABLE} ${mesa_src_mesa_SOURCE_DIR}/main/format_unpack.py ${mesa_src_mesa_SOURCE_DIR}/main/formats.csv
OUTPUT_FILE ${mesa_src_mesa_SOURCE_DIR}/format_unpack.c
WORKING_DIRECTORY ${mesa_src_mesa_SOURCE_DIR}
)

set(files_libmesa_common
${files_libmesa_common}
${mesa_src_mesa_SOURCE_DIR}/format_info.h
${mesa_src_mesa_SOURCE_DIR}/format_pack.c
${mesa_src_mesa_SOURCE_DIR}/format_unpack.c
${mesa_src_mesa_SOURCE_DIR}/program/lex.yy.c
${mesa_src_mesa_SOURCE_DIR}/program/program_parse.tab.c
${mesa_src_mesa_SOURCE_DIR}/program/program_parse.tab.h
${mesa_src_mesa_SOURCE_DIR}/../mapi/glapi/gen/api_exec.c
${mesa_src_mesa_SOURCE_DIR}/../mapi/glapi/gen/enums.c
${mesa_src_mesa_SOURCE_DIR}/format_fallback.c
${mesa_src_mesa_SOURCE_DIR}/get_hash.h
${mesa_src_mesa_SOURCE_DIR}/../mapi/glapi/gen/marshal_generated.c
${mesa_src_mesa_SOURCE_DIR}/main/marshal_generated.h
${mesa_src_mesa_SOURCE_DIR}/main/dispatch.h
${mesa_src_mesa_SOURCE_DIR}/../compiler/ir_expression_operation.h
${mesa_src_mesa_SOURCE_DIR}/main/remap_helper.h
# matypes_h
${mesa_src_SOURCE_DIR}/git_sha1.h
)

include_directories(${mesa_src_mesa_SOURCE_DIR}/main)
include_directories(${mesa_src_SOURCE_DIR}/compiler)
add_library(mesa_classic STATIC
${files_libmesa_common}
${files_libmesa_classic}
)
target_link_libraries(mesa_classic glsl)

vtsl_set_crt(mesa_classic)

add_subdirectory(program)
add_subdirectory(main)
add_subdirectory(drivers)

# Gallium is not required for now
#add_library(mesa_gallium
#${files_libmesa_common}
#${files_libmesa_gallium}
#)
#target_link_libraries(mesa_gallium glsl)

#subdir('program')
#subdir('main')
#
## files shared between classic mesa and gallium mesa
#files_libmesa_common = files(
#  'program/arbprogparse.c',
#  'program/arbprogparse.h',
#  'program/ir_to_mesa.cpp',
#  'program/ir_to_mesa.h',
#  'program/prog_cache.c',
#  'program/prog_cache.h',
#  'program/prog_execute.c',
#  'program/prog_execute.h',
#  'program/prog_instruction.c',
#  'program/prog_instruction.h',
#  'program/prog_noise.c',
#  'program/prog_noise.h',
#  'program/prog_opt_constant_fold.c',
#  'program/prog_optimize.c',
#  'program/prog_optimize.h',
#  'program/prog_parameter.c',
#  'program/prog_parameter.h',
#  'program/prog_parameter_layout.c',
#  'program/prog_parameter_layout.h',
#  'program/prog_print.c',
#  'program/prog_print.h',
#  'program/program.c',
#  'program/program.h',
#  'program/programopt.c',
#  'program/programopt.h',
#  'program/program_parse_extra.c',
#  'program/program_parser.h',
#  'program/prog_statevars.c',
#  'program/prog_statevars.h',
#  'program/symbol_table.c',
#  'program/symbol_table.h',
#  'program/prog_to_nir.c',
#  'program/prog_to_nir.h',
#  'main/accum.c',
#  'main/accum.h',
#  'main/api_arrayelt.c',
#  'main/api_arrayelt.h',
#  'main/api_exec.h',
#  'main/api_loopback.c',
#  'main/api_loopback.h',
#  'main/arbprogram.c',
#  'main/arbprogram.h',
#  'main/arrayobj.c',
#  'main/arrayobj.h',
#  'main/atifragshader.c',
#  'main/atifragshader.h',
#  'main/attrib.c',
#  'main/attrib.h',
#  'main/barrier.c',
#  'main/barrier.h',
#  'main/bbox.c',
#  'main/bbox.h',
#  'main/blend.c',
#  'main/blend.h',
#  'main/blit.c',
#  'main/blit.h',
#  'main/bufferobj.c',
#  'main/bufferobj.h',
#  'main/buffers.c',
#  'main/buffers.h',
#  'main/clear.c',
#  'main/clear.h',
#  'main/clip.c',
#  'main/clip.h',
#  'main/colormac.h',
#  'main/colortab.c',
#  'main/colortab.h',
#  'main/compute.c',
#  'main/compute.h',
#  'main/compiler.h',
#  'main/condrender.c',
#  'main/condrender.h',
#  'main/config.h',
#  'main/conservativeraster.c',
#  'main/conservativeraster.h',
#  'main/context.c',
#  'main/context.h',
#  'main/convolve.c',
#  'main/convolve.h',
#  'main/copyimage.c',
#  'main/copyimage.h',
#  'main/cpuinfo.c',
#  'main/cpuinfo.h',
#  'main/dd.h',
#  'main/debug.c',
#  'main/debug.h',
#  'main/debug_output.c',
#  'main/debug_output.h',
#  'main/depth.c',
#  'main/depth.h',
#  'main/dlist.c',
#  'main/dlist.h',
#  'main/draw.c',
#  'main/draw.h',
#  'main/drawpix.c',
#  'main/drawpix.h',
#  'main/drawtex.c',
#  'main/drawtex.h',
#  'main/draw_validate.c',
#  'main/draw_validate.h',
#  'main/enable.c',
#  'main/enable.h',
#  'main/enums.h',
#  'main/errors.c',
#  'main/errors.h',
#  'main/eval.c',
#  'main/eval.h',
#  'main/execmem.c',
#  'main/extensions.c',
#  'main/extensions.h',
#  'main/extensions_table.c',
#  'main/extensions_table.h',
#  'main/externalobjects.c',
#  'main/externalobjects.h',
#  'main/fbobject.c',
#  'main/fbobject.h',
#  'main/feedback.c',
#  'main/feedback.h',
#  'main/ff_fragment_shader.cpp',
#  'main/ffvertex_prog.c',
#  'main/ffvertex_prog.h',
#  'main/fog.c',
#  'main/fog.h',
#  'main/format_pack.h',
#  'main/format_unpack.h',
#  'main/formatquery.c',
#  'main/formatquery.h',
#  'main/formats.c',
#  'main/formats.h',
#  'main/format_utils.c',
#  'main/format_utils.h',
#  'main/framebuffer.c',
#  'main/framebuffer.h',
#  'main/get.c',
#  'main/get.h',
#  'main/genmipmap.c',
#  'main/genmipmap.h',
#  'main/getstring.c',
#  'main/glformats.c',
#  'main/glformats.h',
#  'main/glspirv.c',
#  'main/glspirv.h',
#  'main/glthread.c',
#  'main/glthread.h',
#  'main/glheader.h',
#  'main/hash.c',
#  'main/hash.h',
#  'main/hint.c',
#  'main/hint.h',
#  'main/histogram.c',
#  'main/histogram.h',
#  'main/image.c',
#  'main/image.h',
#  'main/imports.c',
#  'main/imports.h',
#  'main/light.c',
#  'main/light.h',
#  'main/lines.c',
#  'main/lines.h',
#  'main/macros.h',
#  'main/marshal.c',
#  'main/marshal.h',
#  'main/matrix.c',
#  'main/matrix.h',
#  'main/mipmap.c',
#  'main/mipmap.h',
#  'main/mm.c',
#  'main/mm.h',
#  'main/menums.h',
#  'main/mtypes.h',
#  'main/multisample.c',
#  'main/multisample.h',
#  'main/objectlabel.c',
#  'main/objectlabel.h',
#  'main/objectpurge.c',
#  'main/objectpurge.h',
#  'main/pack.c',
#  'main/pack.h',
#  'main/pbo.c',
#  'main/pbo.h',
#  'main/performance_monitor.c',
#  'main/performance_monitor.h',
#  'main/performance_query.c',
#  'main/performance_query.h',
#  'main/pipelineobj.c',
#  'main/pipelineobj.h',
#  'main/pixel.c',
#  'main/pixel.h',
#  'main/pixelstore.c',
#  'main/pixelstore.h',
#  'main/pixeltransfer.c',
#  'main/pixeltransfer.h',
#  'main/points.c',
#  'main/points.h',
#  'main/polygon.c',
#  'main/polygon.h',
#  'main/program_binary.c',
#  'main/program_binary.h',
#  'main/program_resource.c',
#  'main/program_resource.h',
#  'main/querymatrix.c',
#  'main/querymatrix.h',
#  'main/queryobj.c',
#  'main/queryobj.h',
#  'main/rastpos.c',
#  'main/rastpos.h',
#  'main/readpix.c',
#  'main/readpix.h',
#  'main/remap.c',
#  'main/remap.h',
#  'main/renderbuffer.c',
#  'main/renderbuffer.h',
#  'main/robustness.c',
#  'main/samplerobj.c',
#  'main/samplerobj.h',
#  'main/scissor.c',
#  'main/scissor.h',
#  'main/shaderapi.c',
#  'main/shaderapi.h',
#  'main/shaderimage.c',
#  'main/shaderimage.h',
#  'main/shaderobj.c',
#  'main/shaderobj.h',
#  'main/shader_query.cpp',
#  'main/shared.c',
#  'main/shared.h',
#  'main/state.c',
#  'main/state.h',
#  'main/stencil.c',
#  'main/stencil.h',
#  'main/syncobj.c',
#  'main/syncobj.h',
#  'main/texcompress.c',
#  'main/texcompress_astc.cpp',
#  'main/texcompress_astc.h',
#  'main/texcompress_bptc.c',
#  'main/texcompress_bptc.h',
#  'main/texcompress_cpal.c',
#  'main/texcompress_cpal.h',
#  'main/texcompress_etc.c',
#  'main/texcompress_etc.h',
#  'main/texcompress_etc_tmp.h',
#  'main/texcompress_fxt1.c',
#  'main/texcompress_fxt1.h',
#  'main/texcompress.h',
#  'main/texcompress_rgtc.c',
#  'main/texcompress_rgtc.h',
#  'main/texcompress_s3tc.c',
#  'main/texcompress_s3tc.h',
#  'main/texenv.c',
#  'main/texenv.h',
#  'main/texenvprogram.h',
#  'main/texformat.c',
#  'main/texformat.h',
#  'main/texgen.c',
#  'main/texgen.h',
#  'main/texgetimage.c',
#  'main/texgetimage.h',
#  'main/teximage.c',
#  'main/teximage.h',
#  'main/texobj.c',
#  'main/texobj.h',
#  'main/texparam.c',
#  'main/texparam.h',
#  'main/texstate.c',
#  'main/texstate.h',
#  'main/texstorage.c',
#  'main/texstorage.h',
#  'main/texstore.c',
#  'main/texstore.h',
#  'main/texturebindless.c',
#  'main/texturebindless.h',
#  'main/textureview.c',
#  'main/textureview.h',
#  'main/transformfeedback.c',
#  'main/transformfeedback.h',
#  'main/uniform_query.cpp',
#  'main/uniforms.c',
#  'main/uniforms.h',
#  'main/varray.c',
#  'main/varray.h',
#  'main/vdpau.c',
#  'main/vdpau.h',
#  'main/version.c',
#  'main/version.h',
#  'main/viewport.c',
#  'main/viewport.h',
#  'main/vtxfmt.c',
#  'main/vtxfmt.h',
#  'main/es1_conversion.c',
#  'main/es1_conversion.h',
#  'math/m_debug.h',
#  'math/m_debug_clip.c',
#  'math/m_debug_norm.c',
#  'math/m_debug_util.h',
#  'math/m_debug_xform.c',
#  'math/m_eval.c',
#  'math/m_eval.h',
#  'math/m_matrix.c',
#  'math/m_matrix.h',
#  'math/m_trans_tmp.h',
#  'math/m_translate.c',
#  'math/m_translate.h',
#  'math/m_vector.c',
#  'math/m_vector.h',
#  'vbo/vbo_attrib.h',
#  'vbo/vbo_attrib_tmp.h',
#  'vbo/vbo_context.c',
#  'vbo/vbo_exec_api.c',
#  'vbo/vbo_exec.c',
#  'vbo/vbo_exec_draw.c',
#  'vbo/vbo_exec_eval.c',
#  'vbo/vbo_exec.h',
#  'vbo/vbo.h',
#  'vbo/vbo_minmax_index.c',
#  'vbo/vbo_noop.c',
#  'vbo/vbo_noop.h',
#  'vbo/vbo_primitive_restart.c',
#  'vbo/vbo_save_api.c',
#  'vbo/vbo_save.c',
#  'vbo/vbo_save_draw.c',
#  'vbo/vbo_save.h',
#  'vbo/vbo_save_loopback.c',
#  'x86/common_x86.c',
#)
#
## mesa files
#files_libmesa_classic = files(
#  'math/m_clip_tmp.h',
#  'math/m_copy_tmp.h',
#  'math/m_dotprod_tmp.h',
#  'math/m_norm_tmp.h',
#  'math/m_xform.c',
#  'math/m_xform.h',
#  'math/m_xform_tmp.h',
#  'tnl/t_context.c',
#  'tnl/t_context.h',
#  'tnl/t_draw.c',
#  'tnl/tnl.h',
#  'tnl/t_pipeline.c',
#  'tnl/t_pipeline.h',
#  'tnl/t_rebase.c',
#  'tnl/t_split.c',
#  'tnl/t_split_copy.c',
#  'tnl/t_split.h',
#  'tnl/t_split_inplace.c',
#  'tnl/t_vb_cliptmp.h',
#  'tnl/t_vb_fog.c',
#  'tnl/t_vb_light.c',
#  'tnl/t_vb_lighttmp.h',
#  'tnl/t_vb_normals.c',
#  'tnl/t_vb_points.c',
#  'tnl/t_vb_program.c',
#  'tnl/t_vb_render.c',
#  'tnl/t_vb_rendertmp.h',
#  'tnl/t_vb_texgen.c',
#  'tnl/t_vb_texmat.c',
#  'tnl/t_vb_vertex.c',
#  'tnl/t_vertex.c',
#  'tnl/t_vertex_generic.c',
#  'tnl/t_vertex.h',
#  'tnl/t_vertex_sse.c',
#  'tnl/t_vp_build.c',
#  'tnl/t_vp_build.h',
#  'swrast/s_aaline.c',
#  'swrast/s_aaline.h',
#  'swrast/s_aalinetemp.h',
#  'swrast/s_aatriangle.c',
#  'swrast/s_aatriangle.h',
#  'swrast/s_aatritemp.h',
#  'swrast/s_alpha.c',
#  'swrast/s_alpha.h',
#  'swrast/s_atifragshader.c',
#  'swrast/s_atifragshader.h',
#  'swrast/s_bitmap.c',
#  'swrast/s_blend.c',
#  'swrast/s_blend.h',
#  'swrast/s_blit.c',
#  'swrast/s_chan.h',
#  'swrast/s_clear.c',
#  'swrast/s_context.c',
#  'swrast/s_context.h',
#  'swrast/s_copypix.c',
#  'swrast/s_depth.c',
#  'swrast/s_depth.h',
#  'swrast/s_drawpix.c',
#  'swrast_setup/ss_tritmp.h',
#  'swrast_setup/ss_vb.h',
#  'swrast_setup/swrast_setup.h',
#  'swrast/s_feedback.c',
#  'swrast/s_feedback.h',
#  'swrast/s_fog.c',
#  'swrast/s_fog.h',
#  'swrast/s_fragprog.c',
#  'swrast/s_fragprog.h',
#  'swrast/s_lines.c',
#  'swrast/s_lines.h',
#  'swrast/s_linetemp.h',
#  'swrast/s_logic.c',
#  'swrast/s_logic.h',
#  'swrast/s_masking.c',
#  'swrast/s_masking.h',
#  'swrast/s_points.c',
#  'swrast/s_points.h',
#  'swrast/s_renderbuffer.c',
#  'swrast/s_renderbuffer.h',
#  'swrast/s_span.c',
#  'swrast/s_span.h',
#  'swrast/s_stencil.c',
#  'swrast/s_stencil.h',
#  'swrast/s_texcombine.c',
#  'swrast/s_texcombine.h',
#  'swrast/s_texfetch.c',
#  'swrast/s_texfetch.h',
#  'swrast/s_texfetch_tmp.h',
#  'swrast/s_texfilter.c',
#  'swrast/s_texfilter.h',
#  'swrast/s_texrender.c',
#  'swrast/s_texture.c',
#  'swrast/s_triangle.c',
#  'swrast/s_triangle.h',
#  'swrast/s_tritemp.h',
#  'swrast/swrast.h',
#  'swrast/s_zoom.c',
#  'swrast/s_zoom.h',
#  'swrast_setup/ss_context.c',
#  'swrast_setup/ss_context.h',
#  'swrast_setup/ss_triangle.c',
#  'swrast_setup/ss_triangle.h',
#  'drivers/common/driverfuncs.c',
#  'drivers/common/driverfuncs.h',
#  'drivers/common/meta_blit.c',
#  'drivers/common/meta_generate_mipmap.c',
#  'drivers/common/meta.c',
#  'drivers/common/meta.h',
#  'x86/x86_xform.c',
#  'x86/3dnow.c',
#  'x86/sse.c',
#  'x86/rtasm/x86sse.c',
#  'x86/rtasm/x86sse.h',
#  'sparc/sparc.c',
#  'x86-64/x86-64.c',
#)
#
#files_libmesa_gallium = files(
#  'state_tracker/st_atifs_to_tgsi.c',
#  'state_tracker/st_atifs_to_tgsi.h',
#  'state_tracker/st_atom_array.c',
#  'state_tracker/st_atom_atomicbuf.c',
#  'state_tracker/st_atom_blend.c',
#  'state_tracker/st_atom.c',
#  'state_tracker/st_atom_clip.c',
#  'state_tracker/st_atom_constbuf.c',
#  'state_tracker/st_atom_constbuf.h',
#  'state_tracker/st_atom_depth.c',
#  'state_tracker/st_atom_framebuffer.c',
#  'state_tracker/st_atom.h',
#  'state_tracker/st_atom_list.h',
#  'state_tracker/st_atom_image.c',
#  'state_tracker/st_atom_msaa.c',
#  'state_tracker/st_atom_pixeltransfer.c',
#  'state_tracker/st_atom_rasterizer.c',
#  'state_tracker/st_atom_sampler.c',
#  'state_tracker/st_atom_scissor.c',
#  'state_tracker/st_atom_shader.c',
#  'state_tracker/st_atom_stipple.c',
#  'state_tracker/st_atom_storagebuf.c',
#  'state_tracker/st_atom_tess.c',
#  'state_tracker/st_atom_texture.c',
#  'state_tracker/st_atom_viewport.c',
#  'state_tracker/st_cb_bitmap.c',
#  'state_tracker/st_cb_bitmap.h',
#  'state_tracker/st_cb_bitmap_shader.c',
#  'state_tracker/st_cb_blit.c',
#  'state_tracker/st_cb_blit.h',
#  'state_tracker/st_cb_bufferobjects.c',
#  'state_tracker/st_cb_bufferobjects.h',
#  'state_tracker/st_cb_clear.c',
#  'state_tracker/st_cb_clear.h',
#  'state_tracker/st_cb_compute.c',
#  'state_tracker/st_cb_compute.h',
#  'state_tracker/st_cb_condrender.c',
#  'state_tracker/st_cb_condrender.h',
#  'state_tracker/st_cb_copyimage.c',
#  'state_tracker/st_cb_copyimage.h',
#  'state_tracker/st_cb_drawpixels.c',
#  'state_tracker/st_cb_drawpixels.h',
#  'state_tracker/st_cb_drawpixels_shader.c',
#  'state_tracker/st_cb_drawtex.c',
#  'state_tracker/st_cb_drawtex.h',
#  'state_tracker/st_cb_eglimage.c',
#  'state_tracker/st_cb_eglimage.h',
#  'state_tracker/st_cb_fbo.c',
#  'state_tracker/st_cb_fbo.h',
#  'state_tracker/st_cb_feedback.c',
#  'state_tracker/st_cb_feedback.h',
#  'state_tracker/st_cb_flush.c',
#  'state_tracker/st_cb_flush.h',
#  'state_tracker/st_cb_memoryobjects.c',
#  'state_tracker/st_cb_memoryobjects.h',
#  'state_tracker/st_cb_msaa.c',
#  'state_tracker/st_cb_msaa.h',
#  'state_tracker/st_cb_perfmon.c',
#  'state_tracker/st_cb_perfmon.h',
#  'state_tracker/st_cb_program.c',
#  'state_tracker/st_cb_program.h',
#  'state_tracker/st_cb_queryobj.c',
#  'state_tracker/st_cb_queryobj.h',
#  'state_tracker/st_cb_rasterpos.c',
#  'state_tracker/st_cb_rasterpos.h',
#  'state_tracker/st_cb_readpixels.c',
#  'state_tracker/st_cb_readpixels.h',
#  'state_tracker/st_cb_strings.c',
#  'state_tracker/st_cb_strings.h',
#  'state_tracker/st_cb_semaphoreobjects.c',
#  'state_tracker/st_cb_semaphoreobjects.h',
#  'state_tracker/st_cb_syncobj.c',
#  'state_tracker/st_cb_syncobj.h',
#  'state_tracker/st_cb_texturebarrier.c',
#  'state_tracker/st_cb_texturebarrier.h',
#  'state_tracker/st_cb_texture.c',
#  'state_tracker/st_cb_texture.h',
#  'state_tracker/st_cb_viewport.c',
#  'state_tracker/st_cb_viewport.h',
#  'state_tracker/st_cb_xformfb.c',
#  'state_tracker/st_cb_xformfb.h',
#  'state_tracker/st_context.c',
#  'state_tracker/st_context.h',
#  'state_tracker/st_copytex.c',
#  'state_tracker/st_copytex.h',
#  'state_tracker/st_debug.c',
#  'state_tracker/st_debug.h',
#  'state_tracker/st_draw.c',
#  'state_tracker/st_draw_feedback.c',
#  'state_tracker/st_draw.h',
#  'state_tracker/st_extensions.c',
#  'state_tracker/st_extensions.h',
#  'state_tracker/st_format.c',
#  'state_tracker/st_format.h',
#  'state_tracker/st_gen_mipmap.c',
#  'state_tracker/st_gen_mipmap.h',
#  'state_tracker/st_gl_api.h',
#  'state_tracker/st_glsl_to_ir.cpp',
#  'state_tracker/st_glsl_to_ir.h',
#  'state_tracker/st_glsl_to_nir.cpp',
#  'state_tracker/st_glsl_to_tgsi.cpp',
#  'state_tracker/st_glsl_to_tgsi.h',
#  'state_tracker/st_glsl_to_tgsi_array_merge.cpp',
#  'state_tracker/st_glsl_to_tgsi_array_merge.h',
#  'state_tracker/st_glsl_to_tgsi_private.cpp',
#  'state_tracker/st_glsl_to_tgsi_private.h',
#  'state_tracker/st_glsl_to_tgsi_temprename.cpp',
#  'state_tracker/st_glsl_to_tgsi_temprename.h',
#  'state_tracker/st_glsl_types.cpp',
#  'state_tracker/st_glsl_types.h',
#  'state_tracker/st_manager.c',
#  'state_tracker/st_manager.h',
#  'state_tracker/st_mesa_to_tgsi.c',
#  'state_tracker/st_mesa_to_tgsi.h',
#  'state_tracker/st_nir.h',
#  'state_tracker/st_nir_builtins.c',
#  'state_tracker/st_nir_lower_builtin.c',
#  'state_tracker/st_nir_lower_tex_src_plane.c',
#  'state_tracker/st_pbo.c',
#  'state_tracker/st_pbo.h',
#  'state_tracker/st_program.c',
#  'state_tracker/st_program.h',
#  'state_tracker/st_sampler_view.c',
#  'state_tracker/st_sampler_view.h',
#  'state_tracker/st_scissor.c',
#  'state_tracker/st_scissor.h',
#  'state_tracker/st_shader_cache.c',
#  'state_tracker/st_shader_cache.h',
#  'state_tracker/st_texture.c',
#  'state_tracker/st_texture.h',
#  'state_tracker/st_tgsi_lower_yuv.c',
#  'state_tracker/st_tgsi_lower_yuv.h',
#  'state_tracker/st_util.h',
#  'state_tracker/st_vdpau.c',
#  'state_tracker/st_vdpau.h',
#)
#
#matypes_h = []
#if with_asm_arch == 'x86' or with_asm_arch == 'x86_64'
#  gen_matypes = executable(
#    'gen_matypes',
#    'x86/gen_matypes.c',
#    c_args : [c_vis_args, c_msvc_compat_args],
#    include_directories : inc_common,
#  )
#  matypes_h = custom_target(
#    'matypes.h',
#    output : 'matypes.h',
#    command : [gen_matypes],
#    capture : true,
#  )
#endif
#
#inc_libmesa_asm = []
#if with_asm_arch == 'x86'
#  files_libmesa_common += files(
#    'x86/assyntax.h',
#    'x86/clip_args.h',
#    'x86/norm_args.h',
#    'x86/xform_args.h',
#    'x86/common_x86_asm.S',
#    'x86/common_x86_asm.h',
#    'x86/common_x86_features.h',
#    'x86/x86_xform.h',
#    'x86/x86_xform2.S',
#    'x86/x86_xform3.S',
#    'x86/x86_xform4.S',
#    'x86/x86_cliptest.S',
#    'x86/mmx.h',
#    'x86/mmx_blend.S',
#    'x86/mmx_blendtmp.h',
#    'x86/3dnow.h',
#    'x86/3dnow_xform1.S',
#    'x86/3dnow_xform2.S',
#    'x86/3dnow_xform3.S',
#    'x86/3dnow_xform4.S',
#    'x86/sse.h',
#    'x86/sse_xform1.S',
#    'x86/sse_xform2.S',
#    'x86/sse_xform3.S',
#    'x86/sse_xform4.S',
#    'x86/sse_normal.S',
#    'x86/read_rgba_span_x86.S',
#  )
#  inc_libmesa_asm = include_directories('x86')
#elif with_asm_arch == 'x86_64'
#  files_libmesa_common += files('x86-64/x86-64.h', 'x86-64/xform4.S')
#  inc_libmesa_asm = include_directories('x86-64')
#elif with_asm_arch == 'sparc'
#  files_libmesa_common += files(
#    'sparc/sparc_clip.S',
#    'sparc/norm.S',
#    'sparc/xform.S',
#  )
#  inc_libmesa_asm = include_directories('sparc')
#endif
#
#format_fallback_c = custom_target(
#  'format_fallback.c',
#  input : ['main/format_fallback.py', 'main/formats.csv'],
#  output : 'format_fallback.c',
#  command : [prog_python, '@INPUT0@', '@INPUT1@', '@OUTPUT@'],
#  depend_files : files('main/format_parser.py'),
#)
#
#get_hash_h = custom_target(
#  'get_hash.h',
#  input : ['main/get_hash_generator.py', gl_and_es_api_files],
#  output : 'get_hash.h',
#  command : [prog_python, '@INPUT0@', '-f', '@INPUT1@'],
#  depend_files : files('main/get_hash_params.py'),
#  capture : true,
#)
#
#foreach x : [['format_info.h', 'format_info.py'],
#             ['format_pack.c', 'format_pack.py'],
#             ['format_unpack.c', 'format_unpack.py']]
#  files_libmesa_common += custom_target(
#    x[0],
#    input : ['main/@0@'.format(x[1]), 'main/formats.csv'],
#    output : x[0],
#    command : [prog_python, '@INPUT0@', '@INPUT1@'],
#    depend_files : files('main/format_parser.py'),
#    capture : true,
#  )
#endforeach
#
#files_libmesa_common += [
#  mesa_lex,
#  program_parse_tab,
#  main_api_exec_c,
#  main_enums_c,
#  format_fallback_c,
#  get_hash_h,
#  main_marshal_generated_c,
#  main_marshal_generated_h,
#  main_dispatch_h,
#  ir_expression_operation_h,
#  main_remap_helper_h,
#  matypes_h,
#  sha1_h,
#]
#
#if with_sse41
#  libmesa_sse41 = static_library(
#    'mesa_sse41',
#    files('main/streaming-load-memcpy.c', 'main/sse_minmax.c'),
#    c_args : [c_vis_args, c_msvc_compat_args, sse41_args],
#    include_directories : inc_common,
#  )
#else
#  libmesa_sse41 = []
#endif
#
#libmesa_classic = static_library(
#  'mesa_classic',
#  [files_libmesa_common, files_libmesa_classic],
#  c_args : [c_vis_args, c_msvc_compat_args],
#  cpp_args : [cpp_vis_args, cpp_msvc_compat_args],
#  include_directories : [inc_common, inc_libmesa_asm, include_directories('main')],
#  link_with : [libglsl, libmesa_sse41],
#  dependencies : idep_nir_headers,
#  build_by_default : false,
#)
#
#libmesa_gallium = static_library(
#  'mesa_gallium',
#  [files_libmesa_common, files_libmesa_gallium],
#  c_args : [c_vis_args, c_msvc_compat_args],
#  cpp_args : [cpp_vis_args, cpp_msvc_compat_args],
#  include_directories : [inc_common, inc_libmesa_asm, include_directories('main')],
#  link_with : [libglsl, libmesa_sse41],
#  dependencies : [idep_nir_headers, dep_vdpau],
#  build_by_default : false,
#)
#
#subdir('drivers/dri')
#if with_osmesa == 'classic'
#  subdir('drivers/osmesa')
#endif
#if with_glx == 'xlib'
#  subdir('drivers/x11')
#endif
#if with_tests and dri_drivers != []
#  subdir('main/tests')
#endif
#
