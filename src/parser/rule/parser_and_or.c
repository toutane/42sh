#include "parser.h"

enum parser_status parse_pipeline(struct ast **res, struct lexer *lexer);

/**
 * @brief Parse a pipeline
 *
 * and_or =         pipeline ;
 */
enum parser_status parse_and_or(struct ast **res, struct lexer *lexer)
{
    // | pipeline
    if (parse_pipeline(res, lexer) == PARSER_OK)
    {
        return PARSER_OK;
    }
    return PARSER_UNEXPECTED_TOKEN;
}
