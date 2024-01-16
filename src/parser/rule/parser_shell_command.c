#include "parser.h"

enum parser_status parse_rule_if(struct ast **res, struct lexer *lexer);

/**
 * @brief Parse a shell_command
 *
 * shell_command =  rule_if ;
 */
enum parser_status parse_shell_command(struct ast **res, struct lexer *lexer)
{
    // rule_if
    if (parse_rule_if(res, lexer) == PARSER_OK)
    {
        return PARSER_OK;
    }
    return PARSER_UNEXPECTED_TOKEN;
}
