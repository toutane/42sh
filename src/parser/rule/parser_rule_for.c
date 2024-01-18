#include "../parser.h"

/*
 * @brief Parse a for statement
 *
 * rule_for =       'for' WORD
 *                      ( [';'] | [ {'\n'} 'in' { WORD } ( ';' | '\n' ) ] )
 *                                          {'\n'} 'do' compound_list 'done' ;
 */
enum parser_status parse_rule_for(struct ast **res, struct lexer *lexer)
{
    if (lexer_peek(lexer).type == TOKEN_FOR)
    {
        // TODO: Create FOR node
        // struct ast *if_node = calloc(1, sizeof(struct ast));
        // init_if_node(if_node);

        // Pop 'for'
        lexer_pop(lexer);

        if (lexer_peek(lexer).type == TOKEN_WORD)
        {
            // fill_if_node(if_node, *res);
            lexer_pop(lexer);

            if (lexer_peek(lexer).type == TOKEN_SEMICOLON)
            {
                // [';']
                lexer_pop(lexer);
            }
            else if (lexer->cur_tok.type == TOKEN_NEWLINE
                     || lexer->cur_tok.type == TOKEN_IN)
            {
                // [ {'\n'} 'in' { WORD } ( ';' | '\n' ) ]
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
                    lexer_pop(lexer);
                }
                if (lexer_peek(lexer).type == TOKEN_SEMICOLON
                    || lexer->cur_tok.type == TOKEN_NEWLINE)
                {
                    lexer_pop(lexer);
                }
                else
                {
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
                    // fill_if_node(if_node, *res);

                    if (lexer_peek(lexer).type == TOKEN_DONE)
                    {
                        // Pop 'done'
                        lexer_pop(lexer);

                        // *res = if_node;
                        return PARSER_OK;
                    }
                }
            }
        }
        // ast_free(if_node);
        //*res = NULL;
    }
    return PARSER_UNEXPECTED_TOKEN;
}
