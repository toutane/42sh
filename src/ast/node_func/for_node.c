#define _XOPEN_SOURCE 500

#include "../ast.h"

void fill_for_node(struct ast *ast, struct ast *ast_child, char *data)
{
    struct ast_for *ast_for = (struct ast_for *)ast;

    if (ast_child)
    {
        ast_for->data = ast_child;
    }
    else
    {
        if (!ast_for->condition)
        {
            ast_for->condition = strdup(data);
        }
        else
        {
            if (ast_for->array_size == 0)
            {
                ast_for->array = calloc(1, sizeof(char *));
                ast_for->array[0] = strdup(data);
            }
            else
            {
                ast_for->array = realloc(
                    ast_for->array, (ast_for->array_size + 1) * sizeof(char *));
                ast_for->array[ast_for->array_size] = strdup(data);
            }
            ++ast_for->array_size;
        }
    }
    return;
}
