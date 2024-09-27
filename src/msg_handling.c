#include "msg_handling.h"
#include "defines.h"
#include "threads.h"

#include <stdio.h>
#include <string.h>

// Generate status report upon specific command
static void
generate_user_stats_report(unsigned int user_id, int user_msg_count[], 
                           unsigned int num_connected_users, char* response) {
    unsigned int offset = 0;
    for (unsigned int i = 0; i <= user_id; i++) {
      // Disconnected clients will have -1 for message count, don't return stats for them
      if (user_msg_count[i] < 0) continue;

      offset += snprintf(response + offset, BUFFER_SIZE - offset, 
                         "\nUser ID: %u\nMessages exchanged: %u\n", i, user_msg_count[i]);

      // Ensure that we don't overflow the buffer
      if (offset >= BUFFER_SIZE) break;
    }

    snprintf(response + offset, BUFFER_SIZE - offset, 
             "\nTotal connected users: %u\n", num_connected_users);
}

// Cleanup local accumulation buffer
static void cleanup(char msg_buf[], int* msg_len) {
    *msg_len = 0;
    memset(msg_buf, 0, BUFFER_SIZE);
}

int process_msg(char* buf, int len, unsigned int user_id, int user_msg_count[], 
                    unsigned int num_connected_users, thread_mutex_t* user_stats_mutex,
                    char* response, int response_size) {
    static char message[BUFFER_SIZE];
    static int message_len = 0;

    for (int i = 0; i < len; i++) {
        if (buf[i] == '\n') {
            message[message_len] = '\0'; // Null-terminate the message

            // Check if the message is empty
            if (message_len == 0) {
                strncpy(response, "Empty message received", response_size - 1);
                response[response_size - 1] = '\0'; // Ensure null-termination
                return 1;
            }
            
            // If the user requested user stats
            if (strcmp(message, RETURN_MSG_COUNT_CMD) == 0) {
                generate_user_stats_report(user_id, user_msg_count,
                                           num_connected_users, response);
                cleanup(message, &message_len);
                return 1;
            }

            // Echo the received message back to the client
            strncpy(response, message, response_size - 1);
            response[response_size - 1] = '\0'; // Ensure null-termination

            // Increment message counter
            thread_mutex_lock(user_stats_mutex);
            user_msg_count[user_id]++;
            thread_mutex_unlock(user_stats_mutex);

            cleanup(message, &message_len);
            return 1;
        } else {
            // Accumulate characters into the message buffer
            if (message_len < BUFFER_SIZE - 1) {
                message[message_len++] = buf[i];
            } else {
                // Buffer overflow, truncating
                printf("Message too long, truncating.\n");

                cleanup(message, &message_len);
                return -1;
            }
        }
    }

    // Message not yet complete, keep what we have and expect more in upcoming packets
    return 0;
}

