#include "../parser.h"

enum parser_status parse_simple_command(struct ast **res, struct lexer *lexer);

/**
 * @brief Parse a IONUMBER, followed by a redirection symbol, followed by
 * a word
 *
 * redirection = [IONUMBER] ( '>' | '<' | '>>' | '>&' | '<&' | '>|' | '<>' )
 *                                                                      WORD ;
 */
enum parser_status parse_redirection(struct ast **res, struct lexer *lexer);

/**
 * @brief Parse a simple_command
 *
 * command =        simple_command
                  | shell_command
                  ;
 */
enum parser_status parse_command(struct ast **res, struct lexer *lexer)
{
    // simple_command
    // | shell_command { redirecton }
    if (parse_simple_command(res, lexer) == PARSER_OK)
    {
        return PARSER_OK;
    }
    else if (parse_shell_command(res, lexer) == PARSER_OK)
    {
        while (parse_redirection(res, lexer) == PARSER_OK)
        {
            continue;
        }
        return PARSER_OK;
    }
    return PARSER_UNEXPECTED_TOKEN;
}
