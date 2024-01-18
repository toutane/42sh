#include "../parser.h"

/*
 * @brief Parse a possible IONUMBER followed by an operator and a WORD
 *
 * redirection = [IONUMBER] ( '>' | '<' | '>>' | '>&' | '<&' | '>|' | '<>' )
 *                                                                      WORD ;
 */
enum parser_status parse_redirection(struct ast **res, struct lexer *lexer)
{
    // Create AST node
    struct ast *redir_node = calloc(1, sizeof(struct ast_redirection));
    init_redirection_node(redir_node);

    if (lexer_peek(lexer).type == TOKEN_IONUMBER)
    {
        /// Get IONUMBER
        fill_redirection_node_ionumber(redir_node, atoi(lexer->cur_tok.value));
        lexer_pop(lexer);
    }
    if (is_redirection_operator(lexer_peek(lexer)))
    {
        /// Add redirection to ast
        fill_redirection_node_type(redir_node, lexer->cur_tok);
        lexer_pop(lexer);
        if (lexer_peek(lexer).type == TOKEN_WORD)
        {
            /// Add WORD to ast
            fill_redirection_node_target(redir_node, lexer->cur_tok.value);
            lexer_pop(lexer);

            *res = redir_node;

            return PARSER_OK;
        }
    }

    // Free node
    ast_free(redir_node);
    *res = NULL;
    return PARSER_UNEXPECTED_TOKEN;
}
