#ifndef TINY_RENDERER_MATRIX_H
#define TINY_RENDERER_MATRIX_H

#include "matrix_header.h"
#include <iostream>

#pragma region vector


LUA_WRAPPER_IMPLEMENT(matrix_vector2i) {
    int num[2];
    for (int i = 0; i < 2; i++) {
        lua_pushinteger(L, i + 1);
        lua_gettable(L, 1);
        num[i] = lua_tointeger(L, -1);
        lua_pop(L, 1);
    }

    auto *ptr = createUserDataAndClass<Eigen::Vector2i>(L, vector2i_func);
    *ptr = Eigen::Vector2i();
    *ptr << num[0], num[1];

    return 1;
}

#define VECTOR2I_LIB_FUNC(FUNC_NAME) LUA_WRAPPER_IMPLEMENT(FUNC_NAME)

VECTOR2I_LIB_FUNC(vector2i_getX) {
    auto *ptr = getPtr<Eigen::Vector2i>(L);
    lua_pushinteger(L, ptr->x());
    return 1;
}

VECTOR2I_LIB_FUNC(vector2i_getY) {
    auto *ptr = getPtr<Eigen::Vector2i>(L);
    lua_pushinteger(L, ptr->y());
    return 1;
}


LUA_WRAPPER_IMPLEMENT(matrix_vector2d) {
    double num[2];
    for (int i = 0; i < 2; i++) {
        lua_pushinteger(L, i + 1);
        lua_gettable(L, 1);
        num[i] = lua_tonumber(L, -1);
        lua_pop(L, 1);
    }

    auto *ptr = createUserDataAndClass<Eigen::Vector2d>(L, vector2d_func, vector2d_metatable, Vector2d);
    *ptr = Eigen::Vector2d();
    *ptr << num[0], num[1];

    return 1;
}

#define VECTOR2F_LIB_FUNC(FUNC_NAME) LUA_WRAPPER_IMPLEMENT(FUNC_NAME)

VECTOR2F_LIB_FUNC(vector2d_getX) {
    auto *ptr = getPtr<Eigen::Vector2d>(L);
    lua_pushnumber(L, ptr->x());
    return 1;
}

VECTOR2F_LIB_FUNC(vector2d_getY) {
    auto *ptr = getPtr<Eigen::Vector2d>(L);
    lua_pushnumber(L, ptr->y());
    return 1;
}

VECTOR2F_LIB_FUNC(vector2d_norm) {
    auto *ptr = getPtr<Eigen::Vector2d>(L);
    lua_pushnumber(L, ptr->norm());
    return 1;
}

#define VECTOR2F_METATABLE_FUNC(FUNC_NAME) LUA_WRAPPER_IMPLEMENT(FUNC_NAME)

VECTOR2F_METATABLE_FUNC(vector2d_add) {
    auto *thisVec2 = getPtr<Eigen::Vector2d>(L);

    int t = lua_type(L, 2);
    if (t == LUA_TNUMBER) { /* numbers */
        if (lua_isinteger(L, 2)) /* integer? */
        {
            auto num = static_cast<double>(lua_tointeger(L, 2));
            auto *ptr = createUserDataAndClass<Eigen::Vector2d>(L, vector2d_func, vector2d_metatable, Vector2d);
            *ptr = {thisVec2->x() + num, thisVec2->y() + num};

        } else /* float */
        {
            double num = lua_tonumber(L, 2);
            auto *ptr = createUserDataAndClass<Eigen::Vector2d>(L, vector2d_func, vector2d_metatable, Vector2d);
            *ptr = {thisVec2->x() + num, thisVec2->y() + num};
        }
    } else if (strcmp("table", lua_typename(L, t)) == 0) {
        lua_pushstring(L, "type");
        lua_gettable(L, 2);
        if (lua_isinteger(L, -1)) {
            auto type = static_cast<MatrixType>(lua_tointeger(L, -1));
            if (type == Vector2d) {
                auto *otherVec2 = getPtr<Eigen::Vector2d>(L, 2);
                auto *ptr = createUserDataAndClass<Eigen::Vector2d>(L, vector2d_func, vector2d_metatable, Vector2d);
                *ptr = *thisVec2 + *otherVec2;
            }
        }
    }

    return 1;
}

