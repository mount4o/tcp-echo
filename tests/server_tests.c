#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "defines.h"
#include "threads.h"
#include "msg_handling.h"

int process_msg(char* buf, int len, unsigned int user_id, int user_msg_count[], 
                    unsigned int num_connected_users, thread_mutex_t* user_stats_mutex, 
                    char* response, int response_size);

void test_message_processing(void) {
    char buffer[BUFFER_SIZE];
    char response[BUFFER_SIZE];
    unsigned int num_connected_users = 1;  // Assume at least 1 user connected for simplicity
    thread_mutex_t user_stats_mutex;
    thread_mutex_init(&user_stats_mutex);

    int user_msg_count[MAX_USERS] = {0};
    unsigned int user_id = 0;

    // Test normal message
    strcpy(buffer, "Hello, World!\n");
    int result = process_msg(buffer, strlen(buffer), user_id, user_msg_count, num_connected_users, &user_stats_mutex, response, sizeof(response));
    assert(result == 1);
    assert(strcmp(response, "Hello, World!") == 0);
    assert(user_msg_count[user_id] == 1);

    // Test #statistics command
    sprintf(buffer, "%s\n", RETURN_MSG_COUNT_CMD);
    result = process_msg(buffer, strlen(buffer), user_id, user_msg_count, num_connected_users, &user_stats_mutex, response, sizeof(response));
    assert(result == 1);
    assert(strstr(response, "User ID: 0") != NULL);
    assert(strstr(response, "Messages exchanged: 1") != NULL);
    assert(strstr(response, "Total connected users: 1") != NULL);

    // Test partial message
    strcpy(buffer, "Partial");
    result = process_msg(buffer, strlen(buffer), user_id, user_msg_count, num_connected_users, &user_stats_mutex, response, sizeof(response));
    assert(result == 0); // Message not complete

    // Test completion of the partial message
    strcpy(buffer, " message\n");
    result = process_msg(buffer, strlen(buffer), user_id, user_msg_count, num_connected_users, &user_stats_mutex, response, sizeof(response));
    assert(result == 1);
    assert(strcmp(response, "Partial message") == 0);
    assert(user_msg_count[user_id] == 2);

    // Cleanup
    thread_mutex_destroy(&user_stats_mutex);
}

int main(void) {
    test_message_processing();
    printf("All tests passed.\n");
    return 0;
}
