#include "sockets.h"
#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

static struct sockaddr_in server, client;

int init_socket(int port) {
    int server_fd;
    WSADATA wsa_data;
    int err;

    // Initialize WinSock
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        on_error("WSAStartup failed\n");
    }

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == INVALID_SOCKET) {
        on_error("Could not create socket\n");
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    int opt_val = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt_val, sizeof(opt_val));

    // Bind socket
    err = bind(server_fd, (struct sockaddr*)&server, sizeof(server));
    if (err == SOCKET_ERROR) {
        on_error("Could not bind socket\n");
    }

    // Listen on socket
    err = listen(server_fd, 128);
    if (err == SOCKET_ERROR) {
        on_error("Could not listen on socket\n");
    }

    printf("Server is listening on %d\n", port);
    return server_fd;
}

void accept_socket_conn(int* client_fd_ptr, int server_fd) {
    int client_len = sizeof(client);
    *client_fd_ptr = accept(server_fd, (struct sockaddr*)&client, &client_len);
    if (*client_fd_ptr == INVALID_SOCKET) {
        // Make sure it's the same as what a POSIX socket would return, for unified interface purposes
        *client_fd_ptr = -1;
    }
}

void close_socket_conn(int* client_fd_ptr) {
    if (client_fd_ptr && *client_fd_ptr != INVALID_SOCKET) {
        closesocket(*client_fd_ptr);
        *client_fd_ptr = INVALID_SOCKET; // Invalidate the socket after closing
    }
}

int recv_msg(int* client_fd_ptr, char buf[], int buf_size) {
    int bytes_received = recv(*client_fd_ptr, buf, buf_size - 1, 0);
    if (bytes_received == SOCKET_ERROR) {
        // Make sure it's the same as what a POSIX socket would return, for unified interface purposes
        bytes_received = 0;
    }
    return bytes_received;
}

void close_socket(int* server_fd) {
    if (server_fd && *server_fd != INVALID_SOCKET) {
        closesocket(*server_fd);
        *server_fd = INVALID_SOCKET; // Invalidate the socket after closing
        WSACleanup(); // Clean up WinSock when done
    }
}
