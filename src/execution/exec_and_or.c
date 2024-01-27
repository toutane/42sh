#include "exec.h"

// eval or node
int eval_and(struct ast *ast, struct mem *mem)
{
    struct ast_and_or *ast_and_or = (struct ast_and_or *)ast;

    int left_res = eval_ast(ast_and_or->left, mem);
    if (left_res != 0)
    {
        return left_res;
    }
    return eval_ast(ast_and_or->right, mem);
}

// eval or node
int eval_or(struct ast *ast, struct mem *mem)
{
    struct ast_and_or *ast_and_or = (struct ast_and_or *)ast;

    if (eval_ast(ast_and_or->left, mem) == 0)
    {
        return 0;
    }
    return eval_ast(ast_and_or->right, mem);
}
