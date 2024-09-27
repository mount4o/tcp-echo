#ifndef MSG_HANDLING_H
#define MSG_HANDLING_H
#include <string.h>
#include <stdio.h>
#include "defines.h"
#include "threads.h"

// Process new user message
int process_msg(char* buf, int len, unsigned int user_id, int user_msg_count[], 
                    unsigned int num_connected_users, thread_mutex_t* user_stats_mutex,
                    char* response, int response_size);
  
#endif // MSG_HANDLING_H

