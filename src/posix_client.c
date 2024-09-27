#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "sockets.h"
#include "defines.h"

int main(int argc, char *argv[]) {
    int port = PORT;

    if (argc < 2) {
        on_error("Usage: %s <server_ip> (optional - <port>)\n", argv[0]);
    }
    const char *server_ip = argv[1];

    if (argc > 2) port = atoi(argv[2]);

    int sock_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Create socket
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) on_error("Could not create socket");

    // Set server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        on_error("Invalid address or address not supported");
    }

    // Connect to the server
    if (connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        on_error("Connection failed");
    }

    printf("Connected to %s on port %d\n", server_ip, port);

    while (1) {
        printf("Enter message: ");
        fgets(buffer, BUFFER_SIZE, stdin);

        // Send message to server
        if (send(sock_fd, buffer, strlen(buffer), 0) < 0) {
            on_error("Send failed");
        }

        // Receive response from server
        int read_size = recv(sock_fd, buffer, BUFFER_SIZE, 0);
        if (read_size < 0) {
            on_error("Recv failed");
        } else if (read_size == 0) {
            printf("Server disconnected\n");
            break;
        }

        // Null-terminate the received data
        buffer[read_size] = '\0';
        printf("Server response: %s\n", buffer);
    }

    // Close the socket
    close(sock_fd);

    return 0;
}
