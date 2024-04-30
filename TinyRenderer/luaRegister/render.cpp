#include "render.h"
#include "matrix.h"

#pragma region tgaImage

#define TGA_IMAGE_LIB_FUNC(FUNC_NAME) LUA_WRAPPER_IMPLEMENT(FUNC_NAME)

static const struct luaL_Reg image_func[] = {
        {"save",         tgaImage_save},
        {"close",        tgaImage_close},
        {"getColorAt",   tgaImage_getColorAt},
        {"getZBufferAt", tgaImage_getZBufferAt},
        {"setPixel", tgaImage_setPixel},
        //utils
        {"line",         utils_line},
        {"triangleL2",   utils_triangle_l2},
        {"triangleL3",   utils_triangle_l3},
        {"triangle",     utils_triangle_l3},
};

/* render.createImage(width, height, withZBuffer)  -> return TGAImage*
 * width, height: define image size
 * withZBuffer: create float array for zBuffer or not, default false
 * */
LUA_WRAPPER_IMPLEMENT(tgaImage_create) {
    int width = static_cast<int>(luaL_checkinteger(L, 1));
    int height = static_cast<int>(luaL_checkinteger(L, 2));
    bool withZBuffer = lua_toboolean(L, 3);
    auto *image = new TGAImage(width, height, TGAImage::RGB);

    createClass(L, image, image_func);

    if (withZBuffer) {
        auto *zBuffer = new double[width * height];
        std::fill(zBuffer, zBuffer + width * height, -std::numeric_limits<double>::max());

        lua_pushstring(L, "zBuffer");
        lua_pushlightuserdata(L, zBuffer);
        lua_settable(L, -3);
    }

    return 1;
}

LUA_WRAPPER_IMPLEMENT(tgaImage_open) {
    const char *path = lua_tostring(L, 1);

    auto *image = createUserDataAndClass<TGAImage>(L, image_func);
    image->emptyPtr();
    image->read_tga_file(path);
    image->flip_vertically();

    lua_pushinteger(L, image->get_width());
    lua_pushinteger(L, image->get_height());

    return 3;
}

TGA_IMAGE_LIB_FUNC(tgaImage_save) {
    auto *image = getPtr<TGAImage>(L);
    const char *name = lua_tostring(L, 2);

    image->flip_vertically(); // I want to have the origin at the left bottom corner of the image
    image->write_tga_file(name);

    delete image;
    return 1;
}

TGA_IMAGE_LIB_FUNC(tgaImage_close) {
    auto *image = getPtr<TGAImage>(L);
    delete image;
    return 0;
}

TGA_IMAGE_LIB_FUNC(tgaImage_getColorAt) {
    auto *image = getPtr<TGAImage>(L);
    int x = lua_tointeger(L, 2), y = lua_tointeger(L, 3);

    auto color = image->get(x, y);
    lua_newtable(L);

    lua_pushinteger(L, 1);
    lua_pushinteger(L, color.b);
    lua_settable(L, -3);

    lua_pushinteger(L, 2);
    lua_pushinteger(L, color.g);
    lua_settable(L, -3);

    lua_pushinteger(L, 3);
    lua_pushinteger(L, color.r);
    lua_settable(L, -3);

    lua_pushinteger(L, 4);
    lua_pushinteger(L, color.a);
    lua_settable(L, -3);

    return 1;
}

TGA_IMAGE_LIB_FUNC(tgaImage_getZBufferAt) {
    lua_pushstring(L, "zBuffer");
    lua_gettable(L, 1);
    auto zBuffer = (double *) lua_topointer(L, -1);
    lua_pop(L, 1);

    auto *image = getPtr<TGAImage>(L);

    int x = lua_tointeger(L, 2), y = lua_tointeger(L, 3);
    if (x >= 0 && x < image->get_width() && y >= 0 && y < image->get_height()) {
        int index = image->get_width() * y + x;
        lua_pushnumber(L, zBuffer[index]);
    } else {
        lua_pushnumber(L, std::numeric_limits<double>::min());
    }


    return 1;
}

