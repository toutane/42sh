#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>

#include "../ast/ast.h"
#include "lexer/lexer.h"
#include "utils/token/token.h"

enum parser_status
{
    PARSER_OK,
    PARSER_UNEXPECTED_TOKEN,
};

/**
 *
 * @brief Parse function, juste call parse_input
 *
 */
enum parser_status parse(struct ast **res, struct lexer *lexer);

#endif /* ! PARSER_H */