VECTOR2F_METATABLE_FUNC(vector2d_subtract) {
    auto *thisVec2 = getPtr<Eigen::Vector2d>(L);

    int t = lua_type(L, 2);
    if (t == LUA_TNUMBER) { /* numbers */
        if (lua_isinteger(L, 2)) /* integer? */
        {
            auto num = static_cast<double>(lua_tointeger(L, 2));
            auto *ptr = createUserDataAndClass<Eigen::Vector2d>(L, vector2d_func, vector2d_metatable, Vector2d);
            *ptr = {thisVec2->x() - num, thisVec2->y() - num};

        } else /* float */
        {
            double num = lua_tonumber(L, 2);
            auto *ptr = createUserDataAndClass<Eigen::Vector2d>(L, vector2d_func, vector2d_metatable, Vector2d);
            *ptr = {thisVec2->x() - num, thisVec2->y() - num};
        }
    } else if (strcmp("table", lua_typename(L, t)) == 0) {
        lua_pushstring(L, "type");
        lua_gettable(L, 2);
        if (lua_isinteger(L, -1)) {
            auto type = static_cast<MatrixType>(lua_tointeger(L, -1));
            if (type == Vector2d) {
                auto *otherVec2 = getPtr<Eigen::Vector2d>(L, 2);
                auto *ptr = createUserDataAndClass<Eigen::Vector2d>(L, vector2d_func, vector2d_metatable, Vector2d);
                *ptr = *thisVec2 - *otherVec2;
            }
        }
    }

    return 1;
}

VECTOR2F_METATABLE_FUNC(vector2d_multiply) {
    auto *thisVec2 = getPtr<Eigen::Vector2d>(L);

    int t = lua_type(L, 2);
    if (t == LUA_TNUMBER) { /* numbers */
        if (lua_isinteger(L, 2)) /* integer? */
        {
            auto num = static_cast<double>(lua_tointeger(L, 2));
            auto *ptr = createUserDataAndClass<Eigen::Vector2d>(L, vector2d_func, vector2d_metatable, Vector2d);
            *ptr = {thisVec2->x() * num, thisVec2->y() * num};

        } else /* float */
        {
            double num = lua_tonumber(L, 2);
            auto *ptr = createUserDataAndClass<Eigen::Vector2d>(L, vector2d_func, vector2d_metatable, Vector2d);
            *ptr = {thisVec2->x() * num, thisVec2->y() * num};
        }
    } else if (strcmp("table", lua_typename(L, t)) == 0) {
        lua_pushstring(L, "type");
        lua_gettable(L, 2);
        if (lua_isinteger(L, -1)) {
            auto type = static_cast<MatrixType>(lua_tointeger(L, -1));
            if (type == Vector2d) {
                auto *otherVec2 = getPtr<Eigen::Vector2d>(L, 2);
                auto *ptr = createUserDataAndClass<Eigen::Vector2d>(L, vector2d_func, vector2d_metatable, Vector2d);
                *ptr = {thisVec2->x() * otherVec2->x(), thisVec2->y() * otherVec2->y()};
            }
        }
    }

    return 1;
}

