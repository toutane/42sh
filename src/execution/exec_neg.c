#include "exec.h"

int eval_neg(struct ast *ast)
{
    struct ast_neg *ast_neg = (struct ast_neg *)ast;

    int status = eval_ast(ast_neg->data);
    if (status == EXIT_SUCCESS)
    {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
