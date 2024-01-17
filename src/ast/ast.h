#ifndef AST_H
#define AST_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "../error_handling/error_handling_execvp.h"
#include "../utils/token/token.h"

enum ast_type
{
    AST_SIMPLE_COMMAND,
    AST_COMMAND_LIST,
    AST_CONDITION,
    AST_REDIRECTION,
    AST_PIPELINE,
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
 * @brief Frees the ast.
 */
void ast_free(struct ast *ast);

#endif /* ! AST_H */
