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

struct lexer *lexer_new(struct stream_info *stream, struct options *opts)
{
    struct lexer *lexer = calloc(1, sizeof(struct lexer));
    if (lexer == NULL)
    {
        fprintf(stderr, "lexer_new: calloc failed\n");
        return NULL;
    }

    lexer->opts = opts;
    lexer->stream = stream;
    lexer->cur_tok.type = TOKEN_NONE;
    lexer->cur_tok.value = NULL;
    lexer->must_parse_next_tok = 1;

    if (lexer->opts->verbose)
    {
        printf("[LEXER] Lexer created\n");
    }

    return lexer;
}

void lexer_free(struct lexer *lexer)
{
    if (lexer == NULL)
    {
        printf("lexer_free: lexer is NULL\n");
        return;
    }

    if (lexer->cur_tok.value != NULL)
    {
        free(lexer->cur_tok.value); // Free the current token value if any
    }

    int is_verbose = lexer->opts->verbose;

    free(lexer);

    if (is_verbose)
    {
        printf("[LEXER] Lexer freed\n");
    }
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

static void append_char_to_token_value(struct token *tok, char c)
{
    if (tok == NULL)
    {
        fprintf(stderr, "append_char_to_token_value: tok is NULL\n");
        return;
    }

    if (tok->value == NULL)
    {
        tok->value = calloc(2, sizeof(char));
        tok->value[0] = c;
        tok->value[1] = '\0';
        return;
    }
    size_t len = strlen(tok->value);
    tok->value = realloc(tok->value, len + 2);
    tok->value[len] = c;
    tok->value[len + 1] = '\0';
}

static void handle_end_of_file(struct lexer *lexer)
{
    if (lexer->cur_tok.type == TOKEN_NONE)
    {
        fill_token(&lexer->cur_tok, TOKEN_EOF, NULL);
        stream_pop(lexer->stream);
    }
}

static void handle_newline(struct lexer *lexer)
{
    if (lexer->cur_tok.type == TOKEN_NONE)
    {
        fill_token(&lexer->cur_tok, TOKEN_NEWLINE, NULL);
        stream_pop(lexer->stream);
    }
}

static void handle_semicolon(struct lexer *lexer)
{
    if (lexer->cur_tok.type == TOKEN_NONE)
    {
        fill_token(&lexer->cur_tok, TOKEN_SEMICOLON, NULL);
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

static void delimit_token(struct lexer *lexer)
{
    int is_inside_quotes = 0;

    struct stream_info *stream = lexer->stream;

    lexer->cur_tok.type = TOKEN_NONE;

    while (1)
    {
        char cur_char = stream_peek(stream);

        /* Token Recognition Algorithm Rule 1
         * If the end of the input stream is encountered, the current token
         * shall be delimited. */
        if (cur_char == EOF)
        {
            handle_end_of_file(lexer);
            if (is_inside_quotes)
            {
                lexer->cur_tok.type = TOKEN_ERROR; // Unexpected EOF while looking for matching single quote
            }
            return;
        }

        // Token Recognition Algorithm Rule 4
        if (cur_char == '\'')
        {
            handle_single_quote(lexer, &is_inside_quotes);
            continue;
        }

        // Token Recognition Algorithm Rule 7 (modified)
        if (cur_char == '\n' && !is_inside_quotes)
        {
            handle_newline(lexer);
            return;
        }

        // Token Recognition Algorithm Rule 7.1 (added, used to recognize
        // semicolon)
        if (cur_char == ';' && !is_inside_quotes)
        {
            handle_semicolon(lexer);
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

static void categorize_token(struct lexer *lexer)
{
    char *val = lexer->cur_tok.value;
    if (lexer->cur_tok.type == TOKEN_WORD)
    {
        if (strcmp(val, "if") == 0)
        {
            lexer->cur_tok.type = TOKEN_IF;
        }
        else if (strcmp(val, "then") == 0)
        {
            lexer->cur_tok.type = TOKEN_THEN;
        }
        else if (strcmp(val, "else") == 0)
        {
            lexer->cur_tok.type = TOKEN_ELSE;
        }
        else if (strcmp(val, "elif") == 0)
        {
            lexer->cur_tok.type = TOKEN_ELIF;
        }
        else if (strcmp(val, "fi") == 0)
        {
            lexer->cur_tok.type = TOKEN_FI;
        }
    }
}

static void single_quote_expansion(struct lexer *lexer)
{
    if (lexer->cur_tok.type != TOKEN_WORD)
    {
        return;
    }

    char *val = lexer->cur_tok.value;
    size_t len = strlen(val);
    if (len < 2)
    {
        return;
    }

    // Calculate the number of single quotes in the token value
    int nb_single_quotes = 0;
    for (size_t i = 0; i < len; i++)
    {
        if (val[i] == '\'')
        {
            nb_single_quotes++;
        }
    }

    // Remove every single quote from the token value
    char *new_val = calloc(len - nb_single_quotes + 1, sizeof(char));
    int j = 0;
    for (size_t i = 0; i < len; i++)
    {
        if (val[i] != '\'')
        {
            new_val[j] = val[i];
            j++;
        }
    }

    free(lexer->cur_tok.value);
    lexer->cur_tok.value = new_val;
}

struct token parse_input_for_tok(struct lexer *lexer)
{
    delimit_token(lexer);
    categorize_token(lexer);
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
