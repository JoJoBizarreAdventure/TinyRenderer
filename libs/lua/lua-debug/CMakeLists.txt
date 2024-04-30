project(lua-debug)

set(luasocketPath ${CMAKE_CURRENT_BINARY_DIR}/luasocket)
set(luasocketCPath "${luasocketPath}" PARENT_SCOPE)

#luasocket core
add_library(luasocket SHARED)
set_target_properties(luasocket PROPERTIES OUTPUT_NAME core)
SET_TARGET_PROPERTIES(luasocket PROPERTIES PREFIX "")
set_target_properties(luasocket PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${luasocketPath}/socket")
set_target_properties(luasocket PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${luasocketPath}/socket")
set_target_properties(luasocket PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${luasocketPath}/socket")

link_lua(luasocket)
if (WIN32)
    target_link_libraries(luasocket PRIVATE wsock32 ws2_32)
endif ()

set(luasocketSrcDir "${CMAKE_CURRENT_SOURCE_DIR}/luasocket/src")

set(luasocketSrcFiles luasocket.c
        auxiliar.c buffer.c compat.c except.c inet.c io.c options.c select.c tcp.c timeout.c udp.c)

if (WIN32)
    list(APPEND luasocketSrcFiles wsocket.c)
else ()
    list(APPEND luasocketSrcFiles usocket.c)
endif ()

list(TRANSFORM luasocketSrcFiles PREPEND ${luasocketSrcDir}/)
target_sources(luasocket PRIVATE ${luasocketSrcFiles})

#luasocket mime
add_library(mime SHARED ${luasocketSrcDir}/mime.c)
set_target_properties(mime PROPERTIES OUTPUT_NAME core)
SET_TARGET_PROPERTIES(mime PROPERTIES PREFIX "")
set_target_properties(mime PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${luasocketPath}/mime")
set_target_properties(mime PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${luasocketPath}/mime")
set_target_properties(luasocket PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${luasocketPath}/mime")
link_lua(mime)
add_dependencies(luasocket mime)

#luasocket lua file
set(luasocketBaseDir ltn12.lua socket.lua mime.lua)
list(TRANSFORM luasocketBaseDir PREPEND ${luasocketSrcDir}/)
set(luasocketLuaFiles http.lua tp.lua ftp.lua smtp.lua url.lua)
list(TRANSFORM luasocketLuaFiles PREPEND ${luasocketSrcDir}/)
file(COPY ${luasocketBaseDir} DESTINATION ${luasocketPath})
file(COPY ${luasocketLuaFiles} DESTINATION ${luasocketPath}/socket)

#mobdebug lua file
set(mobdebugPath ${CMAKE_CURRENT_BINARY_DIR}/mobdebug)
set(mobdebugCPath "${mobdebugPath}" PARENT_SCOPE)
file(COPY MobDebug/src/mobdebug.lua DESTINATION ${mobdebugPath})