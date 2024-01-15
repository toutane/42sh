#include "parser.h"

/**
 * @brief Parse a shell_command
 *
 * shell_command =  rule_if ;
 */
enum parser_status parse_shell_command(struct ast **res,
                                              struct lexer *lexer);

/**
 * @brief Parse a WORD, followed by any number of element
 *
 * simple_command = WORD { element } ;
 */
enum parser_status parse_simple_command(struct ast **res,
                                               struct lexer *lexer);

enum parser_status parse_command(struct ast **res, struct lexer *lexer)
{
    // simple_command
    // | shell_command
    if (parse_simple_command(res, lexer) == PARSER_OK)
    {
        return PARSER_OK;
    }
    else if (parse_shell_command(res, lexer) == PARSER_OK)
    {
        return PARSER_OK;
    }
    return PARSER_UNEXPECTED_TOKEN;
}
