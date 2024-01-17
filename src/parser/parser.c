#define _XOPEN_SOURCE 500

#include "parser.h"

void fill_sc_node(struct ast *sc_node, struct lexer *lexer)
{
    if (sc_node->nb_args == 0)
    {
        // Init simple-command node
        sc_node->type = AST_SIMPLE_COMMAND;
        sc_node->nb_args = 2;
        sc_node->argv = malloc(sc_node->nb_args * sizeof(char *));
        // TODO: Check for NULL after allocation try
        sc_node->argv[0] = strdup(lexer->cur_tok.value);
        sc_node->argv[sc_node->nb_args - 1] = NULL;
    }
    else
    {
        // Append element to simple-command node
        sc_node->nb_args += 1;
        sc_node->argv =
            realloc(sc_node->argv, sc_node->nb_args * sizeof(char *));
        // TODO: Check for NULL after allocation try
        sc_node->argv[sc_node->nb_args - 2] = strdup(lexer->cur_tok.value);
        sc_node->argv[sc_node->nb_args - 1] = NULL;
    }
}

void fill_list_node(struct ast *list_node, struct ast *sc_node)
{
    list_node->nb_child += 1;
    list_node->children = realloc(list_node->children,
                                  sizeof(struct ast *) * list_node->nb_child);
    // TODO: Check for NULL after allocation try
    list_node->children[list_node->nb_child - 1] = sc_node;
}

void init_if_node(struct ast *if_node)
{
    // Init if_node
    if_node->type = AST_CONDITION;
    if_node->nb_child = 3;
    if_node->children = calloc(if_node->nb_child, sizeof(struct ast *));
    // TODO: Check for NULL after allocation try
}

void fill_if_node(struct ast *if_node, struct ast *node)
{
    if (if_node->nb_child == 0)
    {
        fprintf(stderr, "Uninitialized if_node");
        return;
    }
    for (size_t i = 0; i < if_node->nb_child; i++)
    {
        if (!if_node->children[i])
        {
            if_node->children[i] = node;
            return;
        }
    }
    fprintf(stderr, "If node can't have more than 3 children\n");
}

void init_redirection_node(struct ast *redirection_node)
{
    redir_node->type = AST_REDIRECTION
    redirection_node->nb_args = 3;
    redirection_node->argv = calloc(redirection_node->nb_args, sizeof(char *));
    // TODO CHECK if calloc failed
}

void fill_redirection_node(struct ast *redirection_node, size_t position, char *str)
{
    redirection_node->argv[position] = strdup(str);
}

void init_pipeline_node(struct ast *pipeline_node)
{
    pipe_node->type = AST_PIPELINE;
    pipe_node->nb_child = 2;
    pipe_node->children = calloc(2, sizeof(struct ast *));
    // TODO CHECK if calloc failed
}

void fill_pipeline_node(struct ast *pipe_node, struct ast *ast)
{
    for (size_t i = 0; i < pipe_node->nb_child; ++i)
    {
        if (!pipe->children[i])
        {
            pipe_node->children[i] = node;
            return;
        }
    }
    fprintf(stderr, "42sh: If node can't have more than 2 children\n");
}
