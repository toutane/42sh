#define _XOPEN_SOURCE 500

#include "parser.h"

void fill_sc_node(struct ast *ast, struct lexer *lexer)
{
    struct ast_cmd *sc_node = (struct ast_cmd *)ast;

    if (sc_node->argc == 0)
    {
        // Init simple-command node
        sc_node->base->type = AST_SIMPLE_COMMAND;
        sc_node->argc = 2;
        sc_node->argv = malloc(sc_node->argc * sizeof(char *));
        if (!sc_node)
        {
            exit(1);
        }

        sc_node->argv[0] = strdup(lexer->cur_tok.value);
        sc_node->argv[sc_node->argc - 1] = NULL;
    }
    else
    {
        // Append element to simple-command node
        sc_node->argc += 1;
        sc_node->argv = realloc(sc_node->argv, sc_node->argc * sizeof(char *));
        if (!sc_node)
        {
            exit(1);
        }

        sc_node->argv[sc_node->argc - 2] = strdup(lexer->cur_tok.value);
        sc_node->argv[sc_node->argc - 1] = NULL;
    }
    return;
}

void fill_list_node(struct ast *ast, struct ast *ast_cmd)
{
    struct ast_cmd_list *ast_cmd_list = (struct ast_cmd_list *)ast;

    while (ast_cmd_list->next)
    {
        ast_cmd_list = (struct ast_cmd_list *)ast_cmd_list->next;
    }

    ast_cmd_list->next = ast_cmd;
    return;
}

/*
void init_if_node(struct ast *ast)
{
    ast->type = AST_CONDITION;
    return;
}
*/

// every if, then and else have to be filled in the right order
void fill_if_node(struct ast *ast, struct ast *ast_child)
{
    struct ast_condition *ast_condition = (struct ast_condition *)ast;

    if (!ast_condition->condition)
    {
        ast_condition->condition = ast_child;
    }
    else if (!ast_condition->then_body)
    {
        ast_condition->then_body = ast_child;
    }
    else
    {
        ast_condition->else_body = ast_child;
    }
    return;
}

/*
void init_redirection_node(struct ast *redirection_node)
{
    redirection_node->type = AST_REDIRECTION;
    redirection_node->nb_args = 3;
    redirection_node->argv = calloc(redirection_node->nb_args, sizeof(char *));
    // TODO CHECK if calloc failed
}
*/

// default case: ionumber set to 1
void fill_redirection_node(struct ast *ast, int ionumber, char *str)
{
    struct ast_redirection *ast_redirection = (struct ast_redirection *)ast;

    if (!str)
    {
        ast_redirection->ionumber = ionumber;
    }
    else
    {
        ast_redirection->data = strdup(str);
    }
    return;
}

/*
void init_pipeline_node(struct ast *pipe_node)
{
    pipe_node->type = AST_PIPELINE;
    pipe_node->nb_child = 2;
    pipe_node->children = calloc(2, sizeof(struct ast *));
    // TODO CHECK if calloc failed
}
*/

void fill_pipeline_node(struct ast *ast, struct ast *ast_child)
{
    struct ast_pipeline *ast_pipeline = (struct ast_pipeline *)ast;

    if (!ast_pipeline->left)
    {
        ast_pipeline->left = ast_child;
    }
    else
    {
        ast_pipeline->left = ast_child;
    }

    return;
}
