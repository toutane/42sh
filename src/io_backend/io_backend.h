#ifndef IO_BACKEND_H
#define IO_BACKEND_H

#include <stdio.h>

struct stream_info {
	FILE *fp;
};

/*
 * @brief Create a new stream_info object
 * @param path The path to the file to open if any
 * @return A pointer to the newly created stream_info object
 *
 * If the input mode is stdin, the path must be set to null, as the buffer
 * If the input mode is file, the path must be set to the path of the file, and the buffer to null
 * If the input is a string, the path must be set to null, and the buffer to the string
 */
struct stream_info *stream_new(const char *path, char *buf);

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
