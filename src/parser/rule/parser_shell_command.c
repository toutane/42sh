#include "parser.h"

static char create_subshell(struct ast **res, struct lexer *lexer)
{
    if (parse_compound_list(res, lexer) == PARSER_OK)
    {
        struct ast_subshell *node_subshell =
            calloc(1, sizeof(struct ast_subshell));
        node_subshell->base.type = AST_SUBSHELL;

        node_subshell->compound_list = *res;

        *res = (struct ast *)node_subshell;
        return 1;
    }
    return 0;
}

/**
 * @brief Parse a shell rule
 *
 * shell_command =  '{' compound_list '}'
 *                  | '(' compound_list ')'
 *                  | rule_for
 *                  | rule_while
 *                  | rule_until
 *                  | rule_if
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

        ast_free(*res);
        *res = NULL;
        return PARSER_FAIL;
    }
    else if (lexer->cur_tok.type == TOKEN_LPAREN)
    {
        lexer_pop(lexer);

        if (create_subshell(res, lexer))
        {
            if (lexer->cur_tok.type == TOKEN_RPAREN)
            {
                lexer_pop(lexer);
                return PARSER_OK;
            }
        }

        ast_free(*res);
        *res = NULL;
        return PARSER_FAIL;
    }
    else if (parse_rule_for(res, lexer) == PARSER_OK)
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
    else if (parse_rule_if(res, lexer) == PARSER_OK)
    {
        return PARSER_OK;
    }
    else if (parse_rule_case(res, lexer) == PARSER_OK)
    {
        return PARSER_OK;
    }
    return PARSER_UNEXPECTED_TOKEN;
}
