#include "exec.h"

int eval_condition(struct ast *ast)
{
    int status = 0;

    if (eval_ast(ast->children[0]) == EXIT_SUCCESS)
    {
        status = eval_ast(ast->children[1]);
    }
    else if (ast->children[2] != NULL)
    {
        status = eval_ast(ast->children[2]);
    }

    return status;
}
