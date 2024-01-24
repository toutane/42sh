#include "../ast.h"

// every if, then and else have to be filled in the right order
void fill_if_node(struct ast *ast, struct ast *ast_child)
{
    struct ast_condition *ast_condition = (struct ast_condition *)ast;

    if (!ast_condition->condition)
    {
        ast_condition->condition = ast_child;
    }
    else if (!ast_condition->then_body)
    {
        ast_condition->then_body = ast_child;
    }
    else
    {
        ast_condition->else_body = ast_child;
    }
    return;
}
