#include "lexer/token_recognition.h"

void handle_escape_quote(struct lexer *lexer, enum QUOTING_CONTEXT *quoting_ctx)
{
    // Consume the <blackslash> to get the next character
    stream_pop(lexer->stream);

    char next_char = stream_peek(lexer->stream);

    // If a <newline> follows the <backslash>, the shell shall interpret
    // this as line continuation.
    if (next_char == '\n' && *quoting_ctx != SINGLE_QUOTE)
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

    if (*quoting_ctx == SINGLE_QUOTE)
    {
        append_char_to_token_value(&lexer->cur_tok, '\\');
        return;
    }

    // We append the <backslash> and the next character to the current token
    append_char_to_token_value(&lexer->cur_tok, '\\');
    append_consume(lexer, next_char);
}

void handle_single_quote(struct lexer *lexer, enum QUOTING_CONTEXT *quoting_ctx)
{
    if (*quoting_ctx == NONE)
    {
        *quoting_ctx = SINGLE_QUOTE;
        if (lexer->cur_tok.type == TOKEN_NONE)
        {
            fill_token(&lexer->cur_tok, TOKEN_WORD, NULL);
        }

        append_consume(lexer, '\'');
    }
    else
    {
        if (*quoting_ctx != DOUBLE_QUOTE)
        {
            *quoting_ctx = NONE;
        }
        append_consume(lexer, '\'');
    }

    return;
}

void handle_double_quote(struct lexer *lexer, enum QUOTING_CONTEXT *quoting_ctx)
{
    if (*quoting_ctx == NONE)
    {
        *quoting_ctx = DOUBLE_QUOTE;
        if (lexer->cur_tok.type == TOKEN_NONE)
        {
            fill_token(&lexer->cur_tok, TOKEN_WORD, NULL);
        }

        append_consume(lexer, '"');
    }
    else
    {
        if (*quoting_ctx != SINGLE_QUOTE)
        {
            *quoting_ctx = NONE;
        }
        append_consume(lexer, '"');
    }

    return;
}
