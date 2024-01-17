#include "../parser.h"

enum parser_status parse_element(struct ast **res, struct lexer *lexer);

enum parser_status parse_prefix(struct ast **res, struct lexer *lexer);

/**
 * @brief Parse any prefix, possibly followed by a WORD and by any number
 * of element
 *
 * simple_command = prefix { prefix }
 *                  | { prefix } WORD { element }
 *                  ;
 */
enum parser_status parse_simple_command(struct ast **res, struct lexer *lexer)
{
    // prefix { prefix }
    // | { prefix } WORD { element }
    char prefixed = 0;
    if (parse_prefix(res, lexer) == PARSER_OK)
    {
        // parse { prefix }
        prefixed = 1;
        while (parse_prefix(res, lexer) == PARSER_OK)
        {
            continue;
        }
    }
    if (lexer_peek(lexer).type == TOKEN_WORD)
    {
        struct ast *sc_node = calloc(1, sizeof(struct ast));
        if (!sc_node)
        {
            return PARSER_UNEXPECTED_TOKEN;
        }
        sc_node->type = AST_SIMPLE_COMMAND;

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
    else
    {
        return (prefixed ? PARSER_OK : PARSER_UNEXPECTED_TOKEN);
    }
    return PARSER_UNEXPECTED_TOKEN;
}
