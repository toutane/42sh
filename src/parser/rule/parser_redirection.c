#include "parser.h"

/*
 * @brief Parse a possible IONUMBER followed by an operator and a WORD
 *
 * redirection = [IONUMBER] ( '>' | '<' | '>>' | '>&' | '<&' | '>|' | '<>' )
 *                                                                      WORD ;
 */
enum parser_status parse_redirection(struct ast **res, struct lexer *lexer)
{
    // TODO: Create AST node
    if (lexer_peek(lexer).type == TOKEN_IONUMBER)
    {
        /// Get IONUMBER
        lexer_pop(lexer);
    }
    if (is_redirection_word(lexer_peek(lexer).type))
    {
        /// Add redirection to ast
        lexer_pop(lexer);
        if (lexer_peek(lexer).type == TOKEN_WORD)
        {
            /// Add WORD to ast
            lexer_pop(lexer);
            return PARSER_OK;
        }
    }
    // Free node
    return PARSER_UNEXPECTED_TOKEN;
}
