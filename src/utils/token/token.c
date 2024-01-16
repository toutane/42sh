#include "token.h"

char *token_type_to_str(enum token_type type)
{
    switch (type)
    {
    case TOKEN_NONE:
        return "TOKEN_NONE";
    case TOKEN_WORD:
        return "TOKEN_WORD";
    case TOKEN_NEWLINE:
        return "TOKEN_NEWLINE";
    case TOKEN_SEMICOLON:
        return "TOKEN_SEMICOLON";
    case TOKEN_EOF:
        return "TOKEN_EOF";
    case TOKEN_IF:
        return "TOKEN_IF";
    case TOKEN_THEN:
        return "TOKEN_THEN";
    case TOKEN_ELSE:
        return "TOKEN_ELSE";
    case TOKEN_ELIF:
        return "TOKEN_ELIF";
    case TOKEN_FI:
        return "TOKEN_FI";
    default:
        return "TOKEN_UNKNOWN";
    }
}

int is_reserved_word(struct token token)
{
    enum token_type type = token.type;
    return type == TOKEN_IF || type == TOKEN_THEN || type == TOKEN_ELSE
        || type == TOKEN_ELIF || type == TOKEN_FI;
}
