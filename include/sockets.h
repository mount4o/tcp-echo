#ifndef SOCKETS_H
#define SOCKETS_H

#ifdef _WIN32 // Check for Windows
#include <winsock2.h>
#include <ws2tcpip.h>  // For modern socket functions like inet_pton, etc.
#pragma comment(lib, "ws2_32.lib")  // Link with Ws2_32.lib
#else
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

#include "error_handling.h"
#include <stdlib.h>

/**
 * Initializes a server socket for listening on the specified port.
 *
 * This function creates a socket, binds it to the specified port, 
 * and sets it up to listen for incoming connections. It also sets 
 * the socket options to allow address reuse.
 * 
 * On Windows, this function internally ensures that the Winsock library is initialized
 * by calling `WSAStartup` before proceeding with socket creation.
 *
 * @param port The port number on which the server will listen for incoming connections.
 * @return The server socket file descriptor on success, or -1 on error.
 */
int init_socket(int port);

/**
 * Accepts a new client connection on the given server socket.
 *
 * This function does not allocate memory for the file descriptor, 
 * and it does not verify if the connection was successfully established. 
 * It is the caller's responsibility to manage memory allocation for the 
 * client file descriptor and to handle the success or failure of the connection.
 *
 * On success, the client file descriptor will be stored in the memory location
 * pointed to by `client_fd_ptr`.
 * 
 * @param client_fd_ptr Pointer to an integer where the client file descriptor will be stored.
 * @param server_fd The server socket file descriptor.
 * @return void
 */
void accept_socket_conn(int* client_fd_ptr, int server_fd);

/**
 * Closes the client socket connection.
 *
 * This function closes the connection associated with the given client 
 * file descriptor and invalidates it by setting it to -1.
 *
 * On Windows, this function will internally call `closesocket` to close the socket,
 * while on POSIX systems it will call `close`.
 * 
 * @param client_fd_ptr Pointer to the client file descriptor to be closed.
 * @return void
 */
void close_socket_conn(int* client_fd_ptr);

/**
 * Receives a message from the client socket.
 *
 * This function reads data from the client socket into the provided buffer.
 * The buffer is not null-terminated, allowing for messages to be received 
 * across multiple packets. It is the caller's responsibility to handle 
 * the termination and processing of the buffer.
 *
 * On success, the number of bytes read will be returned. If the client has disconnected,
 * the function will return 0. On error, the function will return -1.
 * 
 * @param client_fd_ptr Pointer to the client file descriptor.
 * @param buf The buffer where the received message will be stored.
 * @param buf_size The size of the buffer.
 * @return The number of bytes read, or 0 if the client has disconnected, or -1 on error.
 */
int recv_msg(int* client_fd_ptr, char buf[], int buf_size);

/**
 * Closes the server socket and cleans up resources.
 *
 * This function closes the server socket associated with the given 
 * file descriptor and invalidates the descriptor by setting it to -1.
 *
 * On Windows, this function also ensures that the Winsock library is properly cleaned up
 * by calling `WSACleanup` if necessary.
 * 
 * @param server_fd Pointer to the server socket file descriptor to be closed.
 * @return void
 */
void close_socket(int* server_fd);

#endif // SOCKETS_H
