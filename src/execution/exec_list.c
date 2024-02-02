#include "execution/exec.h"

int eval_list(struct ast *ast, struct mem *mem)
{
    int status = 0;
    struct ast_cmd_list *ast_cmd_list = (struct ast_cmd_list *)ast;

    int break_number;
    int continue_number;
    while (ast_cmd_list)
    {
        status = eval_ast(ast_cmd_list->cmd, mem);

        // if a break was called
        break_number = get_break_number();
        continue_number = get_continue_number();
        if (break_number != 0 || continue_number != 0)
        {
            break;
        }
        ast_cmd_list = (struct ast_cmd_list *)ast_cmd_list->next;
    }

    return status;
}