TGA_IMAGE_LIB_FUNC(tgaImage_setPixel) {
    auto *image = getPtr<TGAImage>(L);

    int x = lua_tointeger(L, 2), y = lua_tointeger(L, 3);

    unsigned char b, g, r, a;
    lua_pushinteger(L, 1);
    lua_gettable(L, 4);
    b = static_cast<unsigned char>(lua_tointeger(L, -1));
    lua_pop(L, 1);

    lua_pushinteger(L, 2);
    lua_gettable(L, 4);
    g = static_cast<unsigned char>(lua_tointeger(L, -1));
    lua_pop(L, 1);

    lua_pushinteger(L, 3);
    lua_gettable(L, 4);
    r = static_cast<unsigned char>(lua_tointeger(L, -1));
    lua_pop(L, 1);

    lua_pushinteger(L, 4);
    lua_gettable(L, 4);
    a = static_cast<unsigned char>(lua_tointeger(L, -1));
    lua_pop(L, 1);

    image->set(x, y, {b, g, r, a});

    return 0;
}

#pragma endregion

#pragma region model

#define MODEL_LIB_FUNC(FUNC_NAME) LUA_WRAPPER_IMPLEMENT(FUNC_NAME)

static const struct luaL_Reg model_func[] = {
        {"unload",        model_unload},
        {"faces",         model_faces},
        {"vert",          model_vert},
        {"uvIndexes",     model_uvIndexes},
        {"uv",            model_uv},
        {"normalIndexes", model_normalIndexes},
        {"normal",        model_normal},
};

LUA_WRAPPER_IMPLEMENT(model_load) {
    const char *path = lua_tostring(L, 1);
    auto *model = new Model(path);

    createClass(L, model, model_func);

    return 1;
}

MODEL_LIB_FUNC(model_unload) {
    auto *model = getPtr<Model>(L);
    delete model;

    lua_pushstring(L, "zBuffer");
    lua_gettable(L, 1);
    auto *zBuffer = (double *) lua_topointer(L, -1);
    delete zBuffer;

    return 0;
}

MODEL_LIB_FUNC(model_faces) {
    auto *model = getPtr<Model>(L);
    lua_newtable(L);
    for (int i = 0; i < model->nFaces(); i++) {
        lua_pushinteger(L, i + 1);
        lua_newtable(L);

        std::vector<int> face = model->face(i);
        for (int j = 0; j < 3; j++) {
            lua_pushinteger(L, j + 1);
            lua_pushinteger(L, face[j]);
            lua_settable(L, -3);
        }
        lua_settable(L, -3);
    }
    lua_pushinteger(L, model->nFaces());
    return 2;
}

MODEL_LIB_FUNC(model_vert) {
    auto *model = getPtr<Model>(L);
    int i = lua_tointeger(L, 2);

    auto *ptr = createUserDataAndClass<Eigen::Vector3d>(L, vector3d_func, vector3d_metatable, Vector3d);
    *ptr = model->vert(i);

    return 1;
}

MODEL_LIB_FUNC(model_uvIndexes) {
    auto *model = getPtr<Model>(L);
    lua_newtable(L);
    for (int i = 0; i < model->nUVIndexes(); i++) {
        lua_pushinteger(L, i + 1);
        lua_newtable(L);

        std::vector<int> uvIndex = model->uvIndex(i);
        for (int j = 0; j < 3; j++) {
            lua_pushinteger(L, j + 1);
            lua_pushinteger(L, uvIndex[j]);
            lua_settable(L, -3);
        }
        lua_settable(L, -3);
    }
    lua_pushinteger(L, model->nUVIndexes());
    return 2;
}

MODEL_LIB_FUNC(model_uv) {
    auto *model = getPtr<Model>(L);
    int i = lua_tointeger(L, 2);

    Eigen::Vector2d v = model->uv(i); // no extend usage, just return uv

    auto *ptr = createUserDataAndClass<Eigen::Vector2d>(L, vector2d_func, vector2d_metatable, Vector2d);
    *ptr = v;

    return 1;
}

MODEL_LIB_FUNC(model_normalIndexes) {
    auto *model = getPtr<Model>(L);
    lua_newtable(L);
    for (int i = 0; i < model->nNormalIndexes(); i++) {
        lua_pushinteger(L, i + 1);
        lua_newtable(L);

        std::vector<int> normalIndex = model->normalIndex(i);
        for (int j = 0; j < 3; j++) {
            lua_pushinteger(L, j + 1);
            lua_pushinteger(L, normalIndex[j]);
            lua_settable(L, -3);
        }
        lua_settable(L, -3);
    }
    lua_pushinteger(L, model->nNormalIndexes());
    return 2;
}

