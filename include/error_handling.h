#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

/**
 * Prints an error message to stderr and terminates the program.
 *
 * This macro is used for handling fatal errors. It prints a formatted 
 * error message to the standard error stream, ensures that the output 
 * is flushed, and then exits the program with a status of 1.
 * 
 * Usage:
 *     on_error("Error: %s\n", errorMessage);
 * 
 * @param ... A format string followed by a variable number of arguments 
 *            (similar to printf), which allows for detailed error messages.
 */
#define on_error(...) { fprintf(stderr, __VA_ARGS__); fflush(stderr); exit(1); }

#endif // ERROR_HANDLING_H


