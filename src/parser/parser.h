#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>
#include <string.h>

#include "../ast/ast.h"
#include "../lexer/lexer.h"
#include "../utils/token/token.h"

enum parser_status
{
    PARSER_OK,
    PARSER_UNEXPECTED_TOKEN,
};

void fill_sc_node(struct ast *sc_node, struct lexer *lexer);

void fill_list_node(struct ast *list_node, struct ast *sc_node);

void init_if_node(struct ast *if_node);

void fill_if_node(struct ast *if_node, struct ast *node);

void init_redirection_node(struct ast *redirection_node);

void fill_redirection_node(struct ast *redirection_node, int position,
                           char *str);

void init_pipeline_node(struct ast *if_node);

void fill_pipeline_node(struct ast *if_node, struct ast *node);

/**
 * @brief Parse either a list, or newline
 *
 * input =     list '\n'
 *          |  list EOF
 *          | '\n'
 *          | EOF
 *          ;
 */
enum parser_status parse(struct ast **res, struct lexer *lexer);

#endif /* ! PARSER_H */
