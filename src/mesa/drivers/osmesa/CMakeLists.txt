# Copyright © 2017 Dylan Baker

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

project(mesa_src_mesa_drivers_osmesa)

cmake_minimum_required(VERSION 3.4)

add_library(libosmesa STATIC
${mesa_src_mesa_drivers_osmesa_SOURCE_DIR}/osmesa.c
${mesa_src_SOURCE_DIR}/mapi/glapi/gen/glapitable.h
${mesa_src_SOURCE_DIR}/mapi/glapi/gen/glapitemp.h
)
target_link_libraries(libosmesa libmesa_classic libglapi_static)

#osmesa_link_with = []
#
#if with_shared_glapi
#  osmesa_link_with += libglapi
#endif
#
#libosmesa = shared_library(
#  osmesa_lib_name,
#  ['osmesa.c', glapitable_h, glapitemp_h],
#  c_args : c_vis_args,
#  cpp_args : cpp_vis_args,
#  link_args : ld_args_gc_sections,
#  include_directories : [
#    inc_include, inc_src, inc_mapi, inc_mesa, inc_gallium, inc_gallium_aux,
#  ],
#  link_whole : libglapi_static,
#  link_with : [libmesa_classic, osmesa_link_with],
#  dependencies : [dep_thread, dep_selinux],
#  version : '8.0.0',
#  install : true,
#)
#
#pkg.generate(
#  name : 'osmesa',
#  description : 'Mesa Off-screen Rendering Library',
#  version : '8.0.0',
#  libraries : libosmesa,
#  libraries_private : gl_priv_libs,
#)
#
