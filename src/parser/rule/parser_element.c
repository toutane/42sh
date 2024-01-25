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
    enum parser_status return_status = PARSER_OK;

    // | WORD
    lexer_peek(lexer);
    if (lexer->cur_tok.type == TOKEN_WORD || is_reserved_word(lexer->cur_tok))
    {
        // Append cur_tok to AST simple-command node
        fill_sc_node_arg(*res, lexer);

        // Pop element
        lexer_pop(lexer);

        return PARSER_OK;
    }
    else if ((return_status = parse_redirection(res, lexer)) == PARSER_OK)
    {
        return PARSER_OK;
    }
    return return_status;
}
