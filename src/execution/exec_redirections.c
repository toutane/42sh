#include "exec.h"

static int
eval_redirection_GREAT(struct ast *ast,
                       struct hash_map *gv_hash_map) // TODO: Check returns code
{
    // assert(ast->type == AST_REDIRECTION);
    struct ast_redirection *ast_redir = (struct ast_redirection *)ast;
    // assert(ast_redir->redirection_type == REDIR_GREAT);

    int fd = open(ast_redir->target, O_CREAT | O_TRUNC | O_WRONLY, 0644);
    if (fd == -1)
    {
        fprintf(stderr, "failed to open redirection file\n");
        _exit(2); // Here
    }

    // Save potentialy closed fd
    int stdout_dup = dup(ast_redir->ionumber);

    // Assigning fd to ionumber
    if (dup2(fd, ast_redir->ionumber) == -1)
    {
        fprintf(stderr, "dup2 failed\n");
        _exit(127); // Here
    }

    int ret_val = 0;
    if (ast_redir->next != NULL)
    {
        ret_val = eval_ast(ast_redir->next, gv_hash_map);
    }

    fflush(NULL);

    // Restoring closed fds
    dup2(stdout_dup, ast_redir->ionumber);
    close(stdout_dup);

    return ret_val;
}

typedef int (*eval_type)(struct ast *ast, struct hash_map *gv_hash_map);
int eval_redirection(struct ast *ast, struct hash_map *gv_hash_map)
{
    if (!ast)
    {
        return 0;
    }

    static const eval_type functions[] = {
        [REDIR_GREAT] = &eval_redirection_GREAT,
        [REDIR_CLOBBER] = &eval_redirection_GREAT,
    };

    return (*functions[((struct ast_redirection *)ast)->redirection_type])(
        ast, gv_hash_map);
}
