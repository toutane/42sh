#include "exec.h"

int eval_until(struct ast *ast, struct hash_map *memory)
{
    int status = 0;
    struct ast_until *ast_until = (struct ast_until *)ast;

    int break_number;
    int continue_number;
    while (eval_ast(ast_until->condition, memory) != EXIT_SUCCESS)
    {
        status = eval_ast(ast_until->data, memory);

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

    return status;
}
