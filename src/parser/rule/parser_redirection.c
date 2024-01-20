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
    struct ast_redirection *redir_node =
        calloc(1, sizeof(struct ast_redirection));
    init_redirection_node(&redir_node->base);

    if (lexer_peek(lexer).type == TOKEN_IONUMBER)
    {
        /// Get IONUMBER
        fill_redirection_node_ionumber(&redir_node->base,
                                       atoi(lexer->cur_tok.value));
        lexer_pop(lexer);
    }
    if (is_redirection_operator(lexer_peek(lexer)))
    {
        /// Add redirection to ast
        fill_redirection_node_type(&redir_node->base, lexer->cur_tok);
        lexer_pop(lexer);
        if (lexer_peek(lexer).type == TOKEN_WORD)
        {
            /// Add WORD to ast
            fill_redirection_node_target(&redir_node->base,
                                         lexer->cur_tok.value);
            lexer_pop(lexer);

            // OLD !
            // Add ast to the node
            // redir_node->next = *res;

            *res = &redir_node->base;

            return PARSER_OK;
        }
    }

    // Free node
    // *res = redir_node->next;
    // redir_node->next = NULL;
    ast_free(&redir_node->base);
    return PARSER_UNEXPECTED_TOKEN;
}
