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

project(mesa_src_mapi_glapi)

cmake_minimum_required(VERSION 3.4)

add_subdirectory(gen)

include_directories(
${mesa_src_mapi_glapi_SOURCE_DIR}
${mesa_src_mapi_glapi_SOURCE_DIR}/gen
)

set(static_glapi_files
# No DRI platform
# ${mesa_src_mapi_glapi_SOURCE_DIR}/gen/glapi_gentable.c
# ${mesa_src_mapi_glapi_SOURCE_DIR}/gen/glapitable.h
)

if(FALSE) # We use static version of glapi - libglapi_static
  set(static_glapi_files
  ${static_glapi_files}
  ${mesa_src_mapi_glapi_SOURCE_DIR}/../mapi_glapi.c
  ${mesa_src_mapi_glapi_SOURCE_DIR}/../entry.c
  ${mesa_src_mapi_glapi_SOURCE_DIR}/../entry.h
  ${mesa_src_mapi_glapi_SOURCE_DIR}/../entry_x86-64_tls.h
  ${mesa_src_mapi_glapi_SOURCE_DIR}/../entry_x86_tls.h
  ${mesa_src_mapi_glapi_SOURCE_DIR}/../entry_x86_tsd.h
  ${mesa_src_mapi_glapi_SOURCE_DIR}/../entry_ppc64le_tls.h
  ${mesa_src_mapi_glapi_SOURCE_DIR}/../entry_ppc64le_tsd.h
  ${mesa_src_mapi_glapi_SOURCE_DIR}/../mapi_tmp.h
  ${mesa_src_mapi_glapi_SOURCE_DIR}/gen/glapi_mapi_tmp.h
  )
  add_definitions(-DMAPI_MODE_BRIDGE)
  # add_definitions(-DMAPI_ABI_HEADER="${mesa_src_mapi_glapi_SOURCE_DIR}/gen/glapi_mapi_tmp.h")
  add_definitions(-DMAPI_ABI_HEADER="${mesa_src_mapi_SOURCE_DIR}/shared-glapi/glapi_mapi_tmp.h")
else()
  set(static_glapi_files # Static version
  ${static_glapi_files}
  ${mesa_src_mapi_glapi_SOURCE_DIR}/glapi_dispatch.c
  ${mesa_src_mapi_glapi_SOURCE_DIR}/glapi_entrypoint.c
  ${mesa_src_mapi_glapi_SOURCE_DIR}/glapi_getproc.c
  ${mesa_src_mapi_glapi_SOURCE_DIR}/glapi_nop.c
  ${mesa_src_mapi_glapi_SOURCE_DIR}/glapi.c
  ${mesa_src_mapi_glapi_SOURCE_DIR}/glapi.h
  ${mesa_src_mapi_glapi_SOURCE_DIR}/glapi_priv.h
  ${files_mapi_util}
  ${mesa_src_mapi_glapi_SOURCE_DIR}/gen/glapitable.h
  ${mesa_src_mapi_glapi_SOURCE_DIR}/gen/glapi_mapi_tmp.h
  ${mesa_src_mapi_glapi_SOURCE_DIR}/gen/glprocs.h
  ${mesa_src_mapi_glapi_SOURCE_DIR}/gen/glapitemp.h
  )
  add_definitions(-DMAPI_MODE_UTIL)
  # add_definitions(-DMAPI_ABI_HEADER="${mesa_src_mapi_glapi_SOURCE_DIR}/gen/glapi_mapi_tmp.h")
endif()

add_library(libglapi_static STATIC
${static_glapi_files}
)

#subdir('gen')
#
#inc_glapi = include_directories('.')
#
#static_glapi_files = []
#static_glapi_args = []
#
#if ['apple', 'windows'].contains(with_dri_platform)
#  static_glapi_files += [glapi_gentable_c, glapitable_h]
#endif
#
#if with_shared_glapi
#  static_glapi_files += files(
#    '../entry.c',
#    '../entry.h',
#    '../entry_x86-64_tls.h',
#    '../entry_x86_tls.h',
#    '../entry_x86_tsd.h',
#    '../entry_ppc64le_tls.h',
#    '../entry_ppc64le_tsd.h',
#    '../mapi_tmp.h',
#  )
#  static_glapi_files += glapi_mapi_tmp_h
#  static_glapi_args += [
#    '-DMAPI_MODE_BRIDGE',
#    '-DMAPI_ABI_HEADER="@0@"'.format(glapi_mapi_tmp_h.full_path()),
#    gcc_lto_quirk,
#  ]
#else
#  static_glapi_args += '-DMAPI_MODE_UTIL'
#  static_glapi_files += files(
#    'glapi_dispatch.c',
#    'glapi_entrypoint.c',
#    'glapi_getproc.c',
#    'glapi_nop.c',
#    'glapi.c',
#    'glapi.h',
#    'glapi_priv.h',
#  )
#  static_glapi_files += files_mapi_util
#  static_glapi_files += [
#    glapitable_h, glapi_mapi_tmp_h, glprocs_h, glapitemp_h,
#  ]
#  if with_asm_arch == 'x86'
#    static_glapi_files += glapi_x86_s
#  elif with_asm_arch == 'x86_64'
#    static_glapi_files += glapi_x86_64_s
#  elif with_asm_arch == 'sparc'
#    static_glapi_files += glapi_sparc_s
#  endif
#endif
#
#libglapi_static = static_library(
#  'glapi_static',
#  static_glapi_files,
#  include_directories : [inc_mesa, inc_include, inc_src, inc_mapi],
#  c_args : [c_msvc_compat_args, static_glapi_args],
#  dependencies : [dep_thread, dep_selinux],
#  build_by_default : false,
#)
#
#if with_any_opengl and not with_shared_glapi and with_tests
#  test(
#    'glapi_static_check_table',
#    executable(
#      'glapi_static_check_table',
#      ['tests/check_table.cpp', glapitable_h],
#      include_directories : [inc_include, inc_src, inc_mesa, inc_mapi],
#      link_with : [libglapi_static],
#      dependencies : [idep_gtest, dep_thread],
#    ),
#    suite : ['mapi'],
#  )
#endif
#