VECTOR2F_METATABLE_FUNC(vector2d_divide) {
    auto *thisVec2 = getPtr<Eigen::Vector2d>(L);

    int t = lua_type(L, 2);
    if (t == LUA_TNUMBER) { /* numbers */
        if (lua_isinteger(L, 2)) /* integer? */
        {
            auto num = static_cast<double>(lua_tointeger(L, 2));
            auto *ptr = createUserDataAndClass<Eigen::Vector2d>(L, vector2d_func, vector2d_metatable, Vector2d);
            *ptr = {thisVec2->x() / num, thisVec2->y() / num};

        } else /* float */
        {
            double num = lua_tonumber(L, 2);
            auto *ptr = createUserDataAndClass<Eigen::Vector2d>(L, vector2d_func, vector2d_metatable, Vector2d);
            *ptr = {thisVec2->x() / num, thisVec2->y() / num};
        }
    } else if (strcmp("table", lua_typename(L, t)) == 0) {
        lua_pushstring(L, "type");
        lua_gettable(L, 2);
        if (lua_isinteger(L, -1)) {
            auto type = static_cast<MatrixType>(lua_tointeger(L, -1));
            if (type == Vector2d) {
                auto *otherVec2 = getPtr<Eigen::Vector2d>(L, 2);
                auto *ptr = createUserDataAndClass<Eigen::Vector2d>(L, vector2d_func, vector2d_metatable, Vector2d);
                *ptr = {thisVec2->x() / otherVec2->x(), thisVec2->y() / otherVec2->y()};
            }
        }
    }

    return 1;
}

VECTOR2F_METATABLE_FUNC(vector2d_tostring) {
    auto *thisVec2 = getPtr<Eigen::Vector2d>(L);

    std::stringstream ss;
    ss << "vec2(" << thisVec2->x() << ", " << thisVec2->y() << ")";

    lua_pushstring(L, ss.str().c_str());

    return 1;
}

LUA_WRAPPER_IMPLEMENT(matrix_vector3d) {
    double num[3];
    for (int i = 0; i < 3; i++) {
        lua_pushinteger(L, i + 1);
        lua_gettable(L, 1);
        num[i] = lua_tonumber(L, -1);
        lua_pop(L, 1);
    }

    auto *ptr = createUserDataAndClass<Eigen::Vector3d>(L, vector3d_func, vector3d_metatable, Vector3d);
    *ptr = Eigen::Vector3d();
    *ptr << num[0], num[1], num[2];

    return 1;
}

#define VECTOR3_LIB_FUNC(FUNC_NAME) LUA_WRAPPER_IMPLEMENT(FUNC_NAME)

VECTOR3_LIB_FUNC(vector3d_getX) {
    auto *ptr = getPtr<Eigen::Vector3d>(L);
    lua_pushnumber(L, ptr->x());
    return 1;
}

VECTOR3_LIB_FUNC(vector3d_getY) {
    auto *ptr = getPtr<Eigen::Vector3d>(L);
    lua_pushnumber(L, ptr->y());
    return 1;
}

VECTOR3_LIB_FUNC(vector3d_getZ) {
    auto *ptr = getPtr<Eigen::Vector3d>(L);
    lua_pushnumber(L, ptr->z());
    return 1;
}

VECTOR3_LIB_FUNC(vector3d_toVector4d) {
    auto *ptr = getPtr<Eigen::Vector3d>(L);
    double w;
    if (lua_isnumber(L, 2)) {
        w = lua_tonumber(L, 2);
    } else {
        w = 0;
    }
    auto *vec4 = createUserDataAndClass<Eigen::Vector4d>(L, vector4d_func);
    *vec4 = Eigen::Vector4d(ptr->x(), ptr->y(), ptr->z(), w);
    return 1;
}

VECTOR3_LIB_FUNC(vector3d_normalized) {
    auto *ptr = getPtr<Eigen::Vector3d>(L);

    auto *vec3 = createUserDataAndClass<Eigen::Vector3d>(L, vector3d_func, vector3d_metatable, Vector3d);
    *vec3 = ptr->normalized();
    return 1;
}

VECTOR3_LIB_FUNC(vector3d_dot) {
    auto *ptr1 = getPtr<Eigen::Vector3d>(L);
    auto *ptr2 = getPtr<Eigen::Vector3d>(L, 2);

    double result = ptr1->dot(*ptr2);
    lua_pushnumber(L, result);
    return 1;
}

