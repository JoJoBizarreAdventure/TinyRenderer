#ifndef TINY_RENDERER_RENDER_H
#define TINY_RENDERER_RENDER_H

#include "luaHelper.h"
#include "utils.h"
#include "model.h"

// tgaImage
LUA_WRAPPER_HEADER(tgaImage_create)

LUA_WRAPPER_HEADER(tgaImage_open)

#define TGA_IMAGE_LIB_HEADER(FUNC_NAME) LUA_WRAPPER_HEADER(FUNC_NAME)

TGA_IMAGE_LIB_HEADER(tgaImage_save)

TGA_IMAGE_LIB_HEADER(tgaImage_close)

TGA_IMAGE_LIB_HEADER(tgaImage_getColorAt)

TGA_IMAGE_LIB_HEADER(tgaImage_getZBufferAt)

TGA_IMAGE_LIB_HEADER(tgaImage_setPixel)

// model
LUA_WRAPPER_HEADER(model_load)

#define MODEL_LIB_HEADER(FUNC_NAME) LUA_WRAPPER_HEADER(FUNC_NAME)

MODEL_LIB_HEADER(model_unload)

MODEL_LIB_HEADER(model_faces)

MODEL_LIB_HEADER(model_vert)

MODEL_LIB_HEADER(model_uvIndexes)

MODEL_LIB_HEADER(model_uv)

MODEL_LIB_HEADER(model_normalIndexes)

MODEL_LIB_HEADER(model_normal)

// utils
TGA_IMAGE_LIB_HEADER(utils_line)

TGA_IMAGE_LIB_HEADER(utils_triangle_l2)

TGA_IMAGE_LIB_HEADER(utils_triangle_l3)


#endif //TINY_RENDERER_RENDER_H
