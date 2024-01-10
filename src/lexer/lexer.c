#include "lexer.h"

#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"

struct lexer *lexer_new(const char *input)
{
    struct lexer *lexer = calloc(1, sizeof(struct lexer));

    lexer->input = input;
    lexer->pos = 0;
    lexer->cur_tok.value = NULL;

    return lexer;
}

void lexer_free(struct lexer *lexer)
{
    if (lexer->cur_tok.value != NULL)
    {
        free(lexer->cur_tok.value); // Free the current token value if any
    }

    free(lexer);
}

void fill_token(struct token *tok, enum token_type type, char *value)
{
    tok->type = type;
    tok->value = value;
}

/**
 * @brief Returns a token from the input string
 * This function goes through the input string character by character and
 * builds a token. lexer_peek and lexer_pop should call it. If the input is
 * invalid, you must print something on stderr and return the appropriate token.
 */
struct token parse_input_for_tok(struct lexer *lexer)
{
    // Skip whitespace
    while (isspace(lexer->input[lexer->pos])
           && lexer->input[lexer->pos] != '\n')
    {
        lexer->pos++;
    }

    // Check for a '\n'
    if (lexer->input[lexer->pos] == '\n')
    {
        lexer->pos++;
        fill_token(&lexer->cur_tok, TOKEN_NEWLINE, NULL);
        return lexer->cur_tok;
    }

    // Check for end of input
    if (lexer->input[lexer->pos] == '\0')
    {
        fill_token(&lexer->cur_tok, TOKEN_EOF, NULL);
        return lexer->cur_tok;
    }

    // Check for a ';'
    if (lexer->input[lexer->pos] == ';')
    {
        lexer->pos++;
        fill_token(&lexer->cur_tok, TOKEN_SEMICOLON, NULL);
        return lexer->cur_tok;
    }

    // Check for a word
    size_t len = 0;
    while (!isspace(lexer->input[lexer->pos]))
    {
        len++;
        lexer->pos++;

        if (lexer->input[lexer->pos]
            == ';') // Check for a ';' at the end of the word
        {
            break;
        }
    }

    if (len > 0)
    {
        char *value = calloc(len + 1, sizeof(char));
        memcpy(value, lexer->input + lexer->pos - len, len);

        fill_token(&lexer->cur_tok, TOKEN_WORD, value);
        return lexer->cur_tok;
    }

    return lexer->cur_tok;
}

/**
 * @brief Returns the next token, but doesn't move forward: calling lexer_peek
 * multiple times in a row always returns the same result. This functions is
 * meant to help the parser check if the next token matches some rule.
 */
struct token lexer_peek(struct lexer *lexer)
{
    if (lexer->pos == 0 || lexer->must_parse_next_tok)
    {
        if (lexer->cur_tok.value != NULL)
        {
            free(lexer->cur_tok.value); // Free the previous token value if any
        }

        lexer->must_parse_next_tok = 0;
        return parse_input_for_tok(lexer);
    }

    return lexer->cur_tok;
}

/**
 * @brief Returns the next token, and removes it from the stream:
 *   calling lexer_pop in a loop will iterate over all tokens until EOF.
 */
struct token lexer_pop(struct lexer *lexer)
{
    lexer_peek(lexer);
    lexer->must_parse_next_tok = 1;
    return lexer->cur_tok;
}
