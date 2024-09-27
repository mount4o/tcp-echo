#include "sockets.h"
#include <stdio.h>

static struct sockaddr_in server, client;

int init_socket(int port) {
    int server_fd, err;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) on_error("Could not create socket\n");

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    int opt_val = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof opt_val);

    err = bind(server_fd, (struct sockaddr*)&server, sizeof(server));
    if (err < 0) on_error("Could not bind socket\n");

    err = listen(server_fd, 128);
    if (err < 0) on_error("Could not listen on socket\n");

    printf("Server is listening on %d\n", port);
    return server_fd;
}

void accept_socket_conn(int* client_fd_ptr, int server_fd) {
    socklen_t client_len = sizeof(client);
    *client_fd_ptr = accept(server_fd, (struct sockaddr*)&client, &client_len);
}

void close_socket_conn(int* client_fd_ptr) {
    if (client_fd_ptr && *client_fd_ptr >= 0) {
        close(*client_fd_ptr);
        *client_fd_ptr = -1; // Invalidate the file descriptor after closing
    }
}

int recv_msg(int* client_fd_ptr, char buf[], int buf_size) {
    return recv(*client_fd_ptr, buf, buf_size - 1, 0);
}

void close_socket(int* server_fd) {
    if (server_fd && *server_fd >= 0) {
        close(*server_fd);
        *server_fd = -1; // Invalidate the file descriptor after closing
    }
}
