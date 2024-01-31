#include "execution/exec.h"

int eval_func(struct ast *ast, struct mem *mem)
{
    struct ast_func *ast_func = (struct ast_func *)ast;

    if (ast_func->shell_command != NULL)
    {
        hm_set_fun(mem->hm_fun, ast_func->name, ast_func->shell_command);
        ast_func->shell_command = NULL;
    }

    return 0;
}
