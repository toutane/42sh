#include "../parser.h"

/**
 * @brief Parse a while statement
 *
 * rule_while =     'while' compound_list 'do' compound_list 'done' ;
 */
enum parser_status parse_rule_while(struct ast **res, struct lexer *lexer)
{
    // 'while' compound_list 'do' compound_list 'done'
    if (lexer_peek(lexer).type == TOKEN_WHILE)
    {
        struct ast *while_node = calloc(1, sizeof(struct ast_while));
        while_node->type = AST_WHILE;

        // Pop 'while'
        lexer_pop(lexer);

        if (parse_compound_list(res, lexer) == PARSER_OK)
        {
            fill_if_node(while_node, *res);
            if (lexer_peek(lexer).type == TOKEN_DO)
            {
                // Pop 'do'
                lexer_pop(lexer);

                if (parse_compound_list(res, lexer) == PARSER_OK)
                {
                    fill_if_node(while_node, *res);

                    if (lexer_peek(lexer).type == TOKEN_DONE)
                    {
                        // Pop 'done'
                        lexer_pop(lexer);

                        *res = while_node;
                        return PARSER_OK;
                    }
                }
            }
        }
        ast_free(while_node);
        *res = NULL;
    }
    return PARSER_UNEXPECTED_TOKEN;
}
