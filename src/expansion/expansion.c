#include "expansion.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

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

static void expand_variable(char **str, char *expression,
                            struct hash_map *gv_hash_map)
{
    // printf("expand var: %s\n", expression);
    if (expression == NULL)
    {
        return;
    }

    if (strcmp(expression, "RANDOM") == 0)
    {
        /* If the expression is RANDOM, we generate a random number between 0
         * and 32767 and we update the memory with the new value. */
        fill_random(gv_hash_map);
    }

    /* Once the expression is created and filled properly, we look for the value
     * of the variable in the global variable hash map. If the variable is not
     * found, we set the value to the empty
     * string. Then, we append the value to the token_word string back. */

    char **value = memory_get(gv_hash_map, expression);

    if (value == NULL)
    {
        free(expression);
        return;
    }

    size_t i = 0;
    while (value[i] != NULL)
    {
        size_t j = 0;
        while (value[i][j] != '\0')
        {
            append_char_to_str(str, value[i][j]);
            j++;
        }

        if (value[i + 1] != NULL)
        {
            append_char_to_str(str, ' ');
        }
        i++;
    }

    free(expression);
}

static void param_expansion(char **str, struct stream_info *stream,
                            enum QUOTING_CONTEXT *context,
                            struct hash_map *gv_hash_map)
{
    if (*context == SINGLE_QUOTE)
    {
        // We are in a quote context, so we don't expand the dollar
        append_char_to_str(str, '$');
        stream_pop(stream);
        return;
    }

    // Consume the dollar
    stream_pop(stream);

    // Get the next character
    char next_char = stream_peek(stream);
    if (next_char == EOF)
    {
        return;
    }

    /* In this step, we create a new string (expression) that is the variable
     * identifier. If the next character is a '{', we consume the opening brace
     * and we read until we find a closing brace. Otherwise, we read until we
     * find a character that is not an alphanumeric character or an underscore
     * (because in this case expression is the longer valid name). All the
     * characters that forming the expression are removed from the token_word
     * string. */
    char *expression = NULL;
    if (next_char == '{')
    {
        // Consume the opening brace
        stream_pop(stream);
        while (1)
        {
            next_char = stream_peek(stream);
            if (next_char == EOF)
            {
                break;
            }

            if (next_char == '}')
            {
                stream_pop(stream);
                break;
            }

            append_char_to_str(&expression, next_char);
            stream_pop(stream);
        }
    }
    else
    {
        if (is_char_special_variable(next_char))
        {
            append_char_to_str(&expression, next_char);
            stream_pop(stream);
        }
        else if (isdigit(next_char))
        {
            append_char_to_str(&expression, next_char);
            stream_pop(stream);
        }
        else
        {
            while (1)
            {
                next_char = stream_peek(stream);
                if (next_char == EOF)
                {
                    break;
                }

                if (!isalnum(next_char) && next_char != '_')
                {
                    break;
                }

                append_char_to_str(&expression, next_char);
                stream_pop(stream);
            }
        }
    }

    expand_variable(str, expression, gv_hash_map);
}

static void expand_loop(struct stream_info *stream, char **str,
                        struct hash_map *gv_hash_map)
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

        if (cur_char == '$')
        {
            param_expansion(str, stream, &context, gv_hash_map);
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

char *expand_string(char **str, struct hash_map *gv_hash_map)
{
    if (*str == NULL)
    {
        return NULL;
    }

    char *expanded_str = NULL;

    int err = 0;
    struct stream_info *stream = stream_new(NULL, *str, &err);
    if (stream == NULL)
    {
        return NULL;
    }

    expand_loop(stream, &expanded_str, gv_hash_map);

    stream_free(stream);
    // free(*str);

    return expanded_str;
}
