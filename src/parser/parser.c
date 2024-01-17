#include "parser.h"

enum parser_status parse_input(struct ast **res, struct lexer *lexer);

enum parser_status parse(struct ast **res, struct lexer *lexer)
{
    return parse_input(res, lexer);
}
