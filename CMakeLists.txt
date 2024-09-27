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

# Add the client executable target (POSIX only)
if(NOT WIN32)
    add_executable(tcp_echo_client src/client.c ${PLATFORM_SOURCES})
endif()

# Add the unit tests executable target
add_executable(server_tests tests/server_tests.c ${PLATFORM_SOURCES} ${MESSAGE_HANDLER_SOURCE})

# Specify compile options including debug symbols
if(MSVC)
    # Use appropriate warning level for MSVC
    target_compile_options(tcp_echo_server PRIVATE /W4)
    target_compile_options(server_tests PRIVATE /W4)
    if(NOT WIN32)
        target_compile_options(tcp_echo_client PRIVATE /W4)
    endif()
else()
    # Use GCC/Clang specific warnings
    target_compile_options(tcp_echo_server PRIVATE -Wall -Wextra -pedantic -g)
    target_compile_options(server_tests PRIVATE -Wall -Wextra -pedantic -g)
    if(NOT WIN32)
        target_compile_options(tcp_echo_client PRIVATE -Wall -Wextra -pedantic -g)
    endif()
endif()

# Link against the pthread library if required (for POSIX systems)
if(UNIX)
    target_link_libraries(tcp_echo_server PRIVATE pthread)
    target_link_libraries(server_tests PRIVATE pthread)
endif()
