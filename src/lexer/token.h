#ifndef TOKEN_H
#define TOKEN_H

#include <unistd.h>

enum token_type
{
    TOKEN_NONE, // no token

    TOKEN_WORD, // a word
    TOKEN_NEWLINE, // '\n'
    TOKEN_SEMICOLON, // ';'
    TOKEN_EOF, // end of input marker

    /* Reserved words */

    TOKEN_IF, // 'if'
    TOKEN_THEN, // 'then'
    TOKEN_ELSE, // 'else'
    TOKEN_ELIF, // 'elif'
    TOKEN_FI, // 'fi'
};

struct token
{
    enum token_type type; // The kind of token
    char *value; // If the token is a word, its value, allocated on the heap !
};

#endif /* !TOKEN_H */
