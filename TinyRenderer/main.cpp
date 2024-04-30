#include "luaCpp.h"
#include "iostream"

int main(int argc, char **argv) {
    lua_State *L = luaL_newstate(); /* opens Lua */
    luaL_openlibs(L); /* opens the standard libraries */


    int ret = luaL_dofile(L, "lua/environment.lua");
    if (ret) {
        std::cout << "env:" << lua_tostring(L, -1) << std::endl;
    }
    ret = luaL_dofile(L, "lua/main.lua");
    if (ret) {
        std::cout << "load main:" << lua_tostring(L, -1) << std::endl;
    }

    lua_getglobal(L, "updateCPath");
    lua_pushstring(L, luasocketCPath);
    lua_pushstring(L, mobdebugCPath);
    ret = lua_pcall(L, 2, 0, 0);
    if (ret) {
        std::cout << "execute updateCPath:" << lua_tostring(L, -1) << std::endl;
    }

    lua_getglobal(L, "main");
    ret = lua_pcall(L, 0, 0, 0);
    if (ret) {
        std::cout << "execute main:" << lua_tostring(L, -1) << std::endl;
    }


    lua_close(L);
    return 0;
}
