#include "../parser.h"

enum parser_status parse_and_or(struct ast **res, struct lexer *lexer);

/**
 * @brief Parse a else_clause
 *
 * compound_list =  and_or [';'] {'\n'} ;
 */
enum parser_status parse_compound_list(struct ast **res, struct lexer *lexer)
{
    // RULE:
    // compound_list = {'\n'} and_or { ( ';' | '\n' ) {'\n'} and_or }[';']{'\n'}

    // {'\n'} case - if the and_or is not found afer {'\n'}, all the {'\n'} are
    // already poped, so CHECK if the grammar is deteministic
    while (lexer_peek(lexer).type == TOKEN_NEWLINE)
    {
        lexer_pop(lexer);
    }

    // and_or
    if (parse_and_or(res, lexer) == PARSER_OK)
    {
        // Create list node for AST
        struct ast *list_node = calloc(1, sizeof(struct ast_cmd_list));
        list_node->type = AST_COMMAND_LIST;

        // Add previously parsed `and_or` to AST
        fill_list_node(list_node, *res);

        //  ( ';' | '\n' )
        while (lexer_peek(lexer).type == TOKEN_SEMICOLON
               || lexer_peek(lexer).type == TOKEN_NEWLINE)
        {
            lexer_pop(lexer);

            // {'\n'}
            while (lexer_peek(lexer).type == TOKEN_NEWLINE)
            {
                lexer_pop(lexer);
            }

            // and_or
            if (parse_and_or(res, lexer) == PARSER_OK)
            {
                fill_list_node(list_node, *res);
            }
            // end of rule
            else
            {
                break;
            }
        }

        *res = list_node;
        return PARSER_OK;
    }
    return PARSER_UNEXPECTED_TOKEN;
}
