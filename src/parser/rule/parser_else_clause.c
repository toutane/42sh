#include "parser.h"

/**
 * @brief Parse a else_clause
 *
 * compound_list =  and_or [';'] {'\n'} ;
 */
enum parser_status parse_compound_list(struct ast **res,
                                              struct lexer *lexer);

enum parser_status parse_else_clause(struct ast **res,
                                            struct lexer *lexer)
{
    // 'else' compound_list
    // | 'elif' compound_list 'then' compound_list [else_clause]
    lexer_peek(lexer);
    if (lexer->cur_tok.type == TOKEN_ELSE)
    {
        // Pop 'else'
        lexer_pop(lexer);

        if (parse_compound_list(res, lexer) == PARSER_OK)
        {
            return PARSER_OK;
        }
    }
    else if (lexer->cur_tok.type == TOKEN_ELIF)
    {
        // Create IF node
        struct ast *elif_node = calloc(1, sizeof(struct ast));
        init_if_node(elif_node);

        // Pop 'elif'
        lexer_pop(lexer);

        if (parse_compound_list(res, lexer) == PARSER_OK)
        {
            fill_if_node(elif_node, *res);
            if (lexer_peek(lexer).type == TOKEN_THEN)
            {
                // Pop 'then'
                lexer_pop(lexer);

                if (parse_compound_list(res, lexer) == PARSER_OK)
                {
                    fill_if_node(elif_node, *res);

                    if (parse_else_clause(res, lexer) == PARSER_OK)
                    {
                        fill_if_node(elif_node, *res);
                    }

                    *res = elif_node;
                    return PARSER_OK;
                }
            }
        }
        ast_free(elif_node);
        *res = NULL;
        // *res = elif_node;
    }
    return PARSER_UNEXPECTED_TOKEN;
}