VECTOR3_LIB_FUNC(vector3d_cross) {
    auto *ptr1 = getPtr<Eigen::Vector3d>(L);
    auto *ptr2 = getPtr<Eigen::Vector3d>(L, 2);

    auto *vec3 = createUserDataAndClass<Eigen::Vector3d>(L, vector3d_func, vector3d_metatable, Vector3d);
    *vec3 = ptr1->cross(*ptr2);

    return 1;
}

VECTOR3_LIB_FUNC(vector3d_toInt) {
    auto *ptr = getPtr<Eigen::Vector3d>(L);

    intToTable(L, {
            static_cast<int>(ptr->x()),
            static_cast<int>(ptr->y()),
            static_cast<int>(ptr->z())
    });

    return 1;
}

VECTOR3_LIB_FUNC(vector3d_asTable) {
    auto *ptr = getPtr<Eigen::Vector3d>(L);

    doubleToTable(L, {
            ptr->x(),
            ptr->y(),
            ptr->z()
    });

    return 1;
}

#define VECTOR3_METATABLE_FUNC(FUNC_NAME) LUA_WRAPPER_IMPLEMENT(FUNC_NAME)

VECTOR3_METATABLE_FUNC(vector3d_add) {
    auto *thisVec3 = getPtr<Eigen::Vector3d>(L);

    int t = lua_type(L, 2);
    if (t == LUA_TNUMBER) { /* numbers */
        if (lua_isinteger(L, 2)) /* integer? */
        {
            auto num = static_cast<double>(lua_tointeger(L, 2));
            auto *ptr = createUserDataAndClass<Eigen::Vector3d>(L, vector3d_func, vector3d_metatable, Vector3d);
            *ptr = {thisVec3->x() + num, thisVec3->y() + num, thisVec3->z() + num};

        } else /* float */
        {
            double num = lua_tonumber(L, 2);
            auto *ptr = createUserDataAndClass<Eigen::Vector3d>(L, vector3d_func, vector3d_metatable, Vector3d);
            *ptr = {thisVec3->x() + num, thisVec3->y() + num, thisVec3->z() + num};
        }
    } else if (strcmp("table", lua_typename(L, t)) == 0) {
        lua_pushstring(L, "type");
        lua_gettable(L, 2);
        if (lua_isinteger(L, -1)) {
            auto type = static_cast<MatrixType>(lua_tointeger(L, -1));
            if (type == Vector3d) {
                auto *otherVec3 = getPtr<Eigen::Vector3d>(L, 2);
                auto *ptr = createUserDataAndClass<Eigen::Vector3d>(L, vector3d_func, vector3d_metatable, Vector3d);
                *ptr = *thisVec3 + *otherVec3;
            }
        }
    }

    return 1;
}

VECTOR3_METATABLE_FUNC(vector3d_subtract) {
    auto *thisVec3 = getPtr<Eigen::Vector3d>(L);

    int t = lua_type(L, 2);
    if (t == LUA_TNUMBER) { /* numbers */
        if (lua_isinteger(L, 2)) /* integer? */
        {
            auto num = static_cast<double>(lua_tointeger(L, 2));
            auto *ptr = createUserDataAndClass<Eigen::Vector3d>(L, vector3d_func, vector3d_metatable, Vector3d);
            *ptr = {thisVec3->x() - num, thisVec3->y() - num, thisVec3->z() - num};

        } else /* float */
        {
            double num = lua_tonumber(L, 2);
            auto *ptr = createUserDataAndClass<Eigen::Vector3d>(L, vector3d_func, vector3d_metatable, Vector3d);
            *ptr = {thisVec3->x() - num, thisVec3->y() - num, thisVec3->z() - num};
        }
    } else if (strcmp("table", lua_typename(L, t)) == 0) {
        lua_pushstring(L, "type");
        lua_gettable(L, 2);
        if (lua_isinteger(L, -1)) {
            auto type = static_cast<MatrixType>(lua_tointeger(L, -1));
            if (type == Vector3d) {
                auto *otherVec3 = getPtr<Eigen::Vector3d>(L, 2);
                auto *ptr = createUserDataAndClass<Eigen::Vector3d>(L, vector3d_func, vector3d_metatable, Vector3d);
                *ptr = *thisVec3 - *otherVec3;
            }
        }
    }

    return 1;
}

