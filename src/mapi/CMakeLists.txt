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

project(mesa_src_mapi)

cmake_minimum_required(VERSION 3.4)

set(files_mapi_util
${mesa_src_mapi_SOURCE_DIR}/u_current.c
${mesa_src_mapi_SOURCE_DIR}/u_current.h
${mesa_src_mapi_SOURCE_DIR}/u_execmem.c
${mesa_src_mapi_SOURCE_DIR}/u_execmem.h
)

set(genCommon_py
${mesa_src_mapi_SOURCE_DIR}/new/genCommon.py
CACHE INTERNAL "genCommon_py"
)

set(glapi_gen_mapi_py
${mesa_src_mapi_SOURCE_DIR}/new/gen_gldispatch_mapi.py
CACHE INTERNAL "glapi_gen_mapi_py"
)

set(mapi_abi_py
${mesa_src_mapi_SOURCE_DIR}/mapi_abi.py
CACHE INTERNAL "mapi_abi_py"
)

# Static version only
# add_subdirectory(shared-glapi)
add_subdirectory(glapi)

#files_mapi_util = files(
#  'u_current.c',
#  'u_current.h',
#  'u_execmem.c',
#  'u_execmem.h',
#)
#
#genCommon_py = files('new/genCommon.py')
#glapi_gen_mapi_py = files('new/gen_gldispatch_mapi.py')
#mapi_abi_py = files('mapi_abi.py')
#
#subdir('glapi')
#if with_shared_glapi
#  subdir('shared-glapi')
#else
#  libglapi = []
#endif
#if not with_glvnd
#  if with_gles1
#    subdir('es1api')
#  endif
#  if with_gles2
#    subdir('es2api')
#  endif
#endif
#
