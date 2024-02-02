#define _XOPEN_SOURCE 500

#include "../ast.h"

void fill_for_node_condition(struct ast *ast, char *data)
{
    struct ast_for *ast_for = (struct ast_for *)ast;

    ast_for->condition = strdup(data);
}

void fill_for_node_data(struct ast *ast, char *data)
{
    struct ast_for *ast_for = (struct ast_for *)ast;

    ast_for->array =
        realloc(ast_for->array, (ast_for->array_size + 1) * sizeof(char *));
    ast_for->array[ast_for->array_size] = strdup(data);
    ++ast_for->array_size;
}

void fill_for_node_child(struct ast *ast, struct ast *ast_child)
{
    struct ast_for *ast_for = (struct ast_for *)ast;

    ast_for->data = ast_child;
}
