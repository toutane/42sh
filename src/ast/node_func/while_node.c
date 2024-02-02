#include "ast.h"

void fill_while_node(struct ast *ast, struct ast *ast_child)
{
    struct ast_while *ast_while = (struct ast_while *)ast;

    if (!ast_while->condition)
    {
        ast_while->condition = ast_child;
    }
    else
    {
        ast_while->data = ast_child;
    }
    return;
}
