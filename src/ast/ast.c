#include "ast.h"

void ast_print(struct ast *ast)
{
    if (!ast)
    {
        return;
    }
    if (ast->left)
    {
        ast_print(ast->left);
    }
    if (ast->right)
    {
        ast_print(ast->right);
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
}

void ast_free(struct ast *ast)
{
    if (!ast)
    {
        return;
    }
    if (ast->left)
    {
        ast_free(ast->left);
        ast->left = NULL;
    }
    if (ast->right)
    {
        ast_free(ast->right);
        ast->right = NULL;
    }
    if (ast->type == AST_SIMPLE_COMMAND)
    {
        for (size_t i = 0; i < ast->nb_args; i++)
        {
            free(ast->argv[i]);
        }
        free(ast->argv);
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
    if (ast->type == AST_SIMPLE_COMMAND)
    {
        return eval_sc_node(ast);
    }
    return 1;
}
