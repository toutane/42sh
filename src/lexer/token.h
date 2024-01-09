#ifndef TOKEN_H
#define TOKEN_H

#include <unistd.h>

enum token_type
{
    TOKEN_WORD, // a word
    TOKEN_EOF, // end of input marker
};

struct token
{
    enum token_type type; // The kind of token
    char *value; // If the token is a number, its value
};

#endif /* !TOKEN_H */
