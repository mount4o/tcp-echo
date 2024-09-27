# TCP Echo Server

## Overview

This project is a cross-platform TCP Echo Server application that fulfills the following requirements:

- **TCP Echo Functionality**: The server accepts TCP connections, receives newline-terminated text messages, and sends them back to clients.
- **Client Tracking**: The server keeps track of the number of connected clients and the number of messages exchanged with each client.
- **Statistics Command**: Clients can request server statistics by sending a specific command, which returns the number of connected clients and the message count for each. The initial expected value for the command in question is "#stats" (null terminated at the end). The value can be changed in `defines.h`.
- **Cross-Platform Support**: The application is designed to run on both POSIX-compliant systems (Linux, macOS) and Windows.


## Project Structure

```plaintext
tcp_echo/
├── include/
│   ├── defines.h               # Constants and definitions used across the project
│   ├── error_handling.h        # Error handling macros
│   ├── msg_handling.h          # Message processing functions
│   ├── sockets.h               # Socket abstraction layer for cross-platform support
│   └── threads.h               # Thread abstraction layer for cross-platform support
├── src/
│   ├── client.c                # Utility client application implementation (POSIX only)
│   ├── server.c                # Server application implementation
│   ├── posix_sockets.c         # POSIX implementation of socket functions
│   ├── posix_threads.c         # POSIX implementation of threading functions
│   ├── win32_sockets.c         # Windows implementation of socket functions
│   ├── win32_threads.c         # Windows implementation of threading functions
│   └── msg_handling.c          # Implementation of message handling logic
├── tests/
│   └── server_tests.c          # Unit tests for server-side functionality
├── CMakeLists.txt              # CMake build configuration
└── README.md                   # Project documentation

## Build Instructions

### Prerequisites

- **CMake**: Ensure CMake is installed on your system. You can download it from [cmake.org](https://cmake.org).
- **Compiler**: A C compiler (e.g., GCC, Clang for POSIX systems, or MSVC for Windows).

### Building the Project

1. **Clone the Repository**:

    ```sh
    git clone <repository-url>
    cd tcp_echo
    ```

2. **Create a Build Directory and Navigate to It**:

    ```sh
    mkdir build
    cd build
    ```

3. **Configure the Project with CMake**:

    - **On Linux/macOS**:

        ```sh
        cmake ..
        ```

    - **On Windows**:

        ```sh
        cmake .. -G "MinGW Makefiles"  # or "Visual Studio" for MSVC
        ```

4. **Build the Project**:

    ```sh
    make  # On Windows with MSVC, use 'cmake --build .'
    ```

### Executables

After building the project, three main executables will be generated:

1. **`tcp_echo_server`**:
    - **Description**: This is the TCP Echo Server that listens for incoming connections from clients, echoes received messages, and handles statistics.
    - **Usage**:

        ```sh
        ./tcp_echo_server <port>
        ```

    - **Example**:

        ```sh
        ./tcp_echo_server 8080
        ```

2. **`tcp_echo_client`** (POSIX Only):
    - **Description**: A utility client application used to test the TCP Echo Server by sending messages to it and receiving the echoed responses.
    - **Usage**:

        ```sh
        ./tcp_echo_client <server_ip> <port>
        ```

    - **Example**:

        ```sh
        ./tcp_echo_client 127.0.0.1 8080
        ```

3. **`server_tests`**:
    - **Description**: A set of unit tests for the server-side functionality, including message processing and statistics handling.
    - **Usage**:

        ```sh
        ./server_tests
        ```

    - **Example**:

        ```sh
        ./server_tests
        ```

    - **Output**: The test results will be printed to the console, indicating whether all tests passed.

## Running Tests

Unit tests are provided to verify the functionality of the TCP Echo Server. Follow these steps to run the tests:

1. **Build the Tests**:

    ```sh
    make server_tests
    ```

2. **Run the Tests**:

    ```sh
    ./server_tests
    ```

3. **Check Results**: Ensure that all tests pass. The test output will indicate any issues that need to be addressed.

## Design Decisions

### Abstraction Layers

To ensure cross-platform compatibility, the project employs abstraction layers for sockets and threading:

- **Socket Abstraction**: Abstracts platform-specific socket operations, allowing seamless functionality on both POSIX and Windows platforms.
- **Thread Abstraction**: Similarly, threading operations are abstracted to ensure compatibility across different operating systems.

### Error Handling

The project uses consistent error handling through macros defined in `error_handling.h`. This approach centralizes error reporting and ensures consistent behavior across the application.

### Message Handling

The `msg_handling.c` module manages the logic for processing incoming messages and responding to clients, including handling the statistics command. This separation of concerns enhances modularity and testability.
