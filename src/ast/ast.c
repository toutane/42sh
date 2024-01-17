#include "ast.h"

void ast_free(struct ast *ast)
{
    if (!ast)
    {
        return;
    }

    if (ast->type == AST_SIMPLE_COMMAND)
    {
        if (ast->nb_args > 0)
        {
            for (size_t i = 0; i < ast->nb_args; i++)
            {
                free(ast->argv[i]);
            }
            free(ast->argv);
            ast->argv = NULL;
        }
    }
    else if (ast->type == AST_COMMAND_LIST)
    {
        if (ast->nb_child > 0)
        {
            for (size_t i = 0; i < ast->nb_child; i++)
            {
                ast_free(ast->children[i]);
            }
            free(ast->children);
            ast->children = NULL;
        }
    }
    else if (ast->type == AST_CONDITION)
    {
        ast_free(ast->children[0]);
        ast_free(ast->children[1]);
        ast_free(ast->children[2]);
        free(ast->children);
        ast->children = NULL;
    }

    free(ast);
}
