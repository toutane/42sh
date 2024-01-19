#define _XOPEN_SOURCE 500

#include <string.h>

#include "../ast.h"

void fill_sc_node(struct ast *ast, struct lexer *lexer)
{
    struct ast_cmd *sc_node = (struct ast_cmd *)ast;

    if (sc_node->argc == 0)
    {
        // Init simple-command node
        ast->type = AST_SIMPLE_COMMAND;
        sc_node->argc = 2;
        sc_node->argv = malloc(sc_node->argc * sizeof(char *));
        if (!sc_node)
        {
            exit(1);
        }

        sc_node->argv[0] = strdup(lexer->cur_tok.value);
        sc_node->argv[sc_node->argc - 1] = NULL;
    }
    else
    {
        // Append element to simple-command node
        sc_node->argc += 1;
        sc_node->argv = realloc(sc_node->argv, sc_node->argc * sizeof(char *));
        if (!sc_node)
        {
            exit(1);
        }

        sc_node->argv[sc_node->argc - 2] = strdup(lexer->cur_tok.value);
        sc_node->argv[sc_node->argc - 1] = NULL;
    }
    return;
}
