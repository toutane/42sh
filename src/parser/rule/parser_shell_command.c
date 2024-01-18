#include "../parser.h"

enum parser_status parse_rule_if(struct ast **res, struct lexer *lexer);
enum parser_status parse_rule_while(struct ast **res, struct lexer *lexer);
enum parser_status parse_rule_until(struct ast **res, struct lexer *lexer);
enum parser_status parse_rule_for(struct ast **res, struct lexer *lexer);

/**
 * @brief Parse a shell rule
 *
 * shell_command =  rule_if
 *                  | rule_while
 *                  | rule_until
 *                  | rule_for
 *                  ;
 */
enum parser_status parse_shell_command(struct ast **res, struct lexer *lexer)
{
    // rule_if
    if (parse_rule_if(res, lexer) == PARSER_OK)
    {
        return PARSER_OK;
    }
    else if (parse_rule_while(res, lexer) == PARSER_OK)
    {
        return PARSER_OK;
    }
    else if (parse_rule_until(res, lexer) == PARSER_OK)
    {
        return PARSER_OK;
    }
    else if (parse_rule_for(res, lexer) == PARSER_OK)
    {
        return PARSER_OK;
    }
    return PARSER_UNEXPECTED_TOKEN;
}
