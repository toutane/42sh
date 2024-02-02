#include "parser.h"

// juste call the parse_input rull
enum parser_status parse(struct ast **res, struct lexer *lexer)
{
    return parse_input(res, lexer);
}
