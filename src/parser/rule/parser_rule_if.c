#include "parser.h"

/**
 * @brief Parse a else_clause
 *
 * compound_list =  and_or [';'] {'\n'} ;
 */
enum parser_status parse_compound_list(struct ast **res, struct lexer *lexer);

/**
 * @brief Parse a else_clause
 *
 * else_clause =    'else' compound_list
 *                | 'elif' compound_list 'then' compound_list [else_clause]
 *                ;
 */
enum parser_status parse_else_clause(struct ast **res, struct lexer *lexer);

enum parser_status parse_rule_if(struct ast **res, struct lexer *lexer)
{
    // 'if' compound_list 'then' compound_list [else_clause] 'fi'
    if (lexer_peek(lexer).type == TOKEN_IF)
    {
        // Create IF node
        struct ast *if_node = calloc(1, sizeof(struct ast));
        init_if_node(if_node);

        // Pop 'if'
        lexer_pop(lexer);

        if (parse_compound_list(res, lexer) == PARSER_OK)
        {
            fill_if_node(if_node, *res);
            if (lexer_peek(lexer).type == TOKEN_THEN)
            {
                // Pop 'then'
                lexer_pop(lexer);

                if (parse_compound_list(res, lexer) == PARSER_OK)
                {
                    fill_if_node(if_node, *res);

                    if (parse_else_clause(res, lexer) == PARSER_OK)
                    {
                        fill_if_node(if_node, *res);
                    }
                    if (lexer_peek(lexer).type == TOKEN_FI)
                    {
                        // Pop 'fi'
                        lexer_pop(lexer);

                        *res = if_node;
                        return PARSER_OK;
                    }
                }
            }
        }
        ast_free(if_node);
        *res = NULL;
        // *res = if_node;
    }
    return PARSER_UNEXPECTED_TOKEN;
}