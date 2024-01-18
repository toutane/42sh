#include "exec.h"

int eval_redirection_GREAT(struct ast *ast)// TODO: Check returns code
{
    // assert(ast->type == AST_REDIRECTION);
    struct ast_redirection *ast_redir = (struct ast_redirection *)ast;
    // assert(ast_redir->redirection_type == REDIR_GREAT);

    int fd = open(ast_redir->target, O_CREAT | O_TRUNC | O_WRONLY, 0644);
    if (fd == -1)
    {
        fprintf(stderr, "failed to open redirection file\n");
        _exit(2);// Here
    }

    // Save potentialy closed fd
    int stdout_dup = dup(ast_redir->ionumber);

    // Assigning fd to ionumber
    if (dup2(fd, ast_redir->ionumber) == -1)
    {
        fprintf(stderr, "dup2 failed\n");
        _exit(127);// Here
    }

    if (ast_redir->next != NULL)
    {
        eval_ast(ast_redir->next);
    }

    fflush(NULL);

    // Restoring closed fds
    dup2(stdout_dup, ast_redir->ionumber);
    close(stdout_dup);
}
