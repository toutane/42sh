#include "lexer.h"

#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PRINT_TOKEN(verbose, tok, action)                                      \
    if (verbose)                                                               \
    {                                                                          \
        printf("[LEXER] " action " token: %s\n", token_type_to_str(tok.type)); \
    }

struct LEXER_ERROR_MSG_INFO
{
    enum LEXER_ERROR error;
    char *msg;
};

struct LEXER_ERROR_MSG_INFO lexer_error_msg_map[] = {
    { NO_ERROR, "no error" },
    { UNMATCHED_SINGLE_QUOTE, "unmatched single quote" },
    { UNMATCHED_DOUBLE_QUOTE, "unexpected EOF while looking for matching \"" },
    { UNMATCHED_BRACE, "unmatched brace" },
    { BAD_SUBSTITUTION, "bad substitution" },
};

char *get_lexer_error_msg(enum LEXER_ERROR error)
{
    for (size_t i = 0;
         i < sizeof(lexer_error_msg_map) / sizeof(lexer_error_msg_map[0]); i++)
    {
        if (error == lexer_error_msg_map[i].error)
        {
            return lexer_error_msg_map[i].msg;
        }
    }

    return "Unknown error";
}

void fill_token(struct token *tok, enum token_type type, char *value)
{
    if (tok == NULL)
    {
        fprintf(stderr, "fill_token: tok is NULL\n");
        return;
    }

    tok->type = type;
    tok->value = value;
}

/*
 * @brief: Set the previous char to the last character of the current token
 * value, and len to the current token value length.
 */
static void set_len_and_prev_char(struct lexer *lexer, char *prev_char,
                                  size_t *len)
{
    *len = 0;
    if (lexer->cur_tok.type != TOKEN_NONE && lexer->cur_tok.value != NULL)
    {
        *len = strlen(lexer->cur_tok.value);
    }

    *prev_char = '\0';
    if (lexer->cur_tok.type != TOKEN_NONE && *len > 0)
    {
        *prev_char = lexer->cur_tok.value[*len - 1];
    }
}

/*
 * @brief: Return 1 if the given char is a delimiter, 0 otherwise.
 */
static int is_delimiter(char c)
{
    return isblank(c) || c == '\n' || c == ';';
}

/*
 * @brief: Append the given character to current token value and
 * consume it.
 */
static void append_consume(struct lexer *lexer, char c)
{
    append_char_to_token_value(&lexer->cur_tok, c);
    stream_pop(lexer->stream);
}

/*
 * @brief: Set the current token type to the given type and append
 * the given character its value.
 */
static void set_append_consume(struct lexer *lexer, enum token_type type,
                               char c)
{
    fill_token(&lexer->cur_tok, type, NULL);
    append_consume(lexer, c);
}

static void handle_word_delimiter(struct lexer *lexer, char delim)
{
    // We discard the current character if it is a <blank> delimiter
    if (isblank(delim))
    {
        stream_pop(lexer->stream);
        return;
    }

    // If the current token is TOKEN_NONE, we set the current token type to the
    // delimiter
    if (lexer->cur_tok.type == TOKEN_NONE)
    {
        switch (delim)
        {
        case '\n':
            fill_token(&lexer->cur_tok, TOKEN_NEWLINE, NULL);
            break;
        case ';':
            fill_token(&lexer->cur_tok, TOKEN_SEMICOLON, NULL);
            break;
        case EOF:
            fill_token(&lexer->cur_tok, TOKEN_EOF, NULL);
            break;
        }
        stream_pop(lexer->stream);
    }
}

static void handle_escape_quote(struct lexer *lexer, enum QUOTING_CONTEXT *quoting_context)
{
    // Consume the <blackslash> to get the next character
    stream_pop(lexer->stream);

    char next_char = stream_peek(lexer->stream);

    // If a <newline> follows the <backslash>, the shell shall interpret
    // this as line continuation.
    if (next_char == '\n')
    {
        stream_pop(lexer->stream);
        return;
    }

    // If the <backslash> is the first character of a word, we start the
    // word
    if (lexer->cur_tok.type == TOKEN_NONE)
    {
        fill_token(&lexer->cur_tok, TOKEN_WORD, NULL);
    }

    if (*quoting_context == SINGLE_QUOTE)
    {
        append_char_to_token_value(&lexer->cur_tok, '\\');
        return;
    }

    // We append the <backslash> and the next character to the current token
    append_char_to_token_value(&lexer->cur_tok, '\\');
    append_consume(lexer, next_char);
}

