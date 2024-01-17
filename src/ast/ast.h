#ifndef AST_H
#define AST_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

#include "../error_handling/error_handling_execvp.h"
#include "../utils/token/token.h"
#include "../lexer/lexer.h"

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

/**
 * @brief init an pipelie node.
 */
void init_pipeline_node(struct ast *if_node);

/**
 * @brief init an pipelie node.
 */
void init_pipeline_node(struct ast *if_node);

/**
 * @brief init an if node.
 */
void init_if_node(struct ast *if_node);

/**
 * @brief init an redirection node.
 */
void init_redirection_node(struct ast *redirection_node);

/**
 * @brief fill an sc node.
 */
void fill_sc_node(struct ast *sc_node, struct lexer *lexer);

/**
 * @brief fill an list node.
 */
void fill_list_node(struct ast *list_node, struct ast *sc_node);

/**
 * @brief fill an if node.
 */
void fill_if_node(struct ast *if_node, struct ast *node);

/**
 * @brief fill an redirection node.
 */
void fill_redirection_node(struct ast *redirection_node, size_t position, char *str);

/**
 * @brief fill an pipeline node.
 */
void fill_pipeline_node(struct ast *if_node, struct ast *node);


#endif /* ! AST_H */
