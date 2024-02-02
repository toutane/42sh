#define _XOPEN_SOURCE 500
#define _POSIX_SOURCE

#include "expansion.h"

#include <ctype.h>
#include <pwd.h>
#include <stdlib.h>
#include <string.h>

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
        append_char_to_string(str, next_char);
        stream_pop(stream);
    }
    else
    {
        switch (*context)
        {
        case SINGLE_QUOTE:
            append_char_to_string(str, '\\');
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
                || next_char == '\\' || next_char == '\n')
            {
                append_char_to_string(str, next_char);
            }
            else
            {
                append_char_to_string(str, '\\');
                append_char_to_string(str, next_char);
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
        // if noting in quote append an empty string
        *context = NONE;
    }
    else
    {
        // That means where are in a double quote context
        append_char_to_string(str, '\'');
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
        // if noting in quote append an empty string
        *context = NONE;
    }
    else
    {
        // That means where are in a single quote context
        append_char_to_string(str, '"');
    }
    stream_pop(stream);
}

static void expand_variable(char **str, char *expression, struct hm *hm_var)
{
    if (expression == NULL)
    {
        return;
    }

    if (strcmp(expression, "RANDOM") == 0)
    {
        /* If the expression is RANDOM, we generate a random number between 0
         * and 32767 and we update the memory with the new value. */
        set_random(hm_var);
    }

    /* Once the expression is created and filled properly, we look for the value
     * of the variable in the global variable hash map. If the variable is not
     * found, we set the value to the empty
     * string. Then, we append the value to the token_word string back. */

    char *value = get_variable(expression, hm_var);

    if (value == NULL)
    {
        free(expression);
        return;
    }
    my_strcat(str, value);

    free(expression);
}

static void recognize_expr_in_braces(char next_char, char **expression,
                                     struct stream_info *stream)
{
    // Consume the opening brace
    stream_pop(stream);

    /* In this step, we read until we find a closing brace. All the
     * characters that forming the expression are removed from the token_word
     * string. */

    while (1)
    {
        next_char = stream_peek(stream);
        if (next_char == EOF)
        {
            return;
        }

        if (next_char == '}')
        {
            stream_pop(stream);
            return;
        }

        append_char_to_string(expression, next_char);
        stream_pop(stream);
    }
}

static void recognize_expr(char next_char, char **expression,
                           struct stream_info *stream)
{
    /* In this step, we read until we find a character that is not an
     * alphanumeric character or an underscore (because in this case expression
     * is the longer valid name). All the characters that forming the expression
     * are removed from the token_word string. */

    if (is_char_special_variable(next_char))
    {
        append_char_to_string(expression, next_char);
        stream_pop(stream);
    }
    else if (isdigit(next_char))
    {
        append_char_to_string(expression, next_char);
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

            append_char_to_string(expression, next_char);
            stream_pop(stream);
        }
    }
}

static void handle_dollar(char **str, struct stream_info *stream,
                          enum QUOTING_CONTEXT *context, struct mem *mem)
{
    if (*context == SINGLE_QUOTE)
    {
        // We are in a quote context, so we don't expand the dollar
        append_char_to_string(str, '$');
        stream_pop(stream);
        return;
    }

    // Consume the dollar
    stream_pop(stream);

    // Get the next character
    char next_char = stream_peek(stream);
    if ((next_char == EOF || !is_char_valid_in_name(next_char))
        && (next_char != '(' && next_char != ')'))
    {
        append_char_to_string(str, '$');
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
    switch (next_char)
    {
    case '{':
        recognize_expr_in_braces(next_char, &expression, stream);
        break;
    case '(':
        command_substitution(str, stream, mem);
        return;
    default:
        recognize_expr(next_char, &expression, stream);
        break;
    }

    expand_variable(str, expression, mem->hm_var);
}

static void handle_tilde(char **str, struct stream_info *stream,
                         struct mem *mem)
{
    // Consume the tild
    stream_pop(stream);

    // Get HOME variable
    char *home = get_variable("HOME", mem->hm_var);

    // Get the expression
    char next_char = stream_peek(stream);
    char *expression = NULL;
    recognize_expr(next_char, &expression, stream);

    // Expande home variable
    if (expression == NULL || expression[0] == '/')
    {
        my_strcat(str, home);
        my_strcat(str, expression);
        return;
    }

    // Try to get home from expression
    struct passwd *p;
    if ((p = getpwnam(expression)) == NULL)
    {
        my_strcat(str, "~");
        my_strcat(str, expression);
        return;
    }

    my_strcat(str, p->pw_dir);
    free(expression);
    return;
}

static void expand_loop(struct stream_info *stream, char **str, struct mem *mem)
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
            handle_dollar(str, stream, &context, mem);
            continue;
        }

        if (context == NONE && cur_char == '~')
        {
            handle_tilde(str, stream, mem);
            continue;
        }

        if (context != SINGLE_QUOTE && cur_char == '`')
        {
            command_substitution(str, stream, mem);
            continue;
        }

        append_char_to_string(str, cur_char);
        stream_pop(stream);
    }

    if (*str == NULL)
    {
        *str = strdup("");
    }
}

