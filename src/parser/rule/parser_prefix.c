#include <stdio.h>

#include "../parser.h"

/**
 * @brief Parse a redirection
 *
 * prefix =        redirection ;
 */
enum parser_status parse_prefix(struct ast **res, struct lexer *lexer)
{
    lexer_peek(lexer);
    if (is_assignment_word(&lexer->cur_tok, 1))
    {
        // Put it in the AST
        fill_sc_node_redir(*res, lexer);
        lexer_pop(lexer);

        //*res = NULL;
        return PARSER_OK;
    }
    else if (parse_redirection(res, lexer) == PARSER_OK)
    {
        return PARSER_OK;
    }
    return PARSER_UNEXPECTED_TOKEN;
}
