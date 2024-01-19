#include "exec.h"

int eval_for(struct ast *ast)
{
    int status = 0;
    struct ast_for *ast_for = (struct ast_for *)ast;
    if (!ast_for)
        return 0;

    // TODO: implement for execution loop with variables

    return status;
}
