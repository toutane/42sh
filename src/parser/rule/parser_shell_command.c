#include "../parser.h"

/**
 * @brief Parse a shell rule
 *
 * shell_command =  '{' compound_list '}'
 *                  | rule_if
 *                  | rule_while
 *                  | rule_until
 *                  | rule_for
 *                  ;
 */
enum parser_status parse_shell_command(struct ast **res, struct lexer *lexer)
{
    if (lexer_peek(lexer).type == TOKEN_LBRACE)
    {
        lexer_pop(lexer);

        if (parse_compound_list(res, lexer) == PARSER_OK)
        {
            if (lexer_peek(lexer).type == TOKEN_RBRACE)
            {
                lexer_pop(lexer);
                return PARSER_OK;
            }
        }

        return PARSER_UNEXPECTED_TOKEN;
    }
    else if (parse_rule_if(res, lexer) == PARSER_OK)
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