static void handle_single_quote(struct lexer *lexer,
                                enum QUOTING_CONTEXT *quoting_context)
{
    if (*quoting_context == NONE)
    {
        *quoting_context = SINGLE_QUOTE;
        if (lexer->cur_tok.type == TOKEN_NONE)
        {
            fill_token(&lexer->cur_tok, TOKEN_WORD, NULL);
        }

        append_consume(lexer, '\'');
    }
    else
    {
        if (*quoting_context != DOUBLE_QUOTE)
        {
            *quoting_context = NONE;
        }
        append_consume(lexer, '\'');
    }

    return;
}

static void handle_double_quote(struct lexer *lexer,
                                enum QUOTING_CONTEXT *quoting_context)
{
    if (*quoting_context == NONE)
    {
        *quoting_context = DOUBLE_QUOTE;
        if (lexer->cur_tok.type == TOKEN_NONE)
        {
            fill_token(&lexer->cur_tok, TOKEN_WORD, NULL);
        }

        append_consume(lexer, '"');
    }
    else
    {
        if (*quoting_context != SINGLE_QUOTE)
        {
            *quoting_context = NONE;
        }
        append_consume(lexer, '"');
    }

    return;
}

/* Quoting is used to remove the special meaning of certain characters or words
 * to the shell. */
static void handle_quoting(struct lexer *lexer, char cur_char,
                           enum QUOTING_CONTEXT *quoting_context)
{
    switch (cur_char)
    {
    case '\\':
        handle_escape_quote(lexer, quoting_context);
        break;
    case '\'':
        handle_single_quote(lexer, quoting_context);
        break;
    case '"':
        handle_double_quote(lexer, quoting_context);
        break;
    default:
        // Should not happen
        break;
    }
    return;
}

static void handle_comment(struct lexer *lexer)
{
    struct stream_info *stream = lexer->stream;
    stream_pop(stream);
    while (stream_peek(stream) != '\n' && stream_peek(stream) != EOF)
    {
        stream_pop(stream);
    }
}

static void handle_parameter_expansion(struct lexer *lexer, char cur_char,
                                       int *is_inside_braces)
{
    if (lexer->cur_tok.type == TOKEN_NONE)
    {
        fill_token(&lexer->cur_tok, TOKEN_WORD, NULL);
    }

    if (cur_char == '$')
    {
        stream_pop(lexer->stream);

        if (stream_peek(lexer->stream) == '{')
        {
            (*is_inside_braces)++;
            append_char_to_token_value(&lexer->cur_tok, '$');
            append_consume(lexer, '{');
        }
        else
        {
            append_char_to_token_value(&lexer->cur_tok, '$');
        }
    }
    else
    {
        if (*is_inside_braces)
        {
            (*is_inside_braces)--;
        }
        append_consume(lexer, '}');
    }
}

static void set_error(struct lexer *lexer,
                      enum QUOTING_CONTEXT *quoting_context,
                      int is_inside_braces)
{
    if (*quoting_context == SINGLE_QUOTE)
    {
        lexer->last_error = UNMATCHED_SINGLE_QUOTE;
        lexer->cur_tok.type = TOKEN_ERROR;
    }
    else if (*quoting_context == DOUBLE_QUOTE)
    {
        lexer->last_error = UNMATCHED_DOUBLE_QUOTE;
        lexer->cur_tok.type = TOKEN_ERROR;
    }
    else if (is_inside_braces)
    {
        lexer->last_error = UNMATCHED_BRACE;
        lexer->cur_tok.type = TOKEN_ERROR;
    }
    else
    {
        lexer->last_error = NO_ERROR;
    }
}

/*
 * @brief: Updates the current token (inside the lexer struct)
 * according to the input stream. It follows the Token Recognition Algorithm
 * (see the POSIX standard at
 * https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html#tag_18_10)
 * to delimits tokens.
 */
static void delimit_token(struct lexer *lexer,
                          enum QUOTING_CONTEXT *quoting_context)
{
    int is_inside_braces = 0;
    char prev_char;
    size_t len;

    /* Append characters to the current token until the current token is
     * delimited. */
    while (1)
    {
        set_len_and_prev_char(lexer, &prev_char, &len);
        char cur_char = stream_peek(lexer->stream);
        // printf("cur_char: %c\n", cur_char);

        /* Token Recognition Algorithm Rule 1
         * If the end of the input stream is encountered, the current token
         * shall be delimited. If an EOF is encountered while looking for
         * matching single quote the lexer returns a TOKEN_ERROR */
        if (cur_char == EOF)
        {
            handle_word_delimiter(lexer, EOF);
            set_error(lexer, quoting_context, is_inside_braces);

            // Delimit the current token
            return;
        }

        /* Token Recognition Algorithm Rule 2
         * If the previous character was used as part of an operator and the
         * current character is not quoted and can be used with the previous
         * characters to form an operator, it shall be used as part of that
         * (operator) token. */
        if (lexer->cur_tok.type == TOKEN_OPERATOR && !(*quoting_context != NONE)
            && len == 1 && can_be_second_in_ope(prev_char, cur_char))
        {
            append_consume(lexer, cur_char);
            continue;
        }

        /* Token Recognition Algorithm Rule 3 */
        // TODO: here we assume that if the current token is an operator, it has
        // at least one and at most one characters (this is because for now the
        // operators are two characters long at most).
        if (lexer->cur_tok.type == TOKEN_OPERATOR
            && (!can_be_second_in_ope(prev_char, cur_char) || len == 2))
        {
            return;
        }

        /* Token Recognition Algorithm Rule 4
         * If the current character is <backslash>, single-quote, or
         * double-quote and it is not quoted, it shall affect quoting for
         * subsequent characters up to the end of the quoted text. */
        if (cur_char == '\\' || cur_char == '\'' || cur_char == '"')
        {
            handle_quoting(lexer, cur_char, quoting_context);
            continue;
        }

        /* Token Recognition Algorithm Rule 5
         * If the current character is an unquoted '$', the shell shall identify
         * the start of any candidate for parameter expansion from its
         * introductory unquoted character sequences: '$' or "${", up to the
         * matching '}' character.}
         */
        if (*quoting_context == NONE && (cur_char == '$' || cur_char == '}'))
        {
            handle_parameter_expansion(lexer, cur_char, &is_inside_braces);
            continue;
        }

        /* Token Recognition Algorithm Rule 6 */
        if (*quoting_context == NONE && can_be_first_in_ope(cur_char))
        {
            if (lexer->cur_tok.type == TOKEN_NONE)
            {
                set_append_consume(lexer, TOKEN_OPERATOR, cur_char);
                continue;
            }

            /* If the current token is a word, we check if it is a sequence
             * of digits, if it is the case, we don't
             * consider it as a word anymore and we consider it as an
             * io_number (only if the current char is a '<' or a '>'). */
            if (lexer->cur_tok.type == TOKEN_WORD
                && is_str_sequence_of_digits(lexer->cur_tok.value)
                && (cur_char == '<' || cur_char == '>'))
            {
                lexer->cur_tok.type = TOKEN_IONUMBER;
            }
            // TODO: we must set the first character of the next (operator)
            // token here. It seems that it don't break anything if it is
            // not implemented. It will be implemented if we found a case
            // where it is needed.
            return;
        }

        /* Token Recognition Algorithm Rule 7 (modified)
         * If the current character is an unquoted <blank>, '\n' or ';', any
         * token containing the previous character is delimited and the current
         * character shall be discarded if it is a <blank>, for '\n' and ';',
         * the current token shall be updated to TOKEN_NEWLINE or
         * TOKEN_SEMICOLON*/
        if (*quoting_context == NONE && !(is_inside_braces)
            && is_delimiter(cur_char))
        {
            handle_word_delimiter(lexer, cur_char);

            if (isblank(cur_char) && lexer->cur_tok.type != TOKEN_WORD)
            {
                continue;
            }

            // Delimit the current token
            return;
        }

        /* Token Recognition Algorithm Rule 8
         * If the previous token is part of a word, the current character is
         * appended to the that word. */
        if (lexer->cur_tok.type == TOKEN_WORD)
        {
            append_consume(lexer, cur_char);
            continue;
        }

        /* Token Recognition Algorithm Rule 9
         * If the current character is a '#', it and all subsequent characters
         * up to, but excluding, the next <newline> shall be discarded as a
         * comment. The <newline> that ends the line is not considered part of
         * the comment. */
        if (cur_char == '#')
        {
            handle_comment(lexer);
            continue;
        }

        /* Token Recognition Algorithm Rule 10
         * The current character is used as the start of a new word. */
        set_append_consume(lexer, TOKEN_WORD, cur_char);
    }
}

struct token parse_input_for_tok(struct lexer *lexer)
{
    enum QUOTING_CONTEXT quoting_context = NONE;

    // Reset current token
    lexer->cur_tok.type = TOKEN_NONE;

    // Reset last error
    lexer->last_error = NO_ERROR;

    delimit_token(lexer, &quoting_context);

    categorize_token(&(lexer->cur_tok));

    return lexer->cur_tok;
}

struct token lexer_peek(struct lexer *lexer)
{
    int is_verbose = lexer->opts->verbose;

    if (lexer->must_parse_next_tok)
    {
        if (lexer->cur_tok.value != NULL)
        {
            free(lexer->cur_tok.value); // Free the previous token value if any
        }

        lexer->must_parse_next_tok = 0;
        parse_input_for_tok(lexer); // Update the current token

        if (lexer->last_error != NO_ERROR)
        {
            fprintf(stderr, "42sh: lexer error: %s\n",
                    get_lexer_error_msg(lexer->last_error));
        }

        PRINT_TOKEN(is_verbose, lexer->cur_tok, "Peek");
        return lexer->cur_tok;
    }

    PRINT_TOKEN(is_verbose, lexer->cur_tok, "Peek");
    return lexer->cur_tok;
}

struct token lexer_pop(struct lexer *lexer)
{
    lexer_peek(lexer);

    // Print the token if verbose mode is enabled
    PRINT_TOKEN(lexer->opts->verbose, lexer->cur_tok, "Pop");

    lexer->must_parse_next_tok = 1;
    return lexer->cur_tok;
}

/* Check if the given TOKEN_WORD can describe an assignment word. This is
 * specified in the Rule 7 of the algorithm described in section 2.2.10 of the
 * SCL
 * (https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html#tag_18_10_02)
 */
int is_assignment_word(struct token *token, int is_the_first_word)
{
    // An assignment must derive from a TOKEN_WORD
    if (token->type != TOKEN_WORD)
    {
        return 0;
    }

    // Get the position of the '=' character if any, NULL otherwise
    char *equal_sign = strchr(token->value, '=');

    /* Rule 7.a */
    if (is_the_first_word)
    {
        if (equal_sign == NULL)
        {
            return 0;
        }
    }

    /* Rule 7.b */
    // TODO: check if the '=' is not quoted
    // First paragraph of the Rule 7.b

    // Check if the first character is '=', if so 0 is returned
    if (token->value != NULL && token->value[0] == '=')
    {
        return 0;
    }

    // Check that all the characters before '=' form a valid name
    if (equal_sign != NULL)
    {
        char *name = malloc(sizeof(char) * (equal_sign - token->value + 1));
        strncpy(name, token->value, equal_sign - token->value);
        name[equal_sign - token->value] = '\0';

        if (!is_name(name))
        {
            free(name);
            return 0;
        }

        free(name);
        return 1;
    }

    return 0;
}
