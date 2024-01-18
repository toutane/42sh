#include "ast.h"

void fill_pipeline_node(struct ast *ast, struct ast *ast_child)
{
    struct ast_pipeline *ast_pipeline = (struct ast_pipeline *)ast;

    if (!ast_pipeline->left)
    {
        ast_pipeline->left = ast_child;
    }
    else
    {
        ast_pipeline->right = ast_child;
    }

    return;
}
