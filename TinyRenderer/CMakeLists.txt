# main file
add_executable(${PROJECT_NAME} main.cpp)
link_lua(${PROJECT_NAME})
link_luaDebug(${PROJECT_NAME})

# register for lua
set(LUA_LIB_NAME render)
add_library(${LUA_LIB_NAME} SHARED luaRegister/render.cpp)
SET_TARGET_PROPERTIES(render PROPERTIES PREFIX "")
link_lua(${LUA_LIB_NAME})
link_eigen(${LUA_LIB_NAME})

# tga image lua
include_directories(tgaImage/include)
target_sources(${LUA_LIB_NAME} PRIVATE tgaImage/tgaimage.cpp)

# model
include_directories(model/include)
target_sources(${LUA_LIB_NAME} PRIVATE model/model.cpp)
file(COPY obj DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# utils lua
include_directories(utils/include)
target_sources(${LUA_LIB_NAME} PRIVATE utils/line.cpp utils/triangle.cpp)

# lua file
file(COPY lua DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# link to compile
target_link_libraries(${PROJECT_NAME} PRIVATE ${LUA_LIB_NAME})