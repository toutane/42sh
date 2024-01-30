#include "parser.h"

// create the and or node
static void create_and_or_node(struct ast_and_or **node, struct lexer *lexer)
{
    // build and node
    *node = calloc(1, sizeof(struct ast_and_or));

    // check if calloc failed
    if (node == NULL)
    {
        err(1, "calloc fail for node and\n");
    }

    if (lexer->cur_tok.type == TOKEN_AND)
    {
        (*node)->base.type = AST_AND;
    }
    else
    {
        (*node)->base.type = AST_OR;
    }
}

/**
 * @brief Parse one or more pipelines separated by operators && or ||
 *
 * and_or =         pipeline { ( '&&' | '||' ) {'\n'} pipeline } ;
 */
enum parser_status parse_and_or(struct ast **res, struct lexer *lexer)
{
    // | pipeline
    enum parser_status parse_status = PARSER_OK;
    if ((parse_status = parse_pipeline(res, lexer)) == PARSER_OK)
    {
        // { ( '&&' | '||' ) {'\n'} pipeline }
        while (lexer_peek(lexer).type == TOKEN_AND
               || lexer->cur_tok.type == TOKEN_OR)
        {
            // Create AND/OR node
            struct ast_and_or *ast_and_or = NULL;
            create_and_or_node(&ast_and_or, lexer);
            lexer_pop(lexer);

            // fill and or left node
            ast_and_or->left = *res;

            while (lexer_peek(lexer).type == TOKEN_NEWLINE)
            {
                lexer_pop(lexer);
            };

            // parse first pipeline node
            if ((parse_status = parse_pipeline(res, lexer)) == PARSER_OK)
            {
                // fill and or right node
                ast_and_or->right = *res;
                *res = (struct ast *)ast_and_or;
                continue;
            }

            // Free node(s)
            ast_free((struct ast *)ast_and_or);
            *res = NULL;
            return parse_status;
        }
        return PARSER_OK;
    }
    // Free node(s)
    return parse_status;
}
