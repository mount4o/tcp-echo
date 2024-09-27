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

int init_socket(int port);

/*
 * This function doesn't allocate the file descriptor and doesn't check if a connection is
 * established, that's left to the user.
 * @return client file descriptor OR -1 on error
 */
void accept_socket_conn(int* client_fd_ptr, int server_fd);

void close_socket_conn(int* client_fd_ptr);

/*
 * The function doesn't null terminate the string to allow for receival
 * of a message over multiple packets.
 * @return num of bytes read OR 0 on client disconnect or -1 on read error
 */
int recv_msg(int* client_fd_ptr, char buf[], int buf_size);

void close_socket(int* server_fd);

#endif // SOCKETS_H
