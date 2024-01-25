#define _XOPEN_SOURCE 500

#include <string.h>

#include "../ast.h"

// fill simple node with argv and update argc
void fill_sc_node_arg(struct ast *ast, struct lexer *lexer)
{
    struct ast_cmd *sc_node = (struct ast_cmd *)ast;

    // Append element to simple-command node
    sc_node->argc += 1;
    sc_node->argv =
        realloc(sc_node->argv, (sc_node->argc + 1) * sizeof(char *));
    if (!sc_node)
    {
        _exit(1);
    }

    // dup string and NULL value
    sc_node->argv[sc_node->argc - 1] = strdup(lexer->cur_tok.value);
    sc_node->argv[sc_node->argc] = NULL;

    return;
}

void fill_sc_node_redir(struct ast *ast, struct lexer *lexer)
{
    struct ast_cmd *sc_node = (struct ast_cmd *)ast;

    // Append element to simple-command node
    sc_node->prefix_count += 1;
    sc_node->prefix =
        realloc(sc_node->prefix, (sc_node->prefix_count + 1) * sizeof(char *));
    if (!sc_node)
    {
        _exit(1);
    }

    sc_node->prefix[sc_node->prefix_count - 1] = strdup(lexer->cur_tok.value);
    sc_node->prefix[sc_node->prefix_count] = NULL;

    return;
}

/*
void fill_sc_node(struct ast *ast, struct lexer *lexer, int fill_argv)
{
    struct ast_cmd *sc_node = (struct ast_cmd *)ast;

    if (fill_argv)
    {
        if (sc_node->argc == 0)
        {
            // Init simple-command node
            ast->type = AST_SIMPLE_COMMAND;
            sc_node->argc = 1;
            sc_node->argv = malloc(sc_node->argc * sizeof(char *));
            if (!sc_node)
            {
                _exit(1);
            }

            sc_node->argv[0] = strdup(lexer->cur_tok.value);
            sc_node->argv[sc_node->argc - 1] = NULL;
        }
        else
        {
            // Append element to simple-command node
            sc_node->argc += 1;
            sc_node->argv =
                realloc(sc_node->argv, sc_node->argc * sizeof(char *));
            if (!sc_node)
            {
                _exit(1);
            }

            sc_node->argv[sc_node->argc - 2] = strdup(lexer->cur_tok.value);
            sc_node->argv[sc_node->argc - 1] = NULL;
        }
        return;
    }
    else
    {
        if (sc_node->prefix_count == 0)
        {
            // Init simple-command node
            ast->type = AST_SIMPLE_COMMAND;
            sc_node->prefix_count = 2;
            sc_node->prefix = malloc(sc_node->prefix_count * sizeof(char *));
            if (!sc_node)
            {
                _exit(1);
            }

            sc_node->prefix[0] = strdup(lexer->cur_tok.value);
            sc_node->prefix[sc_node->prefix_count - 1] = NULL;
        }
        else
        {
            // Append element to simple-command node
            sc_node->prefix_count += 1;
            sc_node->prefix = realloc(sc_node->prefix,
                                      sc_node->prefix_count * sizeof(char *));
            if (!sc_node)
            {
                _exit(1);
            }

            sc_node->prefix[sc_node->prefix_count - 2] =
                strdup(lexer->cur_tok.value);
            sc_node->prefix[sc_node->prefix_count - 1] = NULL;
        }
        return;
    }
}
*/