MODEL_LIB_FUNC(model_normal) {
    auto *model = getPtr<Model>(L);
    int i = lua_tointeger(L, 2);

    auto *ptr = createUserDataAndClass<Eigen::Vector3d>(L, vector3d_func, vector3d_metatable, Vector3d);
    *ptr = model->normal(i);

    return 1;
}

#pragma endregion

#pragma region utils

TGAColor getColorFromTable(lua_State *L, int tableIdx) {
    lua_pushinteger(L, 1);
    lua_gettable(L, tableIdx);
    unsigned char b = lua_tointeger(L, -1);
    lua_pop(L, 1);

    lua_pushinteger(L, 2);
    lua_gettable(L, tableIdx);
    unsigned char g = lua_tointeger(L, -1);
    lua_pop(L, 1);

    lua_pushinteger(L, 3);
    lua_gettable(L, tableIdx);
    unsigned char r = lua_tointeger(L, -1);
    lua_pop(L, 1);

    lua_pushinteger(L, 4);
    lua_gettable(L, tableIdx);
    unsigned char a = lua_tointeger(L, -1);
    lua_pop(L, 1);

    return {b, g, r, a};
}

TGA_IMAGE_LIB_FUNC(utils_line) {
    auto *image = getPtr<TGAImage>(L);

    int x0 = static_cast<int>(luaL_checkinteger(L, 2));
    int y0 = static_cast<int>(luaL_checkinteger(L, 3));
    int x1 = static_cast<int>(luaL_checkinteger(L, 4));
    int y1 = static_cast<int>(luaL_checkinteger(L, 5));

    auto color = getColorFromTable(L, 6);

    line(x0, y0, x1, y1, *image, color);

    return 0;
}

TGA_IMAGE_LIB_FUNC(utils_triangle_l2) {
    auto *image = getPtr<TGAImage>(L);
    Eigen::Vector2i *v0, *v1, *v2;

    lua_pushinteger(L, 1);
    lua_gettable(L, 2);
    v0 = getPtr<Eigen::Vector2i>(L, -2);
    lua_pop(L, 1);

    lua_pushinteger(L, 2);
    lua_gettable(L, 2);
    v1 = getPtr<Eigen::Vector2i>(L, -2);
    lua_pop(L, 1);

    lua_pushinteger(L, 3);
    lua_gettable(L, 2);
    v2 = getPtr<Eigen::Vector2i>(L, -2);
    lua_pop(L, 1);

    auto color = getColorFromTable(L, 3);

    triangle_l2(*v0, *v1, *v2, *image, color);

    return 0;
}

TGA_IMAGE_LIB_FUNC(utils_triangle_l3) {
    auto *image = getPtr<TGAImage>(L);

    lua_pushstring(L, "zBuffer");
    lua_gettable(L, 1);
    auto *zBuffer = (double *) lua_topointer(L, -1);
    lua_pop(L, 1);

    Eigen::Vector3d *v0, *v1, *v2;

    lua_pushinteger(L, 1);
    lua_gettable(L, 2);
    v0 = getPtr<Eigen::Vector3d>(L, -2);
    lua_pop(L, 1);

    lua_pushinteger(L, 2);
    lua_gettable(L, 2);
    v1 = getPtr<Eigen::Vector3d>(L, -2);
    lua_pop(L, 1);

    lua_pushinteger(L, 3);
    lua_gettable(L, 2);
    v2 = getPtr<Eigen::Vector3d>(L, -2);
    lua_pop(L, 1);

    auto getColor = [&L](double u, double v) {
        lua_pushvalue(L, -1);
        lua_pushnumber(L, u);
        lua_pushnumber(L, v);
        lua_pcall(L, 2, 1, 0);
        auto color = getColorFromTable(L, 4);
        lua_pop(L, 1);
        return color;
    };

    triangle_l3(*v0, *v1, *v2, *image, zBuffer, getColor);

    return 0;
}


#pragma endregion

static const struct luaL_Reg render[] = {
        //tgaImage
        {"createImage", tgaImage_create},
        {"openImage",   tgaImage_open},
        //model
        {"loadModel",   model_load},
        //matrix
        {"vec2i",       matrix_vector2i},
        {"vec2f",       matrix_vector2d},
        {"vec3f",       matrix_vector3d},
        {"vec4f",       matrix_vector4d},
        {"matrix3x3",   matrix_matrix3x3},
        {"matrix4x4",   matrix_matrix4x4},
        //end
        {nullptr,       nullptr}
};

LUA_EXPORT int luaopen_render(lua_State *L) {
    luaL_newlib(L, render);
    return 1;
}
