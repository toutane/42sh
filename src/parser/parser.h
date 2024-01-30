#ifndef PARSER_H
#define PARSER_H

#include <err.h>
#include <stdlib.h>
#include <string.h>

#include "ast/ast.h"
#include "lexer/lexer.h"
#include "utils/token/token.h"

enum parser_status
{
    PARSER_OK,
    PARSER_UNEXPECTED_TOKEN,
    PARSER_FAIL,
};

enum parser_status parse(struct ast **res, struct lexer *lexer);

enum parser_status parse_and_or(struct ast **res, struct lexer *lexer);
enum parser_status parse_command(struct ast **res, struct lexer *lexer);
enum parser_status parse_compound_list(struct ast **res, struct lexer *lexer);
enum parser_status parse_element(struct ast **res, struct lexer *lexer);
enum parser_status parse_else_clause(struct ast **res, struct lexer *lexer);
enum parser_status parse_input(struct ast **res, struct lexer *lexer);
enum parser_status parse_list(struct ast **res, struct lexer *lexer);
enum parser_status parse_pipeline(struct ast **res, struct lexer *lexer);
enum parser_status parse_prefix(struct ast **res, struct lexer *lexer);
enum parser_status parse_redirection(struct ast **res, struct lexer *lexer);
enum parser_status parse_rule_for(struct ast **res, struct lexer *lexer);
enum parser_status parse_rule_if(struct ast **res, struct lexer *lexer);
enum parser_status parse_rule_until(struct ast **res, struct lexer *lexer);
enum parser_status parse_rule_while(struct ast **res, struct lexer *lexer);
enum parser_status parse_shell_command(struct ast **res, struct lexer *lexer);
enum parser_status parse_simple_command(struct ast **res, struct lexer *lexer);
enum parser_status parse_fundec(struct ast **res, struct lexer *lexer);

void push_back(struct ast **list, struct ast *to_push);

#endif /* ! PARSER_H */