VECTOR3_METATABLE_FUNC(vector3d_multiply) {
    auto *thisVec3 = getPtr<Eigen::Vector3d>(L);

    int t = lua_type(L, 2);
    if (t == LUA_TNUMBER) { /* numbers */
        if (lua_isinteger(L, 2)) /* integer? */
        {
            auto num = static_cast<double>(lua_tointeger(L, 2));
            auto *ptr = createUserDataAndClass<Eigen::Vector3d>(L, vector3d_func, vector3d_metatable, Vector3d);
            *ptr = *thisVec3 * num;

        } else /* float */
        {
            double num = lua_tonumber(L, 2);
            auto *ptr = createUserDataAndClass<Eigen::Vector3d>(L, vector3d_func, vector3d_metatable, Vector3d);
            *ptr = *thisVec3 * num;
        }
    } else if (strcmp("table", lua_typename(L, t)) == 0) {
        lua_pushstring(L, "type");
        lua_gettable(L, 2);
        if (lua_isinteger(L, -1)) {
            auto type = static_cast<MatrixType>(lua_tointeger(L, -1));
            if (type == Vector3d) {
                auto *otherVec3 = getPtr<Eigen::Vector3d>(L, 2);
                auto *ptr = createUserDataAndClass<Eigen::Vector3d>(L, vector3d_func, vector3d_metatable, Vector3d);
                *ptr = {thisVec3->x() * otherVec3->x(), thisVec3->y() * otherVec3->y(), thisVec3->z() * otherVec3->z()};
            }
        }
    }

    return 1;
}

VECTOR3_METATABLE_FUNC(vector3d_divide) {
    auto *thisVec3 = getPtr<Eigen::Vector3d>(L);

    int t = lua_type(L, 2);
    if (t == LUA_TNUMBER) { /* numbers */
        if (lua_isinteger(L, 2)) /* integer? */
        {
            auto num = static_cast<double>(lua_tointeger(L, 2));
            auto *ptr = createUserDataAndClass<Eigen::Vector3d>(L, vector3d_func, vector3d_metatable, Vector3d);
            *ptr = *thisVec3 / num;

        } else /* float */
        {
            double num = lua_tonumber(L, 2);
            auto *ptr = createUserDataAndClass<Eigen::Vector3d>(L, vector3d_func, vector3d_metatable, Vector3d);
            *ptr = *thisVec3 / num;
        }
    } else if (strcmp("table", lua_typename(L, t)) == 0) {
        lua_pushstring(L, "type");
        lua_gettable(L, 2);
        if (lua_isinteger(L, -1)) {
            auto type = static_cast<MatrixType>(lua_tointeger(L, -1));
            if (type == Vector3d) {
                auto *otherVec3 = getPtr<Eigen::Vector3d>(L, 2);
                auto *ptr = createUserDataAndClass<Eigen::Vector3d>(L, vector3d_func, vector3d_metatable, Vector3d);
                *ptr = {thisVec3->x() / otherVec3->x(), thisVec3->y() / otherVec3->y(), thisVec3->z() / otherVec3->z()};
            }
        }
    }

    return 1;
}

VECTOR3_METATABLE_FUNC(vector3d_tostring) {
    auto *thisVec3 = getPtr<Eigen::Vector3d>(L);

    std::stringstream ss;
    ss << "vec3(" << thisVec3->x() << ", " << thisVec3->y() << ", " << thisVec3->z() << ")";

    lua_pushstring(L, ss.str().c_str());

    return 1;
}


