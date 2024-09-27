#include "sockets.h"
#include "msg_handling.h"
#include "error_handling.h"
#include "threads.h"
#include "defines.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int user_msg_count[MAX_USERS];
static unsigned int next_user_id = 0;
static unsigned int num_connected_users = 0;
static thread_mutex_t user_stats_mutex;

void* handle_connection(void* client_fd_ptr) {
    thread_mutex_lock(&user_stats_mutex);

    // Generate a new user ID
    unsigned int user_id = next_user_id++;

    user_msg_count[user_id] = 0;
    num_connected_users++;

    thread_mutex_unlock(&user_stats_mutex);

    int client_fd = *(int*)client_fd_ptr;
    free(client_fd_ptr);

    char buf[BUFFER_SIZE];
    char response[BUFFER_SIZE];

    while (1) {
        int read = recv(client_fd, buf, BUFFER_SIZE, 0);
        if (read <= 0) break; // Client disconnected or error occurred

        if (process_msg(buf, read, user_id, user_msg_count, num_connected_users, &user_stats_mutex, response, sizeof(response))) {
            send(client_fd, response, strlen(response), 0);
        }
    }

    thread_mutex_lock(&user_stats_mutex);
    num_connected_users--;
    user_msg_count[user_id] = -1;
    thread_mutex_unlock(&user_stats_mutex);

    close(client_fd);
    return NULL;
}

int main(int argc, char *argv[]) {
    int port = PORT;
    if (argc > 1) port = atoi(argv[1]);

    thread_mutex_init(&user_stats_mutex);

    int server_fd = init_socket(port);

    while (1) {
        int* client_fd_ptr = malloc(sizeof(int));
        if (client_fd_ptr == NULL) {
            on_error("Memory allocation failed\n");
        }

        accept_socket_conn(client_fd_ptr, server_fd);

        if (*client_fd_ptr < 0) {
            free(client_fd_ptr);
            on_error("Could not establish new connection\n");
        }

        thread_t thread;
        if (create_thread(&thread, handle_connection, client_fd_ptr) != 0) {
            free(client_fd_ptr);
            on_error("Could not create thread\n");
        }

        if (detach_thread(&thread) != 0) {
            on_error("Could not detach thread\n");
        }
    }

    close_socket(&server_fd);
    thread_mutex_destroy(&user_stats_mutex);

    return 0;
}
