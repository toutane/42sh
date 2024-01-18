#ifndef AST_H
#define AST_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "../error_handling/error_handling_execvp.h"
#include "../lexer/lexer.h"
#include "../utils/token/token.h"

enum ast_type
{
    AST_SIMPLE_COMMAND,
    AST_COMMAND_LIST,
    AST_CONDITION,
    AST_PIPELINE,
    AST_REDIRECTION,
    AST_NEG,
    /*
    AST_MUL,
    AST_DIV,
    AST_NUMBER,
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
    struct ast base;
    char **argv;
    int argc;
};

struct ast_condition
{
    struct ast base;
    struct ast *condition;
    struct ast *then_body;
    struct ast *else_body;
};

struct ast_cmd_list
{
    struct ast base;
    // ast_cmd
    struct ast *cmd;
    // ast_cmd_list
    struct ast *next;
};

struct ast_pipeline
{
    struct ast base;
    struct ast *left;
    struct ast *right;
};

struct ast_redirection
{
    struct ast base;
    enum redirection_type redirection_type;
    // -1 if no ionumber during parsing
    int ionumber;
    char *data;
};

struct ast_neg
{
    struct ast base;
    struct ast *data;
};

/**
 * @brief Frees the ast.
 */
void ast_free(struct ast *ast);

/**
 * @brief Fill the ast.
 */
void fill_sc_node(struct ast *ast, struct lexer *lexer);
void fill_list_node(struct ast *ast, struct ast *ast_cmd);
void fill_if_node(struct ast *ast, struct ast *ast_child);
void fill_redirection_node(struct ast *ast, int ionumber, char *str);
void fill_pipeline_node(struct ast *ast, struct ast *ast_child);

#endif /* ! AST_H */
