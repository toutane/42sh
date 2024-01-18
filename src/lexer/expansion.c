#include <stdlib.h>
#include <string.h>

#include "io/io.h"
#include "lexer.h"

static void append_char_to_str(char **str, char c)
{
    if (*str == NULL)
    {
        *str = calloc(2, sizeof(char));
        (*str)[0] = c;
        return;
    }

    size_t len = strlen(*str);
    *str = realloc(*str, (len + 2) * sizeof(char));
    (*str)[len] = c;
    (*str)[len + 1] = '\0';
}

static void handle_single_quote(char **str, struct stream_info *stream,
                                enum QUOTING_CONTEXT *context)
{
    if (*context == NONE)
    {
        if (*context == DOUBLE_QUOTE)
        {
            append_char_to_str(str, '\'');
        }
        else
        {
            *context = SINGLE_QUOTE;
        }
    }
    else if (*context == SINGLE_QUOTE)
    {
        *context = NONE;
    }
    stream_pop(stream);
}

static void expand_loop(struct stream_info *stream, char **str)
{
    enum QUOTING_CONTEXT context = NONE;

    while (1)
    {
        char cur_char = stream_peek(stream);
        if (cur_char == EOF)
        {
            break;
        }

        if (cur_char == '\'')
        {
            handle_single_quote(str, stream, &context);
            continue;
        }

        append_char_to_str(str, cur_char);
        stream_pop(stream);
    }

    // TODO: fix this wird behaviour that makes the string null
    if (*str == NULL)
    {
        *str = calloc(2, sizeof(char));
        (*str)[0] = '\0';
    }
}

void expand_quoting(char **str)
{
    if (*str == NULL)
    {
        return;
    }

    char *expanded_str = NULL;

    int err = 0;
    struct stream_info *stream = stream_new(NULL, *str, &err);
    if (stream == NULL)
    {
        return;
    }

    expand_loop(stream, &expanded_str);

    stream_free(stream);
    free(*str);

    *str = expanded_str;
}