LUA_WRAPPER_IMPLEMENT(matrix_vector4d) {
    double num[4];
    for (int i = 0; i < 4; i++) {
        lua_pushinteger(L, i + 1);
        lua_gettable(L, 1);
        num[i] = lua_tonumber(L, -1);
        lua_pop(L, 1);
    }

    auto *ptr = createUserDataAndClass<Eigen::Vector4d>(L, vector4d_func);
    *ptr = Eigen::Vector4d();
    *ptr << num[0], num[1], num[2], num[3];

    return 1;
}

#define VECTOR4_LIB_FUNC(FUNC_NAME) LUA_WRAPPER_IMPLEMENT(FUNC_NAME)

VECTOR4_LIB_FUNC(vector4d_getX) {
    auto *ptr = getPtr<Eigen::Vector4d>(L);
    lua_pushnumber(L, ptr->x());
    return 1;
}

VECTOR4_LIB_FUNC(vector4d_getY) {
    auto *ptr = getPtr<Eigen::Vector4d>(L);
    lua_pushnumber(L, ptr->y());
    return 1;
}

VECTOR4_LIB_FUNC(vector4d_getZ) {
    auto *ptr = getPtr<Eigen::Vector4d>(L);
    lua_pushnumber(L, ptr->z());
    return 1;
}

VECTOR4_LIB_FUNC(vector4d_getW) {
    auto *ptr = getPtr<Eigen::Vector4d>(L);
    lua_pushnumber(L, ptr->w());
    return 1;
}

VECTOR4_LIB_FUNC(vector4d_toVector3d) {
    auto *ptr = getPtr<Eigen::Vector4d>(L);
    bool unitW = false;
    if (lua_isboolean(L, 2)) {
        unitW = lua_toboolean(L, 2);
    }
    auto *vec3 = createUserDataAndClass<Eigen::Vector3d>(L, vector3d_func, vector3d_metatable, Vector3d);
    if (unitW) {
        *vec3 = Eigen::Vector3d(ptr->x() / ptr->w(), ptr->y() / ptr->w(), ptr->z() / ptr->w());
    } else {
        *vec3 = Eigen::Vector3d(ptr->x(), ptr->y(), ptr->z());
    }

    return 1;
}

#pragma endregion

#pragma region matrix


LUA_WRAPPER_IMPLEMENT(matrix_matrix3x3) {
    double num[9];
    for (int y = 0; y < 3; y++) {
        lua_pushinteger(L, y + 1);
        lua_gettable(L, 1);
        for (int x = 0; x < 3; x++) {
            lua_pushinteger(L, x + 1);
            lua_gettable(L, -2);
            num[y * 3 + x] = lua_tonumber(L, -1);
            lua_pop(L, 1);
        }
        lua_pop(L, 1);
    }

    auto *matrix = createUserDataAndClass<Eigen::Matrix3d>(L, matrix3x3_func);
    *matrix = Eigen::Matrix3d();
    *matrix << num[0], num[1], num[2],
            num[3], num[4], num[5],
            num[6], num[7], num[8];

    return 1;
}

#define MATRIX3_LIB_FUNC(FUNC_NAME) LUA_WRAPPER_IMPLEMENT(FUNC_NAME)

MATRIX3_LIB_FUNC(matrix_multiply3x3) {
    auto *ptr1 = getPtr<Eigen::Matrix3d>(L);
    auto *ptr2 = getPtr<Eigen::Matrix3d>(L, 2);

    auto *matrix = createUserDataAndClass<Eigen::Matrix3d>(L, matrix3x3_func);
    *matrix = *ptr1 * *ptr2;

    return 1;
}

