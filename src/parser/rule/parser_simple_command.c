#include "parser.h"

/**
 * @brief Parse a WORD
 *
 * element =        WORD ;
 */
enum parser_status parse_element(struct ast **res, struct lexer *lexer);

enum parser_status parse_simple_command(struct ast **res, struct lexer *lexer)
{
    // | WORD { element }
    lexer_peek(lexer);
    if (lexer->cur_tok.type == TOKEN_WORD)
    {
        struct ast *sc_node = calloc(1, sizeof(struct ast));
        // TODO: Check for NULL after allocation try

        // Fill node
        fill_sc_node(sc_node, lexer);
        // replace AST
        *res = sc_node;

        // Pop first WORD
        lexer_pop(lexer);

        // { element }
        while (parse_element(res, lexer) == PARSER_OK)
        {
            continue;
        }
        return PARSER_OK;
    }
    return PARSER_UNEXPECTED_TOKEN;
}
