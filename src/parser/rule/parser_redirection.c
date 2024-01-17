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
    struct ast *redir_node = calloc(1, sizeof(struct ast));
    init_redirection_node(redir_node);

    if (lexer_peek(lexer).type == TOKEN_IONUMBER)
    {
        /// Get IONUMBER
        fill_redirection_node(redir_node, 2, lexer->cur_tok.value);
        lexer_pop(lexer);
    }
    if (is_redirection_word(lexer_peek(lexer).type))
    {
        /// Add redirection to ast
        fill_redirection_node(redir_node, 0, lexer->cur_tok.value);
        lexer_pop(lexer);
        if (lexer_peek(lexer).type == TOKEN_WORD)
        {
            /// Add WORD to ast
            fill_redirection_node(redir_node, 1, lexer->cur_tok.value);
            lexer_pop(lexer);

            *res = redir_node;

            return PARSER_OK;
        }
    }

    // Free node
    ast_free(redir_node);
    return PARSER_UNEXPECTED_TOKEN;
}
