#include "execution/exec.h"

int eval_while(struct ast *ast, struct mem *mem)
{
    int status = 0;
    struct ast_while *ast_while = (struct ast_while *)ast;

    set_loop_number(get_loop_number() + 1);

    int break_number;
    int continue_number;
    while (eval_ast(ast_while->condition, mem) == EXIT_SUCCESS
           && get_break_number() == 0 && get_continue_number() == 0)
    {
        status = eval_ast(ast_while->data, mem);

        break_number = get_break_number();
        if (break_number != 0)
        {
            set_break_number(break_number - 1);
            break;
        }

        continue_number = get_continue_number();
        if (continue_number != 0)
        {
            set_continue_number(continue_number - 1);
            continue;
        }
    }

    set_loop_number(get_loop_number() - 1);

    return status;
}
