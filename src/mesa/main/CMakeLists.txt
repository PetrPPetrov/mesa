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

project(mesa_src_mesa_main)

cmake_minimum_required(VERSION 3.4)

execute_process(
COMMAND ${PYTHON_EXECUTABLE} ${mesa_src_mesa_main_SOURCE_DIR}/../../mapi/glapi/gen/gl_table.py -f ${gl_and_es_api_files} -m remap_table
OUTPUT_FILE ${mesa_src_mesa_main_SOURCE_DIR}/dispatch.h
WORKING_DIRECTORY ${mesa_src_mesa_main_SOURCE_DIR}
)

execute_process(
COMMAND ${PYTHON_EXECUTABLE} ${mesa_src_mesa_main_SOURCE_DIR}/../../mapi/glapi/gen/gl_marshal_h.py -f ${gl_and_es_api_files}
OUTPUT_FILE ${mesa_src_mesa_main_SOURCE_DIR}/marshal_generated.h
WORKING_DIRECTORY ${mesa_src_mesa_main_SOURCE_DIR}
)

execute_process(
COMMAND ${PYTHON_EXECUTABLE} ${mesa_src_mesa_main_SOURCE_DIR}/../../mapi/glapi/gen/remap_helper.py -f ${gl_and_es_api_files}
OUTPUT_FILE ${mesa_src_mesa_main_SOURCE_DIR}/remap_helper.h
WORKING_DIRECTORY ${mesa_src_mesa_main_SOURCE_DIR}
)

#main_dispatch_h = custom_target(
#  'dispatch.h',
#  input : [files('../../mapi/glapi/gen/gl_table.py'), gl_and_es_api_files],
#  output : 'dispatch.h',
#  command : [prog_python, '@INPUT0@', '-f', '@INPUT1@', '-m', 'remap_table'],
#  depend_files : glapi_gen_depends,
#  capture : true,
#)
#
#main_marshal_generated_h = custom_target(
#  'marshal_generated.h',
#  input : [files('../../mapi/glapi/gen/gl_marshal_h.py'), gl_and_es_api_files],
#  output : 'marshal_generated.h',
#  command : [prog_python, '@INPUT0@', '-f', '@INPUT1@'],
#  depend_files : files('../../mapi/glapi/gen/marshal_XML.py') + glapi_gen_depends,
#  capture : true,
#)
#
#main_remap_helper_h = custom_target(
#  'remap_helper.h',
#  input : [files('../../mapi/glapi/gen/remap_helper.py'), gl_and_es_api_files],
#  output : 'remap_helper.h',
#  command : [prog_python, '@INPUT0@', '-f', '@INPUT1@'],
#  depend_files : glapi_gen_depends,
#  capture : true,
#)
#