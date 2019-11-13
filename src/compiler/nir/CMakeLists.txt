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

project(mesa_src_compiler_nir)

cmake_minimum_required(VERSION 3.4)

execute_process(
COMMAND ${PYTHON_EXECUTABLE} ${mesa_src_compiler_nir_SOURCE_DIR}/nir_builder_opcodes_h.py
OUTPUT_FILE ${mesa_src_compiler_nir_SOURCE_DIR}/nir_builder_opcodes.h
WORKING_DIRECTORY ${mesa_src_compiler_nir_SOURCE_DIR}
)

execute_process(
COMMAND ${PYTHON_EXECUTABLE} ${mesa_src_compiler_nir_SOURCE_DIR}/nir_constant_expressions.py
OUTPUT_FILE ${mesa_src_compiler_nir_SOURCE_DIR}/nir_constant_expressions.c
WORKING_DIRECTORY ${mesa_src_compiler_nir_SOURCE_DIR}
)

execute_process(
COMMAND ${PYTHON_EXECUTABLE} ${mesa_src_compiler_nir_SOURCE_DIR}/nir_opcodes_h.py
OUTPUT_FILE ${mesa_src_compiler_nir_SOURCE_DIR}/nir_opcodes.h
WORKING_DIRECTORY ${mesa_src_compiler_nir_SOURCE_DIR}
)

execute_process(
COMMAND ${PYTHON_EXECUTABLE} ${mesa_src_compiler_nir_SOURCE_DIR}/nir_opcodes_c.py
OUTPUT_FILE ${mesa_src_compiler_nir_SOURCE_DIR}/nir_opcodes.c
WORKING_DIRECTORY ${mesa_src_compiler_nir_SOURCE_DIR}
)

execute_process(
COMMAND ${PYTHON_EXECUTABLE} ${mesa_src_compiler_nir_SOURCE_DIR}/nir_opt_algebraic.py
OUTPUT_FILE ${mesa_src_compiler_nir_SOURCE_DIR}/nir_opt_algebraic.c
WORKING_DIRECTORY ${mesa_src_compiler_nir_SOURCE_DIR}
)

execute_process(
COMMAND ${PYTHON_EXECUTABLE} ${mesa_src_compiler_nir_SOURCE_DIR}/nir_intrinsics_h.py --outdir ${CMAKE_CURRENT_LIST_DIR}
WORKING_DIRECTORY ${mesa_src_compiler_nir_SOURCE_DIR}
)

execute_process(
COMMAND ${PYTHON_EXECUTABLE} ${mesa_src_compiler_nir_SOURCE_DIR}/nir_intrinsics_c.py --outdir ${CMAKE_CURRENT_LIST_DIR}
WORKING_DIRECTORY ${mesa_src_compiler_nir_SOURCE_DIR}
)

