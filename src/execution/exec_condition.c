#include "exec.h"

int eval_condition(struct ast *ast, struct hash_map *gv_hash_map)
{
    int status = 0;
    struct ast_condition *ast_condition = (struct ast_condition *)ast;

    if (eval_ast(ast_condition->condition, gv_hash_map) == EXIT_SUCCESS)
    {
        status = eval_ast(ast_condition->then_body, gv_hash_map);
    }
    else if (ast_condition->else_body != NULL)
    {
        status = eval_ast(ast_condition->else_body, gv_hash_map);
    }

    return status;
}
