#include "../parser.h"

/**
 * @brief Parse either a list, or newline
 *
 * input =     list '\n'
 *          |  list EOF
 *          | '\n'
 *          | EOF
 *          ;
 */
enum parser_status parse_input(struct ast **res, struct lexer *lexer)
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
    fprintf(stderr, "42sh: syntax error\n");
    return PARSER_UNEXPECTED_TOKEN;
}
