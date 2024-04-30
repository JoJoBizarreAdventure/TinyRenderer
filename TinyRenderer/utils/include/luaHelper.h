#ifndef TINY_RENDERER_LUAHELPER_H
#define TINY_RENDERER_LUAHELPER_H

#include "luaCpp.h"
#include "initializer_list"

#if defined(_WIN32)
#define LUA_EXPORT extern "C" __declspec(dllexport)
#else
#define LUA_EXPORT extern "C" __attribute__ ((visibility ("default")))
#endif
#define LUA_WRAPPER_HEADER(FUNC_NAME) static int FUNC_NAME(lua_State*L);
#define LUA_WRAPPER_IMPLEMENT(FUNC_NAME) int FUNC_NAME(lua_State *L)

static void stackDump(lua_State *L) {
    int i;
    int top = lua_gettop(L); /* depth of the stack */
    for (i = 1; i <= top; i++) { /* repeat for each level */
        int t = lua_type(L, i);
        switch (t) {
            case LUA_TSTRING: { /* strings */
                printf("'%s'", lua_tostring(L, i));
                break;
            }
            case LUA_TBOOLEAN: { /* Booleans */
                printf(lua_toboolean(L, i) ? "true" : "false");
                break;
            }
            case LUA_TNUMBER: { /* numbers */
                if (lua_isinteger(L, i)) /* integer? */
                    printf("%lld", lua_tointeger(L, i));
                else /* float */
                    printf("%g", lua_tonumber(L, i));
                break;
            }
            default: { /* other values */
                printf("%s", lua_typename(L, t));
                break;
            }
        }
        printf(" "); /* put a separator */
    }
    printf("\n"); /* end the listing */
}

#define dump stackDump(L);

template<typename T>
static T *getPtr(lua_State *L, int idx = 1) {
    lua_pushstring(L, "ptr");
    lua_gettable(L, idx);
    auto ptr = (T *) lua_topointer(L, -1);
    lua_pop(L, 1);
    return ptr;
}

template<int N>
static void registerLib(lua_State *L, const struct luaL_Reg (&funcs)[N], int idx = -3) {
    for (const auto &func: funcs) {
        lua_pushstring(L, func.name);
        lua_pushcfunction(L, func.func);
        lua_settable(L, idx);
    }
}

template<int N>
static void createClass(lua_State *L, void *ptr, const struct luaL_Reg (&funcs)[N]) {
    lua_newtable(L);

    lua_pushstring(L, "ptr");
    lua_pushlightuserdata(L, ptr);
    lua_settable(L, -3);

    registerLib(L, funcs);
}

template<typename T, int N>
static T *createUserDataAndClass(lua_State *L, const struct luaL_Reg (&funcs)[N]) {
    lua_newtable(L);

    lua_pushstring(L, "ptr");
    auto *ptr = (T *) lua_newuserdata(L, sizeof(T));
    lua_settable(L, -3);

    registerLib(L, funcs);

    return ptr;
}

template<typename T, int N, int M>
static T *createUserDataAndClass(lua_State *L, const struct luaL_Reg (&funcs)[N], const struct luaL_Reg (&metatable)[M],
                                 int type = -1) {
    lua_newtable(L);

    lua_pushstring(L, "ptr");
    auto *ptr = (T *) lua_newuserdata(L, sizeof(T));
    lua_settable(L, -3);

    registerLib(L, funcs);

    lua_newtable(L);
    registerLib(L, metatable);
    lua_setmetatable(L, -2);

    lua_pushstring(L, "type");
    lua_pushinteger(L, type);
    lua_settable(L, -3);

    return ptr;
}

static void intToTable(lua_State *L, std::initializer_list<int> content) {
    lua_newtable(L);

    int index = 1;
    for (const auto &c: content) {
        lua_pushinteger(L, index);
        lua_pushinteger(L, c);
        lua_settable(L, -3);
        index++;
    }
}

static void doubleToTable(lua_State *L, std::initializer_list<double> content) {
    lua_newtable(L);

    int index = 1;
    for (const auto &c: content) {
        lua_pushinteger(L, index);
        lua_pushnumber(L, c);
        lua_settable(L, -3);
        index++;
    }
}

#endif //TINY_RENDERER_LUAHELPER_H
