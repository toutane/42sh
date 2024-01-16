#include "token.h"

#define TOKEN_UNKNOWN "TOKEN_UNKNOWN"

static char *token_to_str[TOKEN_NUMBER] = {
    [TOKEN_NONE] = "TOKEN_NONE",
    [TOKEN_WORD] = "TOKEN_WORD",
    [TOKEN_NEWLINE] = "TOKEN_NEWLINE",
    [TOKEN_SEMICOLON] = "TOKEN_SEMICOLON",
    [TOKEN_EOF] = "TOKEN_EOF",
    [TOKEN_IF] = "TOKEN_IF",
    [TOKEN_THEN] = "TOKEN_THEN",
    [TOKEN_ELSE] = "TOKEN_ELSE",
    [TOKEN_ELIF] = "TOKEN_ELIF",
    [TOKEN_FI] = "TOKEN_FI"
};

char *token_type_to_str(enum token_type type)
{
    if (type < TOKEN_NUMBER)
    {
        return token_to_str[type];
    }
    else
    {
        return TOKEN_UNKNOWN;
    }
}

int is_reserved_word(struct token token)
{
    enum token_type type = token.type;
    return type == TOKEN_IF || type == TOKEN_THEN || type == TOKEN_ELSE
        || type == TOKEN_ELIF || type == TOKEN_FI;
}
