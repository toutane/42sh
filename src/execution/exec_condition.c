#include "exec.h"

int eval_condition(struct ast *ast)
{
    int status = 0;
    struct ast_condition *ast_condition = (struct ast_condition *)ast;

    if (eval_ast(ast_condition->condition) == EXIT_SUCCESS)
    {
        status = eval_ast(ast_condition->then_body);
    }
    else if (ast_condition->else_body != NULL)
    {
        status = eval_ast(ast_condition->else_body);
    }

    return status;
}
