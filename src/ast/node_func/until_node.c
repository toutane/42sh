#include "ast.h"

void fill_until_node(struct ast *ast, struct ast *ast_child)
{
    struct ast_until *ast_until = (struct ast_until *)ast;

    if (!ast_until->condition)
    {
        ast_until->condition = ast_child;
    }
    else
    {
        ast_until->data = ast_child;
    }
    return;
}
