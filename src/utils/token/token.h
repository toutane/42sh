#ifndef TOKEN_H
#define TOKEN_H

#include <unistd.h>

/*
 * @brief The different types of tokens
 */
enum token_type
{
    TOKEN_NONE, // no token
    TOKEN_ERROR, // error token (when a character is unexpected)

    TOKEN_WORD, // a word
    TOKEN_OPERATOR, // an operator
    TOKEN_NEWLINE, // '\n'
    TOKEN_SEMICOLON, // ';'
    TOKEN_EOF, // end of input marker

    /* Operators */

    TOKEN_GREAT, // '>'
    TOKEN_LESS, // '<'
    TOKEN_DGREAT, // '>>'
    // TOKEN_DLESS, // '<<'
    TOKEN_LESSAND, // '<&'
    TOKEN_GREATAND, // '>&'
    TOKEN_LESSGREAT, // '<>'
    TOKEN_CLOBBER, // '>|'

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
int is_reserved_word(struct token token);

/*
 * @brief Return true if the character can be the first character of an operator
 */
int can_be_first_in_ope(char c);

/*
 * @brief Return true if the character can be the second character of the
 * current operator based on the previous character
 */
int can_be_second_in_ope(char prev, char cur);

/*
 * @brief Return true if the token is an redirection operator
 */
int is_redirection_operator(struct token token);

#endif /* !TOKEN_H */
