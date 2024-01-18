#include "../parser.h"

/**
 * @brief Parse a pipeline
 *
 * and_or =         pipeline ;
 */
enum parser_status parse_list(struct ast **res, struct lexer *lexer)
{
    // | and_or
    if (parse_and_or(res, lexer) == PARSER_OK)
    {
        // Create list node for AST
        struct ast *list_node = calloc(1, sizeof(struct ast_cmd_list));
        list_node->type = AST_COMMAND_LIST;

        // Add previously parsed `and_or` to AST
        fill_list_node(list_node, *res);

        // { ';' and_or } [ ';' ] ;
        while (lexer_peek(lexer).type == TOKEN_SEMICOLON)
        {
            // Consume ';' token
            lexer_pop(lexer);

            if (parse_and_or(res, lexer) == PARSER_OK)
            {
                // { ';' and_or } case
                // Add previously parsed `and_or` to AST
                fill_list_node(list_node, *res);
            }
            else
            {
                // [ ';' ] case
                break;
            }
        }
        // Replace AST by list node
        *res = list_node;

        return PARSER_OK;
    }
    return PARSER_UNEXPECTED_TOKEN;
}
