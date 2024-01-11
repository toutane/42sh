#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "io_backend.h"

struct stream_info *get_stream(int argc, char *argv[], struct options *opts)
{
	// Input from string (with '-c' option)
	if (opts->command)
	{
		if (argc < 3)
		{
			fprintf(stderr, "42sh: -c: option requires an argument\n");
			return NULL;
		}
		return stream_new(NULL, argv[2]);
	}

	// Input from stdin
	if (argc < 2)
	{
		return stream_new(NULL, NULL);
	}

	// Input from file
	return stream_new(argv[1], NULL);
}

struct stream_info *stream_new(const char *path, char *buf)
{
	struct stream_info *stream = calloc(1, sizeof(struct stream_info));
	if (stream == NULL)
	{
		fprintf(stderr, "stream_new: calloc failed\n");
		return NULL;
	}

	// Input from string (with '-c' option)
	if (path == NULL && buf != NULL)
	{
		stream->fp = fmemopen(buf, strlen(buf), "r");
		if (stream->fp == NULL)
		{
			fprintf(stderr, "stream_new: fmemopen failed\n");
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
			fprintf(stderr, "stream_new: fopen failed\n");
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

	return NULL;
}

void stream_free(struct stream_info *stream)
{
	if (stream == NULL)
	{
		fprintf(stderr, "stream_free: stream is NULL\n");
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
		fprintf(stderr, "stream_peek: stream is NULL\n");
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
		fprintf(stderr, "stream_pop: stream is NULL\n");
		return -1;
	}

	return fgetc(stream->fp);
}
