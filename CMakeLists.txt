cmake_minimum_required(VERSION 3.10)

project(tcp_echo VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()

include_directories("${PROJECT_SOURCE_DIR}/include")

if(WIN32)
    set(PLATFORM_SOURCES src/win32_sockets.c src/win32_threads.c)
    set(CLIENT_SOURCE src/win32_client.c)
    set(THREAD_LIBS ws2_32)
else()
    set(PLATFORM_SOURCES src/posix_sockets.c src/posix_threads.c)
    set(CLIENT_SOURCE src/posix_client.c)
    set(THREAD_LIBS pthread)
endif()

set(MESSAGE_HANDLER_SOURCE src/msg_handling.c)

add_executable(tcp_echo_server src/server.c ${PLATFORM_SOURCES} ${MESSAGE_HANDLER_SOURCE})
add_executable(tcp_echo_client ${CLIENT_SOURCE} ${PLATFORM_SOURCES})
add_executable(server_tests tests/server_tests.c ${PLATFORM_SOURCES} ${MESSAGE_HANDLER_SOURCE})

if(MSVC)
    target_compile_options(tcp_echo_server PRIVATE /W4)
    target_compile_options(tcp_echo_client PRIVATE /W4)
    target_compile_options(server_tests PRIVATE /W4)
else()
    target_compile_options(tcp_echo_server PRIVATE -Wall -Wextra -pedantic -g)
    target_compile_options(tcp_echo_client PRIVATE -Wall -Wextra -pedantic -g)
    target_compile_options(server_tests PRIVATE -Wall -Wextra -pedantic -g)
endif()

if(UNIX)
    target_link_libraries(tcp_echo_server PRIVATE pthread)
    target_link_libraries(tcp_echo_client PRIVATE pthread)
    target_link_libraries(server_tests PRIVATE pthread)
endif()
