cmake_minimum_required(VERSION 3.15)
project(TinyRenderer)

set(CMAKE_CXX_STANDARD 20)

if (MSVC)
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif ()

# global libs
add_subdirectory(libs)

# games list below
add_subdirectory(TinyRenderer)