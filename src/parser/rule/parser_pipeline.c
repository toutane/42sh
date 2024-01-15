#include "parser.h"

/**
 * @brief Parse a simple_command
 *
 * command =        simple_command
                  | shell_command
                  ;
 */
enum parser_status parse_command(struct ast **res, struct lexer *lexer);

enum parser_status parse_pipeline(struct ast **res, struct lexer *lexer)
{
    // | command
    if (parse_command(res, lexer) == PARSER_OK)
    {
        return PARSER_OK;
    }
    return PARSER_UNEXPECTED_TOKEN;
}
