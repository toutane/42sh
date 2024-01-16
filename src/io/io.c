#define _POSIX_C_SOURCE 200809L

#include "io.h"

#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct stream_info *get_stream(int argc, struct options *opts, int *err)
{
    if (argc < 0 || opts == NULL || err == NULL)
    {
        fprintf(stderr, "42sh: get_stream: invalid arguments\n");
        *err = 1;
        return NULL;
    }

    // Input from string (with '-c' option)
    if (opts->command)
    {
        if (opts->input == NULL)
        {
            fprintf(stderr, "42sh: -c: option requires an argument\n");
            *err = 2;
            return NULL;
        }

        if (opts->verbose)
        {
            printf("Input from string: \"%s\"\n", opts->input);
        }

        return stream_new(NULL, opts->input, err);
    }

    // Input from stdin
    if (opts->input == NULL)
    {
        if (opts->verbose)
        {
            printf("Input from stdin\n");
        }
        return stream_new(NULL, NULL, err);
    }

    // Input from file
    if (opts->verbose)
    {
        printf("Input from file: \"%s\"\n", opts->input);
    }
    return stream_new(opts->input, NULL, err);
}

struct stream_info *stream_new(const char *path, char *buf, int *err)
{
    // maybe useless to set err value to 0, allready made in main func
    //*err = 0;
    struct stream_info *stream = calloc(1, sizeof(struct stream_info));
    if (stream == NULL)
    {
        fprintf(stderr, "42sh: stream_new: calloc failed\n");
        *err = 1;
        return NULL;
    }

    // Input from string (with '-c' option)
    if (path == NULL && buf != NULL)
    {
        size_t len = strlen(buf);
        if (len == 0) // Empty string passed to '-c' option
        {
            *err = 0; // Not an error, but we need to return NULL
            free(stream);
            return NULL;
        }

        stream->fp = fmemopen(buf, len, "r");
        if (stream->fp == NULL)
        {
            fprintf(stderr, "42sh: stream_new: fmemopen failed\n");
            *err = 1;
            free(stream);
            return NULL;
        }
        return stream;
    }

    // Input from file
    if (path != NULL && buf == NULL)
    {
        stream->fp = fopen(path, "r");
        if (stream->fp == NULL)
        {
            fprintf(stderr, "42sh: %s: %s\n", path, strerror(errno));
            *err = 1;
            free(stream);
            return NULL;
        }
        return stream;
    }

    // Input from stdin
    if (path == NULL && buf == NULL)
    {
        stream->fp = stdin;
        return stream;
    }

    // Should never reach here
    free(stream);
    return NULL;
}

void stream_free(struct stream_info *stream)
{
    if (stream == NULL)
    {
        fprintf(stderr, "42sh: stream_free: stream is NULL\n");
        return;
    }

    if (stream->fp != stdin)
    {
        fclose(stream->fp);
    }
    free(stream);
}

int stream_peek(struct stream_info *stream)
{
    if (stream == NULL)
    {
        fprintf(stderr, "42sh: stream_peek: stream is NULL\n");
        return -1;
    }

    int c = fgetc(stream->fp);
    if (c == EOF)
    {
        return EOF;
    }

    ungetc(c, stream->fp);
    return c;
}

int stream_pop(struct stream_info *stream)
{
    if (stream == NULL)
    {
        fprintf(stderr, "42sh: stream_pop: stream is NULL\n");
        return -1;
    }

    return fgetc(stream->fp);
}