set(files_libnir
${mesa_src_compiler_nir_SOURCE_DIR}/nir.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir.h
${mesa_src_compiler_nir_SOURCE_DIR}/nir_builder.h
${mesa_src_compiler_nir_SOURCE_DIR}/nir_builtin_builder.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_builtin_builder.h
${mesa_src_compiler_nir_SOURCE_DIR}/nir_clone.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_constant_expressions.h
${mesa_src_compiler_nir_SOURCE_DIR}/nir_control_flow.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_control_flow.h
${mesa_src_compiler_nir_SOURCE_DIR}/nir_control_flow_private.h
${mesa_src_compiler_nir_SOURCE_DIR}/nir_deref.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_deref.h
${mesa_src_compiler_nir_SOURCE_DIR}/nir_dominance.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_format_convert.h
${mesa_src_compiler_nir_SOURCE_DIR}/nir_from_ssa.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_gather_info.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_gather_ssa_types.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_gather_xfb_info.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_gs_count_vertices.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_inline_functions.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_instr_set.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_instr_set.h
${mesa_src_compiler_nir_SOURCE_DIR}/nir_linking_helpers.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_liveness.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_loop_analyze.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_loop_analyze.h
${mesa_src_compiler_nir_SOURCE_DIR}/nir_lower_alu.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_lower_alu_to_scalar.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_lower_alpha_test.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_lower_array_deref_of_vec.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_lower_atomics_to_ssbo.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_lower_bitmap.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_lower_bool_to_float.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_lower_bool_to_int32.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_lower_clamp_color_outputs.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_lower_clip.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_lower_clip_cull_distance_arrays.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_lower_constant_initializers.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_lower_double_ops.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_lower_drawpixels.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_lower_fb_read.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_lower_fragcoord_wtrans.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_lower_frexp.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_lower_global_vars_to_local.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_lower_gs_intrinsics.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_lower_load_const_to_scalar.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_lower_locals_to_regs.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_lower_idiv.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_lower_indirect_derefs.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_lower_int64.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_lower_io.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_lower_io_arrays_to_elements.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_lower_io_to_temporaries.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_lower_io_to_scalar.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_lower_io_to_vector.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_lower_non_uniform_access.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_lower_packing.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_lower_passthrough_edgeflags.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_lower_patch_vertices.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_lower_phis_to_scalar.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_lower_regs_to_ssa.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_lower_returns.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_lower_scratch.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_lower_subgroups.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_lower_system_values.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_lower_tex.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_lower_to_source_mods.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_lower_two_sided_color.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_lower_vars_to_ssa.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_lower_var_copies.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_lower_vec_to_movs.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_lower_viewport_transform.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_lower_wpos_center.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_lower_wpos_ytransform.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_lower_bit_size.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_lower_uniforms_to_ubo.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_metadata.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_move_load_const.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_move_vec_src_uses_to_dest.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_normalize_cubemap_coords.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_opt_combine_stores.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_opt_comparison_pre.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_opt_conditional_discard.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_opt_constant_folding.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_opt_copy_prop_vars.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_opt_copy_propagate.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_opt_cse.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_opt_dce.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_opt_dead_cf.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_opt_dead_write_vars.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_opt_find_array_copies.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_opt_gcm.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_opt_idiv_const.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_opt_if.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_opt_intrinsics.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_opt_large_constants.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_opt_loop_unroll.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_opt_move_comparisons.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_opt_move_load_ubo.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_opt_peephole_select.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_opt_remove_phis.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_opt_shrink_load.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_opt_trivial_continues.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_opt_undef.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_phi_builder.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_phi_builder.h
${mesa_src_compiler_nir_SOURCE_DIR}/nir_print.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_propagate_invariant.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_remove_dead_variables.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_repair_ssa.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_search.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_search.h
${mesa_src_compiler_nir_SOURCE_DIR}/nir_search_helpers.h
${mesa_src_compiler_nir_SOURCE_DIR}/nir_serialize.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_serialize.h
${mesa_src_compiler_nir_SOURCE_DIR}/nir_strip.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_split_per_member_structs.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_split_var_copies.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_split_vars.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_sweep.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_to_lcssa.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_validate.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_vla.h
${mesa_src_compiler_nir_SOURCE_DIR}/nir_worklist.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_worklist.h
${mesa_src_compiler_nir_SOURCE_DIR}/nir_xfb_info.h
${mesa_src_compiler_nir_SOURCE_DIR}/../spirv/GLSL.ext.AMD.h
${mesa_src_compiler_nir_SOURCE_DIR}/../spirv/GLSL.std.450.h
${mesa_src_compiler_nir_SOURCE_DIR}/../spirv/gl_spirv.c
${mesa_src_compiler_nir_SOURCE_DIR}/../spirv/nir_spirv.h
${mesa_src_compiler_nir_SOURCE_DIR}/../spirv/OpenCL.std.h
${mesa_src_compiler_nir_SOURCE_DIR}/../spirv/spirv.h
${mesa_src_compiler_nir_SOURCE_DIR}/../spirv/spirv_info.h
${mesa_src_compiler_nir_SOURCE_DIR}/../spirv/spirv_to_nir.c
${mesa_src_compiler_nir_SOURCE_DIR}/../spirv/vtn_alu.c
${mesa_src_compiler_nir_SOURCE_DIR}/../spirv/vtn_amd.c
${mesa_src_compiler_nir_SOURCE_DIR}/../spirv/vtn_cfg.c
${mesa_src_compiler_nir_SOURCE_DIR}/../spirv/vtn_glsl450.c
${mesa_src_compiler_nir_SOURCE_DIR}/../spirv/vtn_opencl.c
${mesa_src_compiler_nir_SOURCE_DIR}/../spirv/vtn_private.h
${mesa_src_compiler_nir_SOURCE_DIR}/../spirv/vtn_subgroup.c
${mesa_src_compiler_nir_SOURCE_DIR}/../spirv/vtn_variables.c
)

