/*
 * Mesa 3-D graphics library
 *
 * Copyright (C) 1999-2008  Brian Paul   All Rights Reserved.
 * Copyright (c) 2009 VMware, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */


#ifndef TEXGETIMAGE_H
#define TEXGETIMAGE_H

#include "glheader.h"

struct gl_context;
struct gl_texture_image;
struct gl_texture_object;

extern GLenum
_mesa_base_pack_format(GLenum format);

extern void
_mesa_GetTexSubImage_sw(struct gl_context *ctx,
                        GLint xoffset, GLint yoffset, GLint zoffset,
                        GLsizei width, GLsizei height, GLint depth,
                        GLenum format, GLenum type, GLvoid *pixels,
                        struct gl_texture_image *texImage);

extern void
_mesa_get_compressed_texture_image( struct gl_context *ctx,
                                    struct gl_texture_object *texObj,
                                    struct gl_texture_image *texImage,
                                    GLenum target, GLint level,
                                    GLsizei bufSize, GLvoid *pixels,
                                    bool dsa );


extern void GLAPIENTRY
_mesa_GetTexImage( GLenum target, GLint level,
                   GLenum format, GLenum type, GLvoid *pixels );
extern void GLAPIENTRY
_mesa_GetnTexImageARB( GLenum target, GLint level, GLenum format,
                       GLenum type, GLsizei bufSize, GLvoid *pixels );
extern void GLAPIENTRY
_mesa_GetTextureImage(GLuint texture, GLint level, GLenum format,
                      GLenum type, GLsizei bufSize, GLvoid *pixels);
extern void GLAPIENTRY
_mesa_GetTextureImageEXT( GLuint texture, GLenum target, GLint level,
                          GLenum format, GLenum type, GLvoid *pixels);

extern void GLAPIENTRY
_mesa_GetTextureSubImage(GLuint texture, GLint level,
                         GLint xoffset, GLint yoffset, GLint zoffset,
                         GLsizei width, GLsizei height, GLsizei depth,
                         GLenum format, GLenum type, GLsizei bufSize,
                         void *pixels);


extern void GLAPIENTRY
_mesa_GetCompressedTexImage(GLenum target, GLint lod, GLvoid *img);

extern void GLAPIENTRY
_mesa_GetnCompressedTexImageARB(GLenum target, GLint level, GLsizei bufSize,
                                GLvoid *img);

extern void GLAPIENTRY
_mesa_GetCompressedTextureImage(GLuint texture, GLint level, GLsizei bufSize,
                                GLvoid *pixels);

extern void APIENTRY
_mesa_GetCompressedTextureSubImage(GLuint texture, GLint level,
                                   GLint xoffset, GLint yoffset,
                                   GLint zoffset, GLsizei width,
                                   GLsizei height, GLsizei depth,
                                   GLsizei bufSize, void *pixels);

#endif /* TEXGETIMAGE_H */
