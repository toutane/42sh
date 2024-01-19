#define _XOPEN_SOURCE 500

#include <string.h>

#include "ast.h"

// default case: ionumber set to 1
void fill_redirection_node(struct ast *ast, int ionumber, char *str)
{
    struct ast_redirection *ast_redirection = (struct ast_redirection *)ast;

    if (!str)
    {
        ast_redirection->ionumber = ionumber;
    }
    else
    {
        ast_redirection->data = strdup(str);
    }
    return;
}
