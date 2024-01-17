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

enum redirection_type
{
    REDIR_GREAT,
    REDIR_LESS,
    REDIR_DGREAT,
    REDIR_GREATAND,
    REDIR_LESSAND,
    REDIR_CLOBBER,
    REDIR_LESSGREAT
};

struct ast
{
    enum ast_type type;
};

struct ast_cmd
{
    struct ast *base;
    char **argv;
    int argc;
};

struct ast_condition
{
    struct ast *base;
    struct ast *condition;
    struct ast *then_body;
    struct ast *else_body;
};

struct ast_cmd_list
{
    struct ast *base;
    // ast_cmd
    struct ast *cmd;
    // ast_cmd_list
    struct ast *next;
};

struct ast_pipeline
{
    struct ast *base;
    struct ast *left;
    struct ast *right;
};

struct ast_redirection
{
    struct ast *base;
    enum redirection_type redirection_type;
    // -1 if no ionumber during parsing
    int ionumber;
    char *data;
};

/*
struct ast
{
    enum ast_type type; ///< The kind of node we're dealing with
    char **argv; ///< Content of the AST_SIMPLE_COMMAND
    size_t nb_args; ///< Size of argv field (NULL included)
    struct ast **children; ///< The children list of the node if any
    size_t nb_child; ///< The number of node's children
};
*/

/**
 * @brief Frees the ast.
 */
void ast_free(struct ast *ast);

#endif /* ! AST_H */
