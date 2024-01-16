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
    case TOKEN_OPERATOR:
        return "TOKEN_OPERATOR";
    case TOKEN_LESS:
        return "TOKEN_LESS";
    case TOKEN_GREAT:
        return "TOKEN_GREAT";
    case TOKEN_DGREAT:
        return "TOKEN_DGREAT";
    // case TOKEN_DLESS: // Not implemented yet
    // return "TOKEN_DLESS";
    case TOKEN_LESSAND:
        return "TOKEN_LESSAND";
    case TOKEN_GREATAND:
        return "TOKEN_GREATAND";
    case TOKEN_LESSGREAT:
        return "TOKEN_LESSGREAT";
    case TOKEN_CLOBBER:
        return "TOKEN_CLOBBER";
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

int can_be_first_in_ope(char c)
{
    return c == '<' || c == '>';
}

int can_be_second_in_ope(char prev, char cur)
{
    if (prev == '\0')
    {
        return 0;
    }

    switch (prev)
    {
    case '<':
        return cur == '&' || cur == '>';
    case '>':
        return cur == '>' || cur == '&' || cur == '|';
    default:
        return 0;
    }
}

int is_redirection_operator(struct token token)
{
    enum token_type type = token.type;
    return type == TOKEN_LESS || type == TOKEN_GREAT || type == TOKEN_DGREAT
        || type == TOKEN_LESSAND || type == TOKEN_GREATAND
        || type == TOKEN_LESSGREAT || type == TOKEN_CLOBBER;
}
