#ifndef MSG_HANDLING_H
#define MSG_HANDLING_H

#include <string.h>
#include <stdio.h>
#include "defines.h"
#include "threads.h"

/**
 * Processes a new message from a user and generates a response.
 *
 * This function handles incoming messages from clients, processes them, 
 * and generates appropriate responses based on the content of the message. 
 * It supports commands for generating user statistics and echoes other messages back to the client.
 * 
 * The function also increments the number of messages exchanged by each user. 
 * Mutex lock is used to protect shared data from race conditions when accessed
 * by multiple threads.
 * 
 * @param buf A buffer containing the message received from the client.
 * @param len The length of the message in the buffer.
 * @param user_id The ID of the user sending the message.
 * @param user_msg_count An array tracking the number of messages exchanged by each user.
 * @param num_connected_users The total number of users currently connected to the server.
 * @param user_stats_mutex A mutex used to protect the shared user statistics data.
 * @param response A buffer where the generated response message will be stored.
 * @param response_size The size of the response buffer.
 * @return 1 if a complete message was processed and a response is ready to be sent, 
 *         0 if the message is incomplete and more data is expected, or 
 *        -1 if an error occurred (e.g., buffer overflow).
 */
int process_msg(char* buf, int len, unsigned int user_id, int user_msg_count[], 
                unsigned int num_connected_users, thread_mutex_t* user_stats_mutex,
                char* response, int response_size);

#endif // MSG_HANDLING_H
