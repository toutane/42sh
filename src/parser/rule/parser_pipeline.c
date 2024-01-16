#include "parser.h"

enum parser_status parse_command(struct ast **res, struct lexer *lexer);

/**
 * @brief Parse a command
 *
 * pipeline =       command ;
 */
enum parser_status parse_pipeline(struct ast **res, struct lexer *lexer)
{
    // | command
    if (parse_command(res, lexer) == PARSER_OK)
    {
        return PARSER_OK;
    }
    return PARSER_UNEXPECTED_TOKEN;
}
