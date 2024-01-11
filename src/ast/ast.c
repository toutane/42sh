#include "ast.h"

void ast_print(struct ast *ast)
{
    if (!ast)
    {
        return;
    }
    if (ast->type == AST_SIMPLE_COMMAND)
    {
        printf(":%s:", ast->argv[0]);
        for (size_t i = 1; i < ast->nb_args; i++)
        {
            printf(" %s", ast->argv[i]);
        }
        printf("\n");
    }
    else if (ast->type == AST_COMMAND_LIST)
    {
        printf("List(\n");
        if (ast->nb_child > 0)
        {
            for (size_t i = 0; i < ast->nb_child; i++)
            {
                ast_print(ast->children[i]);
            }
        }
        printf(")\n");
    }
}

void ast_free(struct ast *ast)
{
    if (!ast)
    {
        return;
    }
    if (ast->type == AST_SIMPLE_COMMAND)
    {
        if (ast->nb_args > 0)
        {
            for (size_t i = 0; i < ast->nb_args; i++)
            {
                free(ast->argv[i]);
            }
            free(ast->argv);
            ast->argv = NULL;
        }
    }
    else if (ast->type == AST_COMMAND_LIST)
    {
        if (ast->nb_child > 0)
        {
            for (size_t i = 0; i < ast->nb_child; i++)
            {
                ast_free(ast->children[i]);
            }
            free(ast->children);
            ast->children = NULL;
        }
    }
    free(ast);
}

int eval_sc_node(struct ast *ast)
{
    int status = 0;
    int pid = fork();
    if (pid == 0)
    {
        execvp(ast->argv[0], ast->argv);
        fprintf(stderr, "Failed exec\n");
        return 127;
    }
    else
    {
        waitpid(pid, &status, 0);
    }
    return status;
}

int ast_eval(struct ast *ast)
{
    if (!ast)
    {
        return 1;
    }
    if (ast->type == AST_SIMPLE_COMMAND)
    {
        return eval_sc_node(ast);
    }
    else if (ast->type == AST_COMMAND_LIST)
    {
        for (size_t i = 0; i < ast->nb_child; i++)
        {
            ast_eval(ast->children[i]);
        }
    }
    return 1;
}
