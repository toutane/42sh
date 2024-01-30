#include "../parser.h"

/**
 * @brief Parse a command
 *
 * pipeline =       ['!'] command { '|' {'\n'} command } ;
 */
enum parser_status parse_pipeline(struct ast **res, struct lexer *lexer)
{
    struct ast *neg_node = NULL;

    // ['!']
    enum parser_status parse_status = PARSER_OK;
    if (lexer_peek(lexer).type == TOKEN_NEG)
    {
        neg_node = calloc(1, sizeof(struct ast_neg));
        neg_node->type = AST_NEG;

        // Create negation node
        lexer_pop(lexer);
    }
    // command
    if ((parse_status = parse_command(res, lexer)) == PARSER_OK)
    {
        // { '|' {'\n'} command }
        while (lexer_peek(lexer).type == TOKEN_PIPE)
        {
            // Create pipe node
            struct ast *pipe_node = calloc(1, sizeof(struct ast_pipeline));
            pipe_node->type = AST_PIPELINE;

            fill_pipeline_node(pipe_node, *res);
            lexer_pop(lexer);
            while (lexer_peek(lexer).type == TOKEN_NEWLINE)
            {
                lexer_pop(lexer);
            };
            if ((parse_status = parse_command(res, lexer)) == PARSER_OK)
            {
                fill_pipeline_node(pipe_node, *res);
                *res = pipe_node;

                continue;
            }

            // Free node(s)
            ast_free(pipe_node);
            ast_free(neg_node);
            *res = NULL;
            return parse_status;
        }

        // assign to possible negation node
        if (neg_node)
        {
            ((struct ast_neg *)neg_node)->data = *res;
            *res = neg_node;
        }

        return PARSER_OK;
    }
    // Free node(s)
    ast_free(neg_node);
    return parse_status;
}
