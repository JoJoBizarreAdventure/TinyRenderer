add_subdirectory(eigen)
target_compile_definitions(eigen INTERFACE EIGEN_DONT_ALIGN_STATICALLY)
function(link_eigen target_name)
    target_link_libraries(${target_name} PRIVATE Eigen3::Eigen)
endfunction()

add_subdirectory(lua/lua-cmake)

function(link_lua target_name)
    target_link_libraries(${target_name} PRIVATE lua::lib)
endfunction()

add_subdirectory(lua/lua-debug)

function(link_luaDebug target_name)
    target_link_libraries(${target_name} PRIVATE luasocket)
    target_compile_definitions(${target_name} PRIVATE luasocketCPath="${luasocketCPath}")
    target_compile_definitions(${target_name} PRIVATE mobdebugCPath="${mobdebugCPath}")
endfunction()