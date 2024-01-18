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
    // int is_quoted = (*quoting_context != NONE);
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
            lexer->cur_tok.type =
                (*quoting_context != NONE) ? TOKEN_ERROR : lexer->cur_tok.type;

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
         * If the current character is single-quote, or
         * double-quote and it is not quoted, it shall affect quoting for
         * subsequent characters up to the end of the quoted text. */
        if (cur_char == '\'' || cur_char == '"')
        {
            handle_quoting(lexer, cur_char, quoting_context);
            continue;
        }

        /* Token Recognition Algorithm Rule 6 */
        if (!(*quoting_context != NONE) && can_be_first_in_ope(cur_char))
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
        if (!(*quoting_context != NONE) && is_delimiter(cur_char))
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

    delimit_token(lexer, &quoting_context);

    categorize_token(&(lexer->cur_tok));

    single_quote_expansion(lexer);

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
