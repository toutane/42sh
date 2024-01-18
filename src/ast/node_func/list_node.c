#include "ast.h"

void fill_list_node(struct ast *ast, struct ast *ast_cmd)
{
    struct ast_cmd_list *ast_cmd_list = (struct ast_cmd_list *)ast;

    if (!ast_cmd_list->cmd)
    {
        ast_cmd_list->cmd = ast_cmd;
    }
    else
    {
        while (ast_cmd_list->next)
        {
            ast_cmd_list = (struct ast_cmd_list *)ast_cmd_list->next;
        }
        /// Create a list node
        struct ast_cmd_list *new_list = calloc(1, sizeof(struct ast_cmd_list));
        new_list->base.type = AST_COMMAND_LIST;
        new_list->cmd = ast_cmd;

        // Append it
        ast_cmd_list->next = (struct ast *)new_list;
    }
    return;
}
