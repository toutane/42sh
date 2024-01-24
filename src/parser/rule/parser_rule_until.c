#include "../parser.h"

/**
 * @brief Parse an until statement
 *
 * rule_until =     'until' compound_list 'do' compound_list 'done' ;
 */
enum parser_status parse_rule_until(struct ast **res, struct lexer *lexer)
{
    // 'until' compound_list 'do' compound_list 'done'
    if (lexer_peek(lexer).type == TOKEN_UNTIL)
    {
        struct ast *until_node = calloc(1, sizeof(struct ast_until));
        until_node->type = AST_UNTIL;

        // Pop 'until'
        lexer_pop(lexer);

        if (parse_compound_list(res, lexer) == PARSER_OK)
        {
            fill_if_node(until_node, *res);
            if (lexer_peek(lexer).type == TOKEN_DO)
            {
                // Pop 'do'
                lexer_pop(lexer);

                if (parse_compound_list(res, lexer) == PARSER_OK)
                {
                    fill_if_node(until_node, *res);

                    if (lexer_peek(lexer).type == TOKEN_DONE)
                    {
                        // Pop 'done'
                        lexer_pop(lexer);

                        *res = until_node;
                        return PARSER_OK;
                    }
                }
            }
        }
        ast_free(until_node);
        *res = NULL;
    }
    return PARSER_UNEXPECTED_TOKEN;
}
