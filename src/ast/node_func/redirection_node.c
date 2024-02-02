#define _XOPEN_SOURCE 500

#include <string.h>

#include "ast.h"

void init_redirection_node(struct ast *ast)
{
    struct ast_redirection *ast_redirection = (struct ast_redirection *)ast;
    ast_redirection->base.type = AST_REDIRECTION;
    ast_redirection->ionumber = -1;
}

void fill_redirection_node_ionumber(struct ast *ast, int ionumber)
{
    struct ast_redirection *ast_redirection = (struct ast_redirection *)ast;
    // Supposed to be different from -1 now
    ast_redirection->ionumber = ionumber;
}

void fill_redirection_node_type(struct ast *ast, struct token tok)
{
    struct ast_redirection *ast_redirection = (struct ast_redirection *)ast;
    static enum redirection_type match_table[] = {
        [TOKEN_GREAT] = REDIR_GREAT, // '>'
        [TOKEN_LESS] = REDIR_LESS, // '<'
        [TOKEN_DGREAT] = REDIR_DGREAT, // '>>'
        [TOKEN_LESSAND] = REDIR_LESSAND, // '<&'
        [TOKEN_GREATAND] = REDIR_GREATAND, // '>&'
        [TOKEN_LESSGREAT] = REDIR_LESSGREAT, // '<>'
        [TOKEN_CLOBBER] = REDIR_CLOBBER, // '>|'
    };
    ast_redirection->redirection_type = match_table[tok.type];
    if (ast_redirection->ionumber == -1)
    {
        // Then initialize it
        static int ionumbers_table[] = {
            [TOKEN_GREAT] = 1, // '>'
            [TOKEN_LESS] = 0, // '<'
            [TOKEN_DGREAT] = 1, // '>>'
            [TOKEN_LESSAND] = 0, // '<&'
            [TOKEN_GREATAND] = 1, // '>&'
            [TOKEN_LESSGREAT] = 0, // '<>'
            [TOKEN_CLOBBER] = 1, // '>|'
        };
        ast_redirection->ionumber = ionumbers_table[tok.type];
    }
}

void fill_redirection_node_target(struct ast *ast, char *target)
{
    struct ast_redirection *ast_redirection = (struct ast_redirection *)ast;
    ast_redirection->target = strdup(target);
}
