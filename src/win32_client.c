#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include "sockets.h"
#include "defines.h"

#pragma comment(lib, "Ws2_32.lib")

void on_error(const char* msg) {
    fprintf(stderr, "%s: %d\n", msg, WSAGetLastError());
    exit(1);
}

int main(int argc, char* argv[]) {
    int port = PORT;

    if (argc < 2) {
        on_error("Usage: %s <server_ip> (optional - <port>)\n", argv[0]);
    }
    const char* server_ip = argv[1];

    if (argc > 2) port = atoi(argv[2]);

    WSADATA wsaData;
    int sock_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        on_error("WSAStartup failed");
    }

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == INVALID_SOCKET) {
        on_error("Could not create socket");
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        on_error("Invalid address or address not supported");
    }

    if (connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        on_error("Connection failed");
    }

    printf("Connected to %s on port %d\n", server_ip, port);

    while (1) {
        printf("Enter message: ");
        fgets(buffer, BUFFER_SIZE, stdin);

        if (send(sock_fd, buffer, (int)strlen(buffer), 0) == SOCKET_ERROR) {
            on_error("Send failed");
        }

        // Receive response from server
        int read_size = recv(sock_fd, buffer, BUFFER_SIZE, 0);
        if (read_size == SOCKET_ERROR) {
            on_error("Recv failed");
        } else if (read_size == 0) {
            printf("Server disconnected\n");
            break;
        }

        // Null-terminate the received data
        buffer[read_size] = '\0';
        printf("Server response: %s\n", buffer);
    }

    closesocket(sock_fd);
    WSACleanup();

    return 0;
}
