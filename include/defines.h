#ifndef DEFINES_H
#define DEFINES_H

#define BUFFER_SIZE           1024       // Size of the buffer that stores incoming/outgoing TCP packets
#define PORT                  122        // Port on which the TCP server listens to
#define RETURN_MSG_COUNT_CMD  "#stats"   // The command that returns the count of exchanged messages between client and server
#define MAX_USERS             100          // Max number of clients the server can handle

#endif // DEFINES_H
