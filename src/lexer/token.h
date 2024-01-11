#ifndef TOKEN_H
#define TOKEN_H

#include <unistd.h>

/*
 * @brief The different types of tokens
 */
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

/*
 * @brief Token structure
 */
struct token
{
    enum token_type type; // The kind of token
    char *value; // If the token is a word, its value, allocated on the heap !
};

/*
 * @brief Return the string representation of a token type
 */
char *token_type_to_str(enum token_type type);

/*
 * @brief Return true if the token is a reserved word
 */
int is_token_reserved_word(struct token *token);

#endif /* !TOKEN_H */
