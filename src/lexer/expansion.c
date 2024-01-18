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

static void handle_backslash_quote(char **str, struct stream_info *stream,
                                   enum QUOTING_CONTEXT *context)
{
    if (*context == NONE)
    {
        // Discard the backslash
        stream_pop(stream);

        // Get the character after the backslash
        char next_char = stream_peek(stream);
        if (next_char == EOF)
        {
            return;
        }

        // Append the character to the string
        append_char_to_str(str, next_char);
        stream_pop(stream);
    }
    else
    {
        switch (*context)
        {
        case SINGLE_QUOTE:
            append_char_to_str(str, '\\');
            stream_pop(stream);
            break;
        case DOUBLE_QUOTE:
            stream_pop(stream);
            char next_char = stream_peek(stream);
            if (next_char == EOF)
            {
                return;
            }
            if (next_char == '$' || next_char == '`' || next_char == '"'
                || next_char == '\\')
            {
                append_char_to_str(str, next_char);
            }
            else
            {
                append_char_to_str(str, '\\');
                append_char_to_str(str, next_char);
            }
            stream_pop(stream);
            break;
        default:
            // Should never happen
            break;
        }
    }
}

static void handle_single_quote(char **str, struct stream_info *stream,
                                enum QUOTING_CONTEXT *context)
{
    if (*context == NONE)
    {
        *context = SINGLE_QUOTE;
    }
    else if (*context == SINGLE_QUOTE)
    {
        *context = NONE;
    }
    else
    {
        // That means where are in a double quote context
        append_char_to_str(str, '\'');
    }
    stream_pop(stream);
}

static void handle_double_quote(char **str, struct stream_info *stream,
                                enum QUOTING_CONTEXT *context)
{
    if (*context == NONE)
    {
        *context = DOUBLE_QUOTE;
    }
    else if (*context == DOUBLE_QUOTE)
    {
        *context = NONE;
    }
    else
    {
        // That means where are in a single quote context
        append_char_to_str(str, '"');
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

        if (cur_char == '\\')
        {
            handle_backslash_quote(str, stream, &context);
            continue;
        }

        if (cur_char == '\'')
        {
            handle_single_quote(str, stream, &context);
            continue;
        }

        if (cur_char == '"')
        {
            handle_double_quote(str, stream, &context);
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
