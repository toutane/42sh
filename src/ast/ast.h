#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "../lexer/token.h"

enum ast_type
{
    AST_SIMPLE_COMMAND,
    AST_COMMAND_LIST,
    AST_CONDITION,
    /*
    AST_MUL,
    AST_DIV,
    AST_NUMBER,
    AST_NEG
    */
};

struct ast
{
    enum ast_type type; ///< The kind of node we're dealing with
    char **argv; ///< Content of the AST_SIMPLE_COMMAND
    size_t nb_args; ///< Size of argv field (NULL included)
    struct ast **children; ///< The children list of the node if any
    size_t nb_child; ///< The number of node's children
};

/**
 * @brief Prints the ast (not pretty for the moment).
 */
void ast_print(struct ast *ast);

/**
 * @brief Frees the ast.
 */
void ast_free(struct ast *ast);

/**
 * @brief Evals the ast.
 */
int ast_eval(struct ast *ast);

/**
 * @brief Pretty prints the ast.
 * @param ast The ast to pretty print.
 */
void ast_pretty_print(struct ast *ast);

#endif /* !AST_H */
