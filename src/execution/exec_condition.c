#include "exec.h"

int eval_condition(struct ast *ast, struct mem *mem)
{
    int status = 0;
    struct ast_condition *ast_condition = (struct ast_condition *)ast;

    if (eval_ast(ast_condition->condition, mem) == EXIT_SUCCESS)
    {
        status = eval_ast(ast_condition->then_body, mem);
    }
    else if (ast_condition->else_body != NULL)
    {
        status = eval_ast(ast_condition->else_body, mem);
    }

    return status;
}
