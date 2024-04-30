#ifndef TINY_RENDERER_MATRIX_HEADER_H
#define TINY_RENDERER_MATRIX_HEADER_H

#include "luaHelper.h"

enum MatrixType : int {
    Undefined = -1,
    Vector2i = 0,
    Vector3d = 1,
    Vector4d = 2,
    Vector2d = 3,
};

// vector
LUA_WRAPPER_HEADER(matrix_vector2i)

#define VECTOR2I_LIB_HEADER(FUNC_NAME) LUA_WRAPPER_HEADER(FUNC_NAME)

VECTOR2I_LIB_HEADER(vector2i_getX)

VECTOR2I_LIB_HEADER(vector2i_getY)

static const struct luaL_Reg vector2i_func[] = {
        {"x", vector2i_getX},
        {"y", vector2i_getY},
};

LUA_WRAPPER_HEADER(matrix_vector2d)

#define VECTOR2F_LIB_HEADER(FUNC_NAME) LUA_WRAPPER_HEADER(FUNC_NAME)

VECTOR2F_LIB_HEADER(vector2d_getX)

VECTOR2F_LIB_HEADER(vector2d_getY)

VECTOR2F_LIB_HEADER(vector2d_norm)

static const struct luaL_Reg vector2d_func[] = {
        {"x", vector2d_getX},
        {"y", vector2d_getY},
        {"norm", vector2d_norm},
};

#define VECTOR2F_METATABLE_HEADER(FUNC_NAME) LUA_WRAPPER_HEADER(FUNC_NAME)

VECTOR2F_METATABLE_HEADER(vector2d_add)

VECTOR2F_METATABLE_HEADER(vector2d_subtract)

VECTOR2F_METATABLE_HEADER(vector2d_multiply)

VECTOR2F_METATABLE_HEADER(vector2d_divide)

VECTOR2F_METATABLE_HEADER(vector2d_tostring)

static const struct luaL_Reg vector2d_metatable[] = {
        {"__add",      vector2d_add},
        {"__sub",      vector2d_subtract},
        {"__mul",      vector2d_multiply},
        {"__div",      vector2d_divide},
        {"__tostring", vector2d_tostring},
};

LUA_WRAPPER_HEADER(matrix_vector3d)

#define VECTOR3_LIB_HEADER(FUNC_NAME) LUA_WRAPPER_HEADER(FUNC_NAME)

VECTOR3_LIB_HEADER(vector3d_getX)

VECTOR3_LIB_HEADER(vector3d_getY)

VECTOR3_LIB_HEADER(vector3d_getZ)

VECTOR3_LIB_HEADER(vector3d_toVector4d)

VECTOR3_LIB_HEADER(vector3d_normalized)

VECTOR3_LIB_HEADER(vector3d_dot)

VECTOR3_LIB_HEADER(vector3d_cross)

VECTOR3_LIB_HEADER(vector3d_toInt)

VECTOR3_LIB_HEADER(vector3d_asTable)

static const struct luaL_Reg vector3d_func[] = {
        {"x",          vector3d_getX},
        {"y",          vector3d_getY},
        {"z",          vector3d_getZ},
        {"toVec4f",    vector3d_toVector4d},
        {"normalized", vector3d_normalized},
        {"dot",        vector3d_dot},
        {"cross",      vector3d_cross},
        {"toInt",      vector3d_toInt},
        {"asTable",    vector3d_asTable},
};

#define VECTOR3_METATABLE_HEADER(FUNC_NAME) LUA_WRAPPER_HEADER(FUNC_NAME)

VECTOR3_METATABLE_HEADER(vector3d_add)

VECTOR3_METATABLE_HEADER(vector3d_subtract)

VECTOR3_METATABLE_HEADER(vector3d_multiply)

VECTOR3_METATABLE_HEADER(vector3d_divide)

VECTOR3_METATABLE_HEADER(vector3d_tostring)

static const struct luaL_Reg vector3d_metatable[] = {
        {"__add",      vector3d_add},
        {"__sub",      vector3d_subtract},
        {"__mul",      vector3d_multiply},
        {"__div",      vector3d_divide},
        {"__tostring", vector3d_tostring},
};


LUA_WRAPPER_HEADER(matrix_vector4d)

#define VECTOR4_LIB_HEADER(FUNC_NAME) LUA_WRAPPER_HEADER(FUNC_NAME)

VECTOR4_LIB_HEADER(vector4d_getX)

VECTOR4_LIB_HEADER(vector4d_getY)

VECTOR4_LIB_HEADER(vector4d_getZ)

VECTOR4_LIB_HEADER(vector4d_getW)

VECTOR4_LIB_HEADER(vector4d_toVector3d)

static const struct luaL_Reg vector4d_func[] = {
        {"x",       vector4d_getX},
        {"y",       vector4d_getY},
        {"z",       vector4d_getZ},
        {"w",       vector4d_getW},
        {"toVec3f", vector4d_toVector3d},
};

// matrix
LUA_WRAPPER_HEADER(matrix_matrix3x3)

#define MATRIX3x3_LIB_HEADER(FUNC_NAME) LUA_WRAPPER_HEADER(FUNC_NAME)

MATRIX3x3_LIB_HEADER(matrix_multiply3x3)

MATRIX3x3_LIB_HEADER(matrix_multiply3x1)

MATRIX3x3_LIB_HEADER(matrix_print3x3)

MATRIX3x3_LIB_HEADER(matrix_inverse3x3)

MATRIX3x3_LIB_HEADER(matrix_transpose3x3)


static const struct luaL_Reg matrix3x3_func[] = {
        {"multiplyMatrix", matrix_multiply3x3},
        {"multiplyVector", matrix_multiply3x1},
        {"print",          matrix_print3x3},
        {"inverse",        matrix_inverse3x3},
        {"transpose",      matrix_transpose3x3},
};

LUA_WRAPPER_HEADER(matrix_matrix4x4)

#define MATRIX4x4_LIB_HEADER(FUNC_NAME) LUA_WRAPPER_HEADER(FUNC_NAME)

MATRIX4x4_LIB_HEADER(matrix_multiply4x4)

MATRIX4x4_LIB_HEADER(matrix_multiply4x1)

MATRIX3x3_LIB_HEADER(matrix_print4x4)

MATRIX4x4_LIB_HEADER(matrix_inverse4x4)

MATRIX4x4_LIB_HEADER(matrix_transpose4x4)

static const struct luaL_Reg matrix4x4_func[] = {
        {"multiplyMatrix", matrix_multiply4x4},
        {"multiplyVector", matrix_multiply4x1},
        {"print",          matrix_print4x4},
        {"inverse",        matrix_inverse4x4},
        {"transpose",      matrix_transpose4x4},
};

#endif //TINY_RENDERER_MATRIX_HEADER_H
