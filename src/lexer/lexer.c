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

static void handle_one_char_token(struct lexer *lexer, enum token_type type)
{
    if (lexer->cur_tok.type == TOKEN_NONE)
    {
        fill_token(&lexer->cur_tok, type, NULL);
        stream_pop(lexer->stream);
    }
}

static void handle_single_quote(struct lexer *lexer, int *is_inside_quotes)
{
    if (!*is_inside_quotes)
    {
        *is_inside_quotes = 1;
        if (lexer->cur_tok.type == TOKEN_NONE)
        {
            fill_token(&lexer->cur_tok, TOKEN_WORD, NULL);
        }
        append_char_to_token_value(&lexer->cur_tok, '\'');
        stream_pop(lexer->stream);
    }
    else
    {
        *is_inside_quotes = 0;
        append_char_to_token_value(&lexer->cur_tok, '\'');
        stream_pop(lexer->stream);
    }
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
 * @brief: This function updates the current token (inside the lexer struct)
 * according to the input stream. It follows the Token Recognition Algorithm
 * (see the POSIX standard at
 * https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html#tag_18_10)
 * to delimits tokens.
 */
static void delimit_token(struct lexer *lexer)
{
    int is_inside_quotes = 0;

    struct stream_info *stream = lexer->stream;

    // Reset the current token
    lexer->cur_tok.type = TOKEN_NONE;

    /* Append characters to the current token until the current token is
     * delimited. */
    while (1)
    {
        char cur_char = stream_peek(stream);

        if (lexer->opts->verbose)
        {
            printf("[LEXER] Current token: %s\n",
                   token_type_to_str(lexer->cur_tok.type));
            printf("[LEXER] Current character: %c\n", cur_char);
        }

        /* Token Recognition Algorithm Rule 1
         * If the end of the input stream is encountered, the current token
         * shall be delimited. If an EOF is encountered while looking for
         * matching single quote the lexer returns a TOKEN_ERROR */
        if (cur_char == EOF)
        {
            handle_one_char_token(lexer, TOKEN_EOF);
            if (is_inside_quotes)
            {
                lexer->cur_tok.type = TOKEN_ERROR;
            }

            // Delimit the current token
            return;
        }

        /* Token Recognition Algorithm Rule 2
         * If the previous character was used as part of an operator and the
         * current character is not quoted and can be used with the previous
         * characters to form an operator, it shall be used as part of that
         * (operator) token. */
        size_t len = 0;
        if (lexer->cur_tok.type != TOKEN_NONE && lexer->cur_tok.value != NULL)
        {
            len = strlen(lexer->cur_tok.value);
        }
        char prev_char = '\0';
        if (lexer->cur_tok.type != TOKEN_NONE && len > 0)
        {
            prev_char = lexer->cur_tok.value[len - 1];
        }

        if (lexer->cur_tok.type == TOKEN_OPERATOR && !is_inside_quotes
            && len == 1 && can_be_second_in_ope(prev_char, cur_char))
        {
            append_char_to_token_value(&lexer->cur_tok, cur_char);
            stream_pop(stream);
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

        // Token Recognition Algorithm Rule 4
        if (cur_char == '\'')
        {
            handle_single_quote(lexer, &is_inside_quotes);
            continue;
        }

        /* Token Recognition Algorithm Rule 6 */
        if (!is_inside_quotes && can_be_first_in_ope(cur_char))
        {
            if (lexer->cur_tok.type == TOKEN_NONE)
            {
                fill_token(&lexer->cur_tok, TOKEN_OPERATOR, NULL);
                append_char_to_token_value(&lexer->cur_tok, cur_char);
                stream_pop(stream);
                continue;
            }
            else
            {
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
        }

        /* Token Recognition Algorithm Rule 7 (modified) */
        if (!is_inside_quotes && (cur_char == '\n' || cur_char == ';'))
        {
            /* If the current token is TOKEN_NONE (that means that the current
             * character is a delimiter and that it delimited the previous
             * token), we set the current token type to apropriate token. */
            handle_one_char_token(
                lexer, cur_char == '\n' ? TOKEN_NEWLINE : TOKEN_SEMICOLON);

            // Delimit the current token
            return;
        }

        // Token Recognition Algorithm Rule 7.2
        if (isblank(cur_char) && !is_inside_quotes)
        {
            stream_pop(stream);
            if (lexer->cur_tok.type == TOKEN_WORD)
            {
                return;
            }
            continue;
        }

        /* Token Recognition Algorithm Rule 8
         * If the previous token is part of a word, the current character is
         * appended to the that word. */
        if (lexer->cur_tok.type == TOKEN_WORD)
        {
            append_char_to_token_value(&lexer->cur_tok, cur_char);
            stream_pop(stream);
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
        fill_token(&lexer->cur_tok, TOKEN_WORD, NULL);
        append_char_to_token_value(&lexer->cur_tok, cur_char);
        stream_pop(stream);
        continue;
    }
}

struct token parse_input_for_tok(struct lexer *lexer)
{
    delimit_token(lexer);
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
