#include "parser.h"

enum parser_status parse_command(struct ast **res, struct lexer *lexer);

/**
 * @brief Parse a command
 *
 * pipeline =       ['!'] command { '|' {'\n'} command } ;
 */
enum parser_status parse_pipeline(struct ast **res, struct lexer *lexer)
{
    // ['!']
    if (lexer_peek(lexer).type == TOKEN_NEG)
    {
        // Create negation node
        lexer_pop(lexer);
    }
    // command
    if (parse_command(res, lexer) == PARSER_OK)
    {
        // { '|' {'\n'} command }
        while (lexer_peek(lexer).type == TOKEN_PIPE)
        {
            // Create pipe node
            struct ast *pipe_node = calloc(1, sizeof(struct ast));
            init_pipeline_node(pipe_node);

            fill_pipeline_node(pipe_node, *res);
            lexer_pop(lexer);
            while (lexer_peek(lexer).type == TOKEN_NEWLINE)
            {
                lexer_pop(lexer);
            };
            if (parse_command(res, lexer) == PARSER_OK)
            {
                fill_pipeline_node(pipe_node, *res);
                *res = pipe_node;

                continue;
            }
            // Free node(s)
            ast_free(pipe_node);
            return PARSER_UNEXPECTED_TOKEN;
        }
        // Assign to possible negation node
        // then on AST
        return PARSER_OK;
    }
    // Free node(s)
    return PARSER_UNEXPECTED_TOKEN;
}
