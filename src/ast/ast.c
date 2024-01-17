#include "ast.h"

void ast_free(struct ast *ast)
{
    if (!ast)
    {
        return;
    }

    // Free argv
    for (size_t i = 0; i < ast->nb_args; ++i)
    {
        free(ast->argv[i]);
    }
    free(ast->argv);

    // recursively free childs
    for (size_t i = 0; i < ast->nb_child; ++i)
    {
        ast_free(ast->children[i]);
    }
    free(ast->children);
    free(ast);
}
