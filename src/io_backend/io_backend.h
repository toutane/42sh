#ifndef IO_BACKEND_H
#define IO_BACKEND_H

#include <stdio.h>

#include "../options/opt_parser.h"

struct stream_info
{
    FILE *fp;
};

/*
 * @brief Get a stream_info object from the command line arguments
 */
struct stream_info *get_stream(int argc, char *argv[], struct options *opts,
                               int *err);

/*
 * @brief Create a new stream_info object
 *
 * If the input mode is stdin, the path must be set to null, as the buffer
 * If the input mode is file, the path must be set to the path of the file, and
 * the buffer to null If the input is a string, the path must be set to null,
 * and the buffer to the string
 */
struct stream_info *stream_new(const char *path, char *buf, int *err);

/*
 * @brief Free a stream_info object
 * @param stream The stream_info object to free
 */
void stream_free(struct stream_info *stream);

/*
 * @brief Read a character from a stream
 */
int stream_peek(struct stream_info *stream);

/*
 * @brief Read a character from a stream
 */
int stream_pop(struct stream_info *stream);

#endif /* ! IO_BACKEND_H */
