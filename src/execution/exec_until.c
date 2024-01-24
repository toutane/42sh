#include "exec.h"

int eval_until(struct ast *ast, struct hash_map *memory)
{
    int status = 0;
    struct ast_until *ast_until = (struct ast_until *)ast;

    while (eval_ast(ast_until->condition, memory) != EXIT_SUCCESS)
    {
        status = eval_ast(ast_until->data, memory);
    }

    return status;
}
