#include "exec.h"

int eval_list(struct ast *ast)
{
    int status = 0;

    for (size_t i = 0; i < ast->nb_child; i++)
    {
        status = eval_ast(ast->children[i]);
    }

    return status;
}
