#ifndef AST_H
#define AST_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    AST_WHILE,
    AST_UNTIL,
    AST_FOR,
    AST_REDIRECTION,
    AST_NEG,
    AST_AND,
    AST_OR,
    AST_SUBSHELL,
    AST_FUNC
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
    char **prefix;
    int prefix_count;
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
    // /!\ 1 if no ionumber during parsing
    int ionumber;
    char *target;
    struct ast *next; // -> next -> NULL
};

struct ast_neg
{
    struct ast base;
    struct ast *data;
};

struct ast_for
{
    struct ast base;

    // store the first word in the grammar
    char *condition;

    // if IN exists, then store data in array
    char **array;
    size_t array_size;

    struct ast *data;
};

struct ast_and_or
{
    struct ast base;
    struct ast *left;
    struct ast *right;
};

struct ast_while
{
    struct ast base;
    struct ast *condition;
    struct ast *data;
};

struct ast_until
{
    struct ast base;
    struct ast *condition;
    struct ast *data;
};

<<<<<<< HEAD
struct ast_subshell
{
    struct ast base;
    struct ast *compound_list;
=======
struct ast_func
{
    struct ast base;
    char *name;
    struct ast *shell_command;
>>>>>>> develop
};

/**
 * @brief Frees the ast.
 */
void ast_free(void *ast);

/**
 * @brief Fill the ast.
 */
void fill_sc_node_arg(struct ast *ast, struct lexer *lexer);
void fill_sc_node_redir(struct ast *ast, struct lexer *lexer);
void fill_list_node(struct ast *ast, struct ast *ast_cmd);
void fill_if_node(struct ast *ast, struct ast *ast_child);
void fill_redirection_node(struct ast *ast, int ionumber, char *str);
void fill_pipeline_node(struct ast *ast, struct ast *ast_child);
void fill_for_node_condition(struct ast *ast, char *data);
void fill_for_node_data(struct ast *ast, char *data);
void fill_for_node_child(struct ast *ast, struct ast *ast_child);
void fill_word_func(struct ast *ast, char *func_name);
void fill_func_node(struct ast *ast, struct ast *ast_child);

void init_redirection_node(struct ast *ast);
void fill_redirection_node_ionumber(struct ast *ast, int ionumber);
void fill_redirection_node_type(struct ast *ast, struct token tok);
void fill_redirection_node_target(struct ast *ast, char *target);

#endif /* ! AST_H */
