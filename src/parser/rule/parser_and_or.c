#include "parser.h"

/**
 * @brief Parse a command
 *
 * pipeline =       command ;
 */
enum parser_status parse_pipeline(struct ast **res, struct lexer *lexer);

enum parser_status parse_and_or(struct ast **res, struct lexer *lexer)
{
    // | pipeline
    if (parse_pipeline(res, lexer) == PARSER_OK)
    {
        return PARSER_OK;
    }
    return PARSER_UNEXPECTED_TOKEN;
}
