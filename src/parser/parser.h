#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>
#include <string.h>

#include "../ast/ast.h"
#include "../lexer/lexer.h"
#include "../lexer/token.h"

enum parser_status
{
    PARSER_OK,
    PARSER_UNEXPECTED_TOKEN,
};

/**
 * @brief Parse either a list, or newline
 *
 * input =     list '\n'
 *          |  list EOF
 *          | '\n'
 *          | EOF
 *          ;
 */
enum parser_status parse(struct ast **res, struct lexer *lexer);

#endif /* ! PARSER_H */
