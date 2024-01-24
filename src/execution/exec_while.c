#include "exec.h"

int eval_while(struct ast *ast, struct hash_map *memory)
{
    int status = 0;
    struct ast_while *ast_while = (struct ast_while *)ast;

    while (eval_ast(ast_while->condition, memory) == EXIT_SUCCESS)
    {
        status = eval_ast(ast_while->data, memory);
    }

    return status;
}
