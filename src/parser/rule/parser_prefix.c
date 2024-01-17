#include "parser.h"

enum parser_status parse_redirection(struct ast **res, struct lexer *lexer);

/**
 * @brief Parse a redirection
 *
 * prefix =        redirection ;
 */
enum parser_status parse_prefix(struct ast **res, struct lexer *lexer)
{
    if (lexer_peek(lexer).type == TOKEN_ASSIGNMENT_WORD)
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
