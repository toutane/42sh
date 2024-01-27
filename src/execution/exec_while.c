#include "exec.h"

int eval_while(struct ast *ast, struct mem *mem)
{
    int status = 0;
    struct ast_while *ast_while = (struct ast_while *)ast;

    while (eval_ast(ast_while->condition, mem) == EXIT_SUCCESS)
    {
        status = eval_ast(ast_while->data, mem);
    }

    return status;
}