add_library(libmesacompiler STATIC
${files_libcompiler}
${mesa_src_compiler_SOURCE_DIR}/ir_expression_operation.h
)

vtsl_set_crt(libmesacompiler)

include_directories(${mesa_src_compiler_SOURCE_DIR}/spirv)
include_directories(${mesa_src_compiler_nir_SOURCE_DIR})
add_library(nir STATIC
${files_libnir}
${mesa_src_compiler_spirv_SOURCE_DIR}/spirv_info.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_opt_algebraic.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_opcodes.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_opcodes.h
${mesa_src_compiler_nir_SOURCE_DIR}/nir_constant_expressions.c
${mesa_src_compiler_nir_SOURCE_DIR}/nir_builder_opcodes.h
nir_intrinsics.c
nir_intrinsics.h
../spirv/vtn_gather_types.c
)
target_link_libraries(nir libmesacompiler)

vtsl_set_crt(nir)

#nir_depends = files('nir_opcodes.py', 'nir_intrinsics.py')
#
#nir_builder_opcodes_h = custom_target(
#  'nir_builder_opcodes.h',
#  input : 'nir_builder_opcodes_h.py',
#  output : 'nir_builder_opcodes.h',
#  command : [prog_python, '@INPUT@'],
#  capture : true,
#  depend_files : nir_depends,
#)
#
#nir_constant_expressions_c = custom_target(
#  'nir_constant_expressions.c',
#  input : 'nir_constant_expressions.py',
#  output : 'nir_constant_expressions.c',
#  command : [prog_python, '@INPUT@'],
#  capture : true,
#  depend_files : nir_depends,
#)
#
#nir_opcodes_h = custom_target(
#  'nir_opcodes.h',
#  input : 'nir_opcodes_h.py',
#  output : 'nir_opcodes.h',
#  command : [prog_python, '@INPUT@'],
#  capture : true,
#  depend_files : nir_depends,
#)
#
#nir_opcodes_c = custom_target(
#  'nir_opcodes.c',
#  input : 'nir_opcodes_c.py',
#  output : 'nir_opcodes.c',
#  command : [prog_python, '@INPUT@'],
#  capture : true,
#  depend_files : nir_depends,
#)
#
#nir_opt_algebraic_c = custom_target(
#  'nir_opt_algebraic.c',
#  input : 'nir_opt_algebraic.py',
#  output : 'nir_opt_algebraic.c',
#  command : [prog_python, '@INPUT@'],
#  capture : true,
#  depend_files : files('nir_algebraic.py'),
#)
#
#nir_intrinsics_h = custom_target(
#  'nir_intrinsics.h',
#  input : 'nir_intrinsics_h.py',
#  output : 'nir_intrinsics.h',
#  command : [prog_python, '@INPUT@', '--outdir', meson.current_build_dir()],
#  capture : false,
#  depend_files : files('nir_intrinsics.py'),
#)
#
#nir_intrinsics_c = custom_target(
#  'nir_intrinsic.c',
#  input : 'nir_intrinsics_c.py',
#  output : 'nir_intrinsics.c',
#  command : [prog_python, '@INPUT@', '--outdir', meson.current_build_dir()],
#  capture: false,
#  depend_files : files('nir_intrinsics.py'),
#)
#
#files_libnir = files(
#  'nir.c',
#  'nir.h',
#  'nir_builder.h',
#  'nir_builtin_builder.c',
#  'nir_builtin_builder.h',
#  'nir_clone.c',
#  'nir_constant_expressions.h',
#  'nir_control_flow.c',
#  'nir_control_flow.h',
#  'nir_control_flow_private.h',
#  'nir_deref.c',
#  'nir_deref.h',
#  'nir_dominance.c',
#  'nir_format_convert.h',
#  'nir_from_ssa.c',
#  'nir_gather_info.c',
#  'nir_gather_ssa_types.c',
#  'nir_gather_xfb_info.c',
#  'nir_gs_count_vertices.c',
#  'nir_inline_functions.c',
#  'nir_instr_set.c',
#  'nir_instr_set.h',
#  'nir_linking_helpers.c',
#  'nir_liveness.c',
#  'nir_loop_analyze.c',
#  'nir_loop_analyze.h',
#  'nir_lower_alu.c',
#  'nir_lower_alu_to_scalar.c',
#  'nir_lower_alpha_test.c',
#  'nir_lower_array_deref_of_vec.c',
#  'nir_lower_atomics_to_ssbo.c',
#  'nir_lower_bitmap.c',
#  'nir_lower_bool_to_float.c',
#  'nir_lower_bool_to_int32.c',
#  'nir_lower_clamp_color_outputs.c',
#  'nir_lower_clip.c',
#  'nir_lower_clip_cull_distance_arrays.c',
#  'nir_lower_constant_initializers.c',
#  'nir_lower_double_ops.c',
#  'nir_lower_drawpixels.c',
#  'nir_lower_fb_read.c',
#  'nir_lower_fragcoord_wtrans.c',
#  'nir_lower_frexp.c',
#  'nir_lower_global_vars_to_local.c',
#  'nir_lower_gs_intrinsics.c',
#  'nir_lower_load_const_to_scalar.c',
#  'nir_lower_locals_to_regs.c',
#  'nir_lower_idiv.c',
#  'nir_lower_indirect_derefs.c',
#  'nir_lower_int64.c',
#  'nir_lower_io.c',
#  'nir_lower_io_arrays_to_elements.c',
#  'nir_lower_io_to_temporaries.c',
#  'nir_lower_io_to_scalar.c',
#  'nir_lower_io_to_vector.c',
#  'nir_lower_non_uniform_access.c',
#  'nir_lower_packing.c',
#  'nir_lower_passthrough_edgeflags.c',
#  'nir_lower_patch_vertices.c',
#  'nir_lower_phis_to_scalar.c',
#  'nir_lower_regs_to_ssa.c',
#  'nir_lower_returns.c',
#  'nir_lower_scratch.c',
#  'nir_lower_subgroups.c',
#  'nir_lower_system_values.c',
#  'nir_lower_tex.c',
#  'nir_lower_to_source_mods.c',
#  'nir_lower_two_sided_color.c',
#  'nir_lower_vars_to_ssa.c',
#  'nir_lower_var_copies.c',
#  'nir_lower_vec_to_movs.c',
#  'nir_lower_viewport_transform.c',
#  'nir_lower_wpos_center.c',
#  'nir_lower_wpos_ytransform.c',
#  'nir_lower_bit_size.c',
#  'nir_lower_uniforms_to_ubo.c',
#  'nir_metadata.c',
#  'nir_move_load_const.c',
#  'nir_move_vec_src_uses_to_dest.c',
#  'nir_normalize_cubemap_coords.c',
#  'nir_opt_combine_stores.c',
#  'nir_opt_comparison_pre.c',
#  'nir_opt_conditional_discard.c',
#  'nir_opt_constant_folding.c',
#  'nir_opt_copy_prop_vars.c',
#  'nir_opt_copy_propagate.c',
#  'nir_opt_cse.c',
#  'nir_opt_dce.c',
#  'nir_opt_dead_cf.c',
#  'nir_opt_dead_write_vars.c',
#  'nir_opt_find_array_copies.c',
#  'nir_opt_gcm.c',
#  'nir_opt_idiv_const.c',
#  'nir_opt_if.c',
#  'nir_opt_intrinsics.c',
#  'nir_opt_large_constants.c',
#  'nir_opt_loop_unroll.c',
#  'nir_opt_move_comparisons.c',
#  'nir_opt_move_load_ubo.c',
#  'nir_opt_peephole_select.c',
#  'nir_opt_remove_phis.c',
#  'nir_opt_shrink_load.c',
#  'nir_opt_trivial_continues.c',
#  'nir_opt_undef.c',
#  'nir_phi_builder.c',
#  'nir_phi_builder.h',
#  'nir_print.c',
#  'nir_propagate_invariant.c',
#  'nir_remove_dead_variables.c',
#  'nir_repair_ssa.c',
#  'nir_search.c',
#  'nir_search.h',
#  'nir_search_helpers.h',
#  'nir_serialize.c',
#  'nir_serialize.h',
#  'nir_strip.c',
#  'nir_split_per_member_structs.c',
#  'nir_split_var_copies.c',
#  'nir_split_vars.c',
#  'nir_sweep.c',
#  'nir_to_lcssa.c',
#  'nir_validate.c',
#  'nir_vla.h',
#  'nir_worklist.c',
#  'nir_worklist.h',
#  'nir_xfb_info.h',
#  '../spirv/GLSL.ext.AMD.h',
#  '../spirv/GLSL.std.450.h',
#  '../spirv/gl_spirv.c',
#  '../spirv/nir_spirv.h',
#  '../spirv/OpenCL.std.h',
#  '../spirv/spirv.h',
#  '../spirv/spirv_info.h',
#  '../spirv/spirv_to_nir.c',
#  '../spirv/vtn_alu.c',
#  '../spirv/vtn_amd.c',
#  '../spirv/vtn_cfg.c',
#  '../spirv/vtn_glsl450.c',
#  '../spirv/vtn_opencl.c',
#  '../spirv/vtn_private.h',
#  '../spirv/vtn_subgroup.c',
#  '../spirv/vtn_variables.c',
#)
#
#libnir = static_library(
#  'nir',
#  [files_libnir, spirv_info_c, nir_opt_algebraic_c, nir_opcodes_c,
#   nir_opcodes_h, nir_constant_expressions_c, nir_builder_opcodes_h,
#   vtn_gather_types_c, nir_intrinsics_c, nir_intrinsics_h],
#  include_directories : [inc_common, inc_compiler, include_directories('../spirv')],
#  c_args : [c_vis_args, c_msvc_compat_args, no_override_init_args],
#  link_with : libcompiler,
#  build_by_default : false,
#)
#
## Headers-only dependency
#idep_nir_headers = declare_dependency(
#  sources : [nir_opcodes_h, nir_builder_opcodes_h, nir_intrinsics_h],
#  include_directories : include_directories('.'),
#)
#
## Also link with nir
#idep_nir = declare_dependency(
#  dependencies : idep_nir_headers,
#  link_with : libnir,
#)
#
#nir_algebraic_py = files('nir_algebraic.py')
#
#if with_tests
#  test(
#    'nir_control_flow',
#    executable(
#      'nir_control_flow_test',
#      files('tests/control_flow_tests.cpp'),
#      cpp_args : [cpp_vis_args, cpp_msvc_compat_args],
#      include_directories : [inc_common],
#      dependencies : [dep_thread, idep_gtest, idep_nir],
#      link_with : libmesa_util,
#    ), 
#    suite : ['compiler', 'nir'],
#  )
#
#  test(
#    'nir_vars',
#    executable(
#      'nir_vars_test',
#      files('tests/vars_tests.cpp'),
#      cpp_args : [cpp_vis_args, cpp_msvc_compat_args],
#      include_directories : [inc_common],
#      dependencies : [dep_thread, idep_gtest, idep_nir],
#      link_with : libmesa_util,
#    ),
#    suite : ['compiler', 'nir'],
#  )
#  test(
#    'nir_algebraic_parser',
#    prog_python,
#    args : [
#      join_paths(meson.current_source_dir(), 'tests/algebraic_parser_test.py')
#    ],
#    suite : ['compiler', 'nir'],
#  )
#
#  test(
#    'negative_equal',
#    executable(
#      'negative_equal',
#      files('tests/negative_equal_tests.cpp'),
#      c_args : [c_vis_args, c_msvc_compat_args, no_override_init_args],
#      include_directories : [inc_common],
#      dependencies : [dep_thread, idep_gtest, idep_nir],
#      link_with : libmesa_util,
#    )
#  )
#endif
#
