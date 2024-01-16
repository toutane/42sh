#include "parser.h"

enum parser_status parse_redirection(struct ast **res, struct lexer *lexer);

/**
 * @brief Parse a redirection
 *
 * prefix =        redirection ;
 */
enum parser_status parse_prefix(struct ast **res, struct lexer *lexer)
{
    if (parse_redirection(res, lexer) == PARSER_OK)
    {
        return PARSER_OK;
    }
    return PARSER_UNEXPECTED_TOKEN;
}