MATRIX3_LIB_FUNC(matrix_multiply3x1) {
    auto *ptr1 = getPtr<Eigen::Matrix3d>(L);
    auto *ptr2 = getPtr<Eigen::Vector3d>(L, 2);

    auto vector = createUserDataAndClass<Eigen::Vector3d>(L, vector3d_func, vector3d_metatable, Vector3d);
    *vector = *ptr1 * *ptr2;

    return 1;
}

MATRIX3_LIB_FUNC(matrix_print3x3) {
    auto *ptr = getPtr<Eigen::Matrix3d>(L);

    std::cout << "Matrix3d:" << *ptr << std::endl;

    return 0;
}

MATRIX3_LIB_FUNC(matrix_inverse3x3) {
    auto *ptr = getPtr<Eigen::Matrix3d>(L);

    auto *matrix = createUserDataAndClass<Eigen::Matrix3d>(L, matrix3x3_func);
    *matrix = ptr->inverse();

    return 1;
}

MATRIX3_LIB_FUNC(matrix_transpose3x3) {
    auto *ptr = getPtr<Eigen::Matrix3d>(L);

    auto *matrix = createUserDataAndClass<Eigen::Matrix3d>(L, matrix3x3_func);
    *matrix = ptr->transpose();

    return 1;
}


LUA_WRAPPER_IMPLEMENT(matrix_matrix4x4) {
    double num[16];
    for (int y = 0; y < 4; y++) {
        lua_pushinteger(L, y + 1);
        lua_gettable(L, 1);
        for (int x = 0; x < 4; x++) {
            lua_pushinteger(L, x + 1);
            lua_gettable(L, -2);
            num[y * 4 + x] = lua_tonumber(L, -1);
            lua_pop(L, 1);
        }
        lua_pop(L, 1);
    }

    auto *matrix = createUserDataAndClass<Eigen::Matrix4d>(L, matrix4x4_func);
    *matrix = Eigen::Matrix4d();
    *matrix << num[0], num[1], num[2], num[3],
            num[4], num[5], num[6], num[7],
            num[8], num[9], num[10], num[11],
            num[12], num[13], num[14], num[15];

    return 1;
}

#define MATRIX4_LIB_FUNC(FUNC_NAME) LUA_WRAPPER_IMPLEMENT(FUNC_NAME)

MATRIX4_LIB_FUNC(matrix_multiply4x4) {
    auto *ptr1 = getPtr<Eigen::Matrix4d>(L);
    auto *ptr2 = getPtr<Eigen::Matrix4d>(L, 2);

    auto *matrix = createUserDataAndClass<Eigen::Matrix4d>(L, matrix4x4_func);
    *matrix = *ptr1 * *ptr2;

    return 1;
}

MATRIX4_LIB_FUNC(matrix_multiply4x1) {
    auto *ptr1 = getPtr<Eigen::Matrix4d>(L);
    auto *ptr2 = getPtr<Eigen::Vector4d>(L, 2);

    auto vector = createUserDataAndClass<Eigen::Vector4d>(L, vector4d_func);
    *vector = Eigen::Vector4d();
    *vector = *ptr1 * *ptr2;

    return 1;
}

MATRIX4_LIB_FUNC(matrix_print4x4) {
    auto *ptr = getPtr<Eigen::Matrix4d>(L);

    std::cout << "Matrix4d:\n" << *ptr << std::endl;

    return 1;
}

MATRIX4_LIB_FUNC(matrix_inverse4x4) {
    auto *ptr = getPtr<Eigen::Matrix4d>(L);

    auto *matrix = createUserDataAndClass<Eigen::Matrix4d>(L, matrix4x4_func);
    *matrix = ptr->inverse();

    return 1;
}

MATRIX4_LIB_FUNC(matrix_transpose4x4) {
    auto *ptr = getPtr<Eigen::Matrix4d>(L);

    auto *matrix = createUserDataAndClass<Eigen::Matrix4d>(L, matrix4x4_func);
    *matrix = ptr->transpose();

    return 1;
}

#pragma endregion

#endif //TINY_RENDERER_MATRIX_H
