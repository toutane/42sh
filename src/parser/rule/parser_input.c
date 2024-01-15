#include "parser.h"

/**
 * @brief Parse a and_or
 *
 * list =           and_or { ';' and_or } [ ';' ] ;
 */
enum parser_status parse_list(struct ast **res, struct lexer *lexer);

enum parser_status parse(struct ast **res, struct lexer *lexer)
{
    lexer_peek(lexer);
    // | '\n'
    // | EOF
    if (lexer->cur_tok.type == TOKEN_NEWLINE
        || lexer->cur_tok.type == TOKEN_EOF)
    {
        return PARSER_OK;
    }
    // | list '\n'
    // | list EOF
    if (parse_list(res, lexer) == PARSER_OK)
    {
        // Checked popped
        lexer_peek(lexer);
        if (lexer->cur_tok.type == TOKEN_NEWLINE
            || lexer->cur_tok.type == TOKEN_EOF)
        {
            return PARSER_OK;
        }
    }
    fprintf(stderr, "Syntax Error !\n");
    return PARSER_UNEXPECTED_TOKEN;
}
