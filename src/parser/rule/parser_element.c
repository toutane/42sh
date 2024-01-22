#include "../parser.h"

/**
 * @brief Parse a WORD
 *
 * element =        WORD
 *                  | redirection
 *                  ;
 */
enum parser_status parse_element(struct ast **res, struct lexer *lexer)
{
    // | WORD
    lexer_peek(lexer);
    if (lexer->cur_tok.type == TOKEN_WORD || is_reserved_word(lexer->cur_tok))
    {
        // Append cur_tok to AST simple-command node
        fill_sc_node(*res, lexer, 1);

        // Pop element
        lexer_pop(lexer);

        return PARSER_OK;
    }
    else if (parse_redirection(res, lexer) == PARSER_OK)
    {
        return PARSER_OK;
    }
    return PARSER_UNEXPECTED_TOKEN;
}
