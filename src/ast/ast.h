#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "../lexer/token.h"

enum ast_type
{
    AST_SIMPLE_COMMAND,
    AST_CONDITION
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
    struct ast *left; ///< The left branch if any, unuary or binary
    struct ast *right; ///< The right branch of the binary node
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

#endif /* !AST_H */
