#include "exec.h"

int eval_list(struct ast *ast)
{
    int status = 0;
    struct ast_cmd_list *ast_cmd_list = (struct ast_cmd_list *)ast;

    while (ast_cmd_list)
    {
        status = eval_ast((struct ast *)ast_cmd_list);
        ast_cmd_list = (struct ast_cmd_list *)ast_cmd_list->next;
    }

    return status;
}
