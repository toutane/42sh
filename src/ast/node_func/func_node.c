#define _XOPEN_SOURCE 500

#include "ast.h"

void fill_word_func(struct ast *ast, char *func_name)
{
    struct ast_func *ast_func = (struct ast_func *)ast;

    ast_func->name = strdup(func_name);
    return;
}

void fill_func_node(struct ast *ast, struct ast *ast_child)
{
    struct ast_func *ast_func = (struct ast_func *)ast;

    ast_func->shell_command = ast_child;
    return;
}
