#include "lexer.h"

#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"

struct lexer *lexer_new(struct stream_info *stream)
{
    struct lexer *lexer = calloc(1, sizeof(struct lexer));
    if (lexer == NULL)
    {
        fprintf(stderr, "lexer_new: calloc failed\n");
        return NULL;
    }

    lexer->stream = stream;
    lexer->cur_tok.type = TOKEN_NONE;
    lexer->cur_tok.value = NULL;
    lexer->must_parse_next_tok = 1;

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

    free(lexer);
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

void append_char_to_token_value(struct token *tok, char c)
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

struct token handle_end_of_file(struct lexer *lexer)
{
    if (lexer->cur_tok.type == TOKEN_NONE)
    {
        fill_token(&lexer->cur_tok, TOKEN_EOF, NULL);
        stream_pop(lexer->stream);
        return lexer->cur_tok;
    }
    else
    {
        return lexer->cur_tok;
    }
}

struct token handle_newline(struct lexer *lexer)
{
    if (lexer->cur_tok.type == TOKEN_NONE)
    {
        fill_token(&lexer->cur_tok, TOKEN_NEWLINE, NULL);
        stream_pop(lexer->stream);
        return lexer->cur_tok;
    }
    else
    {
        return lexer->cur_tok;
    }
}

struct token handle_semicolon(struct lexer *lexer)
{
    if (lexer->cur_tok.type == TOKEN_NONE)
    {
        fill_token(&lexer->cur_tok, TOKEN_SEMICOLON, NULL);
        stream_pop(lexer->stream);
        return lexer->cur_tok;
    }
    else
    {
        return lexer->cur_tok;
    }
}

struct token parse_input_for_tok(struct lexer *lexer)
{
    struct stream_info *stream = lexer->stream;

    lexer->cur_tok.type = TOKEN_NONE;

    while (1)
    {
        char cur_char = stream_peek(stream);

        // Token Recognition Algorithm Rule 1
        if (cur_char == EOF)
        {
            return handle_end_of_file(lexer);
        }

        // Token Recognition Algorithm Rule 7 (modified)
        if (cur_char == '\n')
        {
            return handle_newline(lexer);
        }

        // Token Recognition Algorithm Rule 7.1 (added, used to recognize
        // semicolon)
        if (cur_char == ';')
        {
            return handle_semicolon(lexer);
        }

        // Token Recognition Algorithm Rule 8
        if (isspace(cur_char))
        {
            stream_pop(stream);
            if (lexer->cur_tok.type == TOKEN_WORD)
            {
                return lexer->cur_tok;
            }
            continue;
        }

        // Token Recognition Algorithm Rule 9
        if (lexer->cur_tok.type == TOKEN_WORD)
        {
            append_char_to_token_value(&lexer->cur_tok, cur_char);
            stream_pop(stream);
            continue;
        }

        // Token Recognition Algorithm Rule 11
        fill_token(&lexer->cur_tok, TOKEN_WORD, NULL);
        append_char_to_token_value(&lexer->cur_tok, cur_char);
        stream_pop(stream);
        continue;
    }
}

struct token lexer_peek(struct lexer *lexer)
{
    if (lexer->must_parse_next_tok)
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

struct token lexer_pop(struct lexer *lexer)
{
    lexer_peek(lexer);
    lexer->must_parse_next_tok = 1;
    return lexer->cur_tok;
}
