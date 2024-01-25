#include "../parser.h"

/*
 * rule_for =
 * 'for' WORD ( [';'] | [ {'\n'} 'in' { WORD } ( ';' | '\n' ) ] )
 *      {'\n'} 'do' compound_list
 * 'done' ;
 */
enum parser_status parse_rule_for(struct ast **res, struct lexer *lexer)
{
    if (lexer_peek(lexer).type == TOKEN_FOR)
    {
        struct ast *for_node = calloc(1, sizeof(struct ast_for));
        for_node->type = AST_FOR;

        // Pop 'for'
        lexer_pop(lexer);

        if (lexer_peek(lexer).type == TOKEN_WORD)
        {
            fill_for_node_condition(for_node, lexer_peek(lexer).value);
            lexer_pop(lexer);

            if (lexer_peek(lexer).type == TOKEN_SEMICOLON)
            {
                lexer_pop(lexer);
            }
            else if (lexer->cur_tok.type == TOKEN_NEWLINE
                     || lexer->cur_tok.type == TOKEN_IN)
            {
                while (lexer_peek(lexer).type == TOKEN_NEWLINE)
                {
                    lexer_pop(lexer);
                }
                if (lexer_peek(lexer).type == TOKEN_IN)
                {
                    lexer_pop(lexer);
                }
                else
                {
                    return PARSER_UNEXPECTED_TOKEN;
                }

                while (lexer_peek(lexer).type == TOKEN_WORD)
                {
                    // Treat word in node
                    fill_for_node_data(for_node, lexer_peek(lexer).value);
                    lexer_pop(lexer);
                }

                if (lexer_peek(lexer).type == TOKEN_SEMICOLON
                    || lexer->cur_tok.type == TOKEN_NEWLINE)
                {
                    lexer_pop(lexer);
                }
                else
                {
                    ast_free(for_node);
                    return PARSER_UNEXPECTED_TOKEN;
                }
            }

            // {'\n'}
            while (lexer_peek(lexer).type == TOKEN_NEWLINE)
            {
                lexer_pop(lexer);
            }
            if (lexer_peek(lexer).type == TOKEN_DO)
            {
                // Pop 'do'
                lexer_pop(lexer);

                if (parse_compound_list(res, lexer) == PARSER_OK)
                {
                    fill_for_node_child(for_node, *res);

                    if (lexer_peek(lexer).type == TOKEN_DONE)
                    {
                        // Pop 'done'
                        lexer_pop(lexer);

                        *res = for_node;
                        return PARSER_OK;
                    }
                }
            }
        }

        ast_free(for_node);
        *res = NULL;
    }
    return PARSER_UNEXPECTED_TOKEN;
}
