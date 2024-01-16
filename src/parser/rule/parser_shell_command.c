#include "parser.h"

/**
 * @brief Parse a rule_if
 *
 * rule_if =        'if' compound_list 'then' compound_list [else_clause] 'fi' ;
 */
enum parser_status parse_rule_if(struct ast **res, struct lexer *lexer);

enum parser_status parse_shell_command(struct ast **res, struct lexer *lexer)
{
    // rule_if
    if (parse_rule_if(res, lexer) == PARSER_OK)
    {
        return PARSER_OK;
    }
    return PARSER_UNEXPECTED_TOKEN;
}
