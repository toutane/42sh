#include <stdio.h>

#include "../parser.h"

/**
 * @brief Parse a redirection
 *
 * prefix =        redirection ;
 */
enum parser_status parse_prefix(struct ast **res, struct lexer *lexer)
{
    struct token token = lexer_peek(lexer);
    if (is_assignment_word(&token, 1))
    {
        // TODO: Create node for ASSIGNMENT_WORD
        // Put it in the AST
        lexer_pop(lexer);
        return PARSER_OK;
    }
    else if (parse_redirection(res, lexer) == PARSER_OK)
    {
        return PARSER_OK;
    }
    return PARSER_UNEXPECTED_TOKEN;
}
