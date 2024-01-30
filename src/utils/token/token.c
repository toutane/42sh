#include "token.h"

#include <string.h>

struct token_map_str
{
    enum token_type type;
    char *str;
};

struct token_map_int
{
    char *value;
    int type;
};

static struct token_map_int word_map[] = {
    { "if", TOKEN_IF },
    { "then", TOKEN_THEN },
    { "else", TOKEN_ELSE },
    { "elif", TOKEN_ELIF },
    { "fi", TOKEN_FI },
    { "!", TOKEN_NEG },
    { "{", TOKEN_LBRACE },
    { "}", TOKEN_RBRACE },
    { "for", TOKEN_FOR },
    { "in", TOKEN_IN },
    { "while", TOKEN_WHILE },
    { "until", TOKEN_UNTIL },
    { "do", TOKEN_DO },
    { "done", TOKEN_DONE },
    { NULL, 0 } // End of array marker
};

static struct token_map_int operator_map[] = {
    { ">", TOKEN_GREAT },
    { "<", TOKEN_LESS },
    { ">>", TOKEN_DGREAT },
    { ">&", TOKEN_GREATAND },
    { "<&", TOKEN_LESSAND },
    { ">|", TOKEN_CLOBBER },
    { "<>", TOKEN_LESSGREAT },
    { "|", TOKEN_PIPE },
    { "||", TOKEN_OR },
    { "&&", TOKEN_AND },
    { NULL, 0 } // End of array marker
};

void categorize_token(struct token *tok)
{
    char *val = tok->value;
    struct token_map_int *map;

    if (tok->type == TOKEN_WORD)
    {
        map = word_map;
    }
    else if (tok->type == TOKEN_OPERATOR)
    {
        map = operator_map;
    }
    else
    {
        return;
    }

    for (; map->value != NULL; map++)
    {
        if (strcmp(val, map->value) == 0)
        {
            tok->type = map->type;
            break;
        }
    }
}

static struct token_map_str token_map[] = {
    { TOKEN_NONE, "TOKEN_NONE" },
    { TOKEN_WORD, "TOKEN_WORD" },
    { TOKEN_IONUMBER, "TOKEN_IONUMBER" },
    { TOKEN_NEWLINE, "TOKEN_NEWLINE" },
    { TOKEN_SEMICOLON, "TOKEN_SEMICOLON" },
    { TOKEN_ERROR, "TOKEN_ERROR" },
    { TOKEN_EOF, "TOKEN_EOF" },
    { TOKEN_LPAREN, "TOKEN_LPAREN" },
    { TOKEN_RPAREN, "TOKEN_RPAREN" },

    /* Reserved words */

    { TOKEN_IF, "TOKEN_IF" },
    { TOKEN_THEN, "TOKEN_THEN" },
    { TOKEN_ELSE, "TOKEN_ELSE" },
    { TOKEN_ELIF, "TOKEN_ELIF" },
    { TOKEN_FI, "TOKEN_FI" },
    { TOKEN_NEG, "TOKEN_NEG" },
    { TOKEN_LBRACE, "TOKEN_LBRACE" },
    { TOKEN_RBRACE, "TOKEN_RBRACE" },
    { TOKEN_FOR, "TOKEN_FOR" },
    { TOKEN_IN, "TOKEN_IN" },
    { TOKEN_WHILE, "TOKEN_WHILE" },
    { TOKEN_UNTIL, "TOKEN_UNTIL" },
    { TOKEN_DO, "TOKEN_DO" },
    { TOKEN_DONE, "TOKEN_DONE" },

    /* Operators */

    { TOKEN_OPERATOR, "TOKEN_OPERATOR" },
    { TOKEN_LESS, "TOKEN_LESS" },
    { TOKEN_GREAT, "TOKEN_GREAT" },
    { TOKEN_DGREAT, "TOKEN_DGREAT" },
    { TOKEN_LESSAND, "TOKEN_LESSAND" },
    { TOKEN_GREATAND, "TOKEN_GREATAND" },
    { TOKEN_LESSGREAT, "TOKEN_LESSGREAT" },
    { TOKEN_CLOBBER, "TOKEN_CLOBBER" },
    { TOKEN_PIPE, "TOKEN_PIPE" },
    { TOKEN_OR, "TOKEN_OR" },
    { TOKEN_AND, "TOKEN_AND" },

    { 0, "TOKEN_UNKNOWN" } // End of array marker
};

char *token_type_to_str(enum token_type type)
{
    struct token_map_str *map;

    for (map = token_map; map->str != NULL; map++)
    {
        if (type == map->type)
        {
            return map->str;
        }
    }

    return "TOKEN_UNKNOWN";
}

int is_reserved_word(struct token token)
{
    enum token_type type = token.type;
    return type == TOKEN_IF || type == TOKEN_THEN || type == TOKEN_ELSE
        || type == TOKEN_ELIF || type == TOKEN_FI || type == TOKEN_NEG
        || type == TOKEN_LBRACE || type == TOKEN_RBRACE || type == TOKEN_FOR
        || type == TOKEN_IN || type == TOKEN_WHILE || type == TOKEN_UNTIL
        || type == TOKEN_DO || type == TOKEN_DONE;
}

int can_be_first_in_ope(char c)
{
    return c == '<' || c == '>' || c == '|' || c == '&';
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
    case '|':
        return cur == '|';
    case '&':
        return cur == '&';
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
