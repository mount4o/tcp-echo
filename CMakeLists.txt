cmake_minimum_required(VERSION 3.10)

# Set the project name
project(tcp_echo VERSION 1.0 LANGUAGES C)

# Specify the C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# Set the build type to Debug to include debug information
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()

# Set the include directory
include_directories("${PROJECT_SOURCE_DIR}/include")

# Determine the platform and include the appropriate source files
if(WIN32)
    set(PLATFORM_SOURCES src/win32_sockets.c src/win32_threads.c)
    set(THREAD_LIBS ws2_32)
else()
    set(PLATFORM_SOURCES src/posix_sockets.c src/posix_threads.c)
    set(THREAD_LIBS pthread)
endif()

# Add the message handler source file
set(MESSAGE_HANDLER_SOURCE src/msg_handling.c)

# Add the server executable target
add_executable(tcp_echo_server src/server.c ${PLATFORM_SOURCES} ${MESSAGE_HANDLER_SOURCE})

# Add the client executable target
add_executable(tcp_echo_client src/client.c ${PLATFORM_SOURCES})

# Add the unit tests executable target
add_executable(server_tests tests/server_tests.c ${PLATFORM_SOURCES} ${MESSAGE_HANDLER_SOURCE})

# Specify compile options including debug symbols
target_compile_options(tcp_echo_server PRIVATE -Wall -Wextra -pedantic -g)
target_compile_options(tcp_echo_client PRIVATE -Wall -Wextra -pedantic -g)
target_compile_options(server_tests PRIVATE -Wall -Wextra -pedantic -g)

# Link against the required threading library based on platform
if(UNIX)
    target_link_libraries(tcp_echo_server PRIVATE pthread)
    target_link_libraries(server_tests PRIVATE pthread)
elseif(WIN32)
    target_link_libraries(tcp_echo_server PRIVATE ws2_32)
    target_link_libraries(server_tests PRIVATE ws2_32)
endif()
