#include "lexer/lexer_error.h"

struct LEXER_ERROR_MSG_INFO
{
    enum LEXER_ERROR error;
    char *msg;
};

struct LEXER_ERROR_MSG_INFO lexer_error_msg_map[] = {
    { NO_ERROR, "no error" },
    { UNMATCHED_SINGLE_QUOTE, "unexpected EOF while looking for match \'" },
    { UNMATCHED_DOUBLE_QUOTE, "unexpected EOF while looking for matching \"" },
    { UNMATCHED_BRACE, "unmatched brace" },
    { UNMATCHED_PARENTHESIS, "unmatched parenthesis" },
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

void set_lexer_last_error(struct lexer *lexer,
                          enum QUOTING_CONTEXT *quoting_ctx, int braces_depth,
                          int paren_depth)
{
    if (*quoting_ctx != NONE)
    {
        lexer->last_error = *quoting_ctx == SINGLE_QUOTE
            ? UNMATCHED_SINGLE_QUOTE
            : UNMATCHED_DOUBLE_QUOTE;
        lexer->cur_tok.type = TOKEN_ERROR;
        return;
    }

    if (braces_depth != 0)
    {
        lexer->last_error = UNMATCHED_BRACE;
        lexer->cur_tok.type = TOKEN_ERROR;
        return;
    }

    if (paren_depth != 0)
    {
        lexer->last_error = UNMATCHED_PARENTHESIS;
        lexer->cur_tok.type = TOKEN_ERROR;
        return;
    }

    lexer->last_error = NO_ERROR;
}
