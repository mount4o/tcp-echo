#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

// Report error and exit
#define on_error(...) { fprintf(stderr, __VA_ARGS__); fflush(stderr); exit(1); }

#endif // ERROR_HANDLING_H


