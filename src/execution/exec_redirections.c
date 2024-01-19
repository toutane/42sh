#define OPEN_FAIL 2
#define DUP2_FAIL 127

#include "exec.h"

/// > as well as >| redirections
static int eval_redirection_GREAT(struct ast *ast)// TODO: Check returns code
{
    // assert(ast->type == AST_REDIRECTION);
    struct ast_redirection *ast_redir = (struct ast_redirection *)ast;
    // assert(ast_redir->redirection_type == REDIR_GREAT);

    int fd = open(ast_redir->target, O_CREAT | O_TRUNC | O_WRONLY, 0644);
    if (fd == -1)
    {
        fprintf(stderr, "failed to open redirection file\n");
        _exit(OPEN_FAIL);// Here
    }

    // Save potentialy closed fd
    int stdout_dup = dup(ast_redir->ionumber);

    // Assigning fd to ionumber
    if (dup2(fd, ast_redir->ionumber) == -1)
    {
        fprintf(stderr, "dup2 failed\n");
        _exit(DUP2_FAIL);// Here
    }

    int ret_val = 0;// Here
    if (ast_redir->next != NULL)
    {
        ret_val = eval_ast(ast_redir->next);
    }

    fflush(NULL);

    // Restoring closed fds
    dup2(stdout_dup, ast_redir->ionumber);
    close(stdout_dup);

    return ret_val;
}

/// <> redirection
static int eval_redirection_LESSGREAT(struct ast *ast)// TODO: Check returns code
{
    // assert(ast->type == AST_REDIRECTION);
    struct ast_redirection *ast_redir = (struct ast_redirection *)ast;
    // assert(ast_redir->redirection_type == REDIR_LESSGREAT);

    int fd = open(ast_redir->target, O_CREAT | O_RDWR, 0644);
    if (fd == -1)
    {
        fprintf(stderr, "failed to open redirection file\n");
        _exit(OPEN_FAIL);// Here
    }

    // Save potentialy closed fd
    // Should be stdin by default here
    int stdout_dup = dup(ast_redir->ionumber);

    // Assigning fd to ionumber
    if (dup2(fd, ast_redir->ionumber) == -1)
    {
        fprintf(stderr, "dup2 failed\n");
        _exit(DUP2_FAIL);// Here
    }

    int ret_val = 0;// Here
    if (ast_redir->next != NULL)
    {
        ret_val = eval_ast(ast_redir->next);
    }

    fflush(NULL);

    // Restoring closed fds
    dup2(stdout_dup, ast_redir->ionumber);
    close(stdout_dup);

    return ret_val;
}

// Should be at the end of the file
typedef int (*eval_type)(struct ast *ast);
int eval_redirection(struct ast *ast)
{
    if (!ast)
    {
        return 0;
    }

    static const eval_type functions[] = {
        [REDIR_GREAT] = &eval_redirection_GREAT,
        [REDIR_CLOBBER] = &eval_redirection_GREAT,
        [REDIR_LESSGREAT] = &eval_redirection_LESSGREAT,
    };

    return (*functions[((struct ast_redirection *)ast)->redirection_type])(ast);
}
