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
    }
    else if (ast->type == AST_COMMAND_LIST)
    {
        printf("List(");
        if (ast->nb_child > 0)
        {
            for (size_t i = 0; i < ast->nb_child; i++)
            {
                ast_print(ast->children[i]);
            }
        }
        printf(")\n");
    }
    else if (ast->type == AST_CONDITION)
    {
        printf("If(\n");
        if (ast->nb_child > 0)
        {
            printf("cond: ");
            ast_print(ast->children[0]);
            printf("then: ");
            ast_print(ast->children[1]);
            if (ast->nb_child > 2)
            {
                printf("else: ");
                ast_print(ast->children[2]);
            }
        }
        printf(")");
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
    else if (ast->type == AST_CONDITION)
    {
        ast_free(ast->children[0]);
        ast_free(ast->children[1]);
        if (ast->nb_child > 2)
        {
            ast_free(ast->children[2]);
        }
        free(ast->children);
        ast->children = NULL;
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
        return 0;
    }
    int status = 0;
    if (ast->type == AST_SIMPLE_COMMAND)
    {
        status = eval_sc_node(ast);
    }
    else if (ast->type == AST_COMMAND_LIST)
    {
        for (size_t i = 0; i < ast->nb_child; i++)
        {
            status = ast_eval(ast->children[i]);
        }
    }
    else if (ast->type == AST_CONDITION)
    {
        if (ast_eval(ast->children[0]) == EXIT_SUCCESS)
        {
            status = ast_eval(ast->children[1]);
        }
        else if (ast->nb_args > 2)
        {
            status = ast_eval(ast->children[2]);
        }
    }
    return status;
}