char *expand_string(char **str, struct mem *mem)
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

    expand_loop(stream, &expanded_str, mem);

    stream_free(stream);
    // free(*str);

    return expanded_str;
}

static char **field_split(char **expanded_argv, int *expanded_argc,
                          char *temp_str, struct hm *hm_var)
{
    if (!temp_str)
    {
        return expanded_argv;
    }

    // Field split delimiteur
    char *delimiters = get_variable("IFS", hm_var);

    // concat word to last item of expanded_argv
    char *word = strtok(temp_str, delimiters);
    my_strcat(&(expanded_argv[*expanded_argc]), word);
    word = strtok(NULL, delimiters);

    while (word != NULL)
    {
        // Then realloc array, put NULL in last item
        (*expanded_argc)++;
        expanded_argv =
            realloc(expanded_argv, (*expanded_argc + 1) * sizeof(char *));
        expanded_argv[*expanded_argc] = NULL;

        my_strcat(&(expanded_argv[*expanded_argc]), word);
        word = strtok(NULL, delimiters);
    }

    return expanded_argv;
}

static int handle_quotes(char **expanded_argv, int *expanded_argc,
                         struct stream_info *stream,
                         enum QUOTING_CONTEXT *context)
{
    char cur_char = stream_peek(stream);

    if (cur_char == '\\')
    {
        handle_backslash_quote(&(expanded_argv[*expanded_argc]), stream,
                               context);
        return 1;
    }

    if (cur_char == '\'')
    {
        my_strcat(&(expanded_argv[*expanded_argc]), "");
        handle_single_quote(&(expanded_argv[*expanded_argc]), stream, context);
        return 1;
    }

    if (cur_char == '"')
    {
        my_strcat(&(expanded_argv[*expanded_argc]), "");
        handle_double_quote(&(expanded_argv[*expanded_argc]), stream, context);
        return 1;
    }

    return 0;
}

static char **word_expansions(char **expanded_argv, int *expanded_argc,
                              struct stream_info *stream, struct mem *mem)
{
    char *temp_str = NULL;

    enum QUOTING_CONTEXT context = NONE;

    while (1)
    {
        char cur_char = stream_peek(stream);
        if (cur_char == EOF)
        {
            break;
        }

        if (handle_quotes(expanded_argv, expanded_argc, stream, &context))
        {
            continue;
        }

        // tild expansion
        if (context == NONE && cur_char == '~')
        {
            handle_tilde(&temp_str, stream, mem);
            my_strcat(&expanded_argv[*expanded_argc], temp_str);

            free(temp_str);
            temp_str = NULL;
            continue;
        }

        if (cur_char == '$')
        {
            handle_dollar(&temp_str, stream, &context, mem);
            if (context == NONE)
            {
                expanded_argv = field_split(expanded_argv, expanded_argc,
                                            temp_str, mem->hm_var);
            }
            else
            {
                my_strcat(&expanded_argv[*expanded_argc], temp_str);
            }

            free(temp_str);
            temp_str = NULL;
            continue;
        }

        if (context != SINGLE_QUOTE && cur_char == '`')
        {
            command_substitution(&temp_str, stream, mem);
            if (context == NONE)
            {
                expanded_argv = field_split(expanded_argv, expanded_argc,
                                            temp_str, mem->hm_var);
            }
            else
            {
                my_strcat(&expanded_argv[*expanded_argc], temp_str);
            }

            free(temp_str);
            temp_str = NULL;
            continue;
        }

        append_char_to_string(&(expanded_argv[*expanded_argc]), cur_char);
        stream_pop(stream);
    }

    return expanded_argv;
}

char **argv_expansions(char **original_argv, int *argc, struct mem *mem)
{
    int expanded_argc = 0;
    char **expanded_argv = calloc(expanded_argc + 1, sizeof(char *));

    for (int i = 0; i < *argc; i++)
    {
        int err = 0;
        if (original_argv[i][0] == '\0')
        {
            continue;
        }

        struct stream_info *stream = stream_new(NULL, original_argv[i], &err);
        if (stream == NULL)
        {
            fprintf(stderr,
                    "42sh: argv_expansions failed to create new stream\n");
            _exit(EXIT_FAILURE);
        }

        expanded_argv =
            word_expansions(expanded_argv, &expanded_argc, stream, mem);

        // Append null char if nothing
        if (expanded_argv[expanded_argc])
        {
            (expanded_argc)++;
            expanded_argv =
                realloc(expanded_argv, (expanded_argc + 1) * sizeof(char *));
            expanded_argv[expanded_argc] = NULL;
        }

        stream_free(stream);
    }

    expanded_argv =
        realloc(expanded_argv, (expanded_argc + 1) * sizeof(char *));
    expanded_argv[expanded_argc] = NULL;

    *argc = expanded_argc;

    return expanded_argv;
}
