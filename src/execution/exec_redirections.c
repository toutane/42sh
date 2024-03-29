#define OPEN_FAIL 1
#define DUP2_FAIL 1
#define CREATE_FILE_PERM 0644

#include "exec.h"

/// > as well as >| redirections
static int eval_redirection_GREAT(struct ast *ast,
                                  struct mem *mem) // TODO: Check returns code
{
    // assert(ast->type == AST_REDIRECTION);
    struct ast_redirection *ast_redir = (struct ast_redirection *)ast;
    // assert(ast_redir->redirection_type == REDIR_GREAT);

    int fd =
        open(ast_redir->target, O_CREAT | O_TRUNC | O_WRONLY, CREATE_FILE_PERM);
    if (fd == -1)
    {
        fprintf(stderr, "failed to open redirection file\n");
        _exit(OPEN_FAIL); // Here
    }

    // Save potentialy closed fd
    int stdout_dup = dup(ast_redir->ionumber);

    // Assigning fd to ionumber
    if (dup2(fd, ast_redir->ionumber) == -1)
    {
        fprintf(stderr, "dup2 failed\n");
        _exit(DUP2_FAIL); // Here
    }

    int ret_val = EXIT_SUCCESS; // Here
    if (ast_redir->next != NULL)
    {
        ret_val = eval_ast(ast_redir->next, mem);
    }

    fflush(NULL);

    // Restoring closed fds
    dup2(stdout_dup, ast_redir->ionumber);
    close(stdout_dup);

    return ret_val;
}

/// >& redirection
static int
eval_redirection_GREATAND(struct ast *ast,
                          struct mem *mem) // TODO: Check returns code
{
    // assert(ast->type == AST_REDIRECTION);
    struct ast_redirection *ast_redir = (struct ast_redirection *)ast;
    // assert(ast_redir->redirection_type == REDIR_GREATAND);

    // strtoul seems useless here
    // words after >& are implementation-defined (thus won't be tested)
    if (!strcmp(ast_redir->target, "-"))
    {
        close(ast_redir->ionumber);
        return eval_ast(ast_redir->next, mem);
    }
    int fd = atoi(ast_redir->target);

    // Save potentialy closed fd
    int stdout_dup = dup(ast_redir->ionumber);

    // Assigning fd to ionumber
    if (dup2(fd, ast_redir->ionumber) == -1)
    {
        perror("");
        _exit(DUP2_FAIL); // Here
    }

    int ret_val = EXIT_SUCCESS; // Here
    if (ast_redir->next != NULL)
    {
        ret_val = eval_ast(ast_redir->next, mem);
    }

    fflush(NULL);

    // Restoring closed fds
    dup2(stdout_dup, ast_redir->ionumber);
    close(stdout_dup);

    return ret_val;
}

/// <> redirection
static int
eval_redirection_LESSGREAT(struct ast *ast,
                           struct mem *mem) // TODO: Check returns code
{
    // assert(ast->type == AST_REDIRECTION);
    struct ast_redirection *ast_redir = (struct ast_redirection *)ast;
    // assert(ast_redir->redirection_type == REDIR_LESSGREAT);

    int fd = open(ast_redir->target, O_CREAT | O_RDWR, CREATE_FILE_PERM);
    if (fd == -1)
    {
        fprintf(stderr, "failed to open redirection file\n");
        _exit(OPEN_FAIL); // Here
    }

    // Save potentialy closed fd
    // Should be stdin by default here
    int stdout_dup = dup(ast_redir->ionumber);

    // Assigning fd to ionumber
    if (dup2(fd, ast_redir->ionumber) == -1)
    {
        fprintf(stderr, "dup2 failed\n");
        _exit(DUP2_FAIL); // Here
    }

    int ret_val = EXIT_SUCCESS; // Here
    if (ast_redir->next != NULL)
    {
        ret_val = eval_ast(ast_redir->next, mem);
    }

    fflush(NULL);

    // Restoring closed fds
    dup2(stdout_dup, ast_redir->ionumber);
    close(stdout_dup);

    return ret_val;
}

/// < redirection
static int eval_redirection_LESS(struct ast *ast,
                                 struct mem *mem) // TODO: Check returns code
{
    // assert(ast->type == AST_REDIRECTION);
    struct ast_redirection *ast_redir = (struct ast_redirection *)ast;
    // assert(ast_redir->redirection_type == REDIR_LESS);

    int fd = open(ast_redir->target, O_RDONLY, CREATE_FILE_PERM);
    if (fd == -1)
    {
        fprintf(stderr, "failed to open redirection file\n");
        _exit(OPEN_FAIL); // Here
    }

    // Save potentialy closed fd
    // Should be stdin by default here
    int stdout_dup = dup(ast_redir->ionumber);

    // Assigning fd to ionumber
    if (dup2(fd, ast_redir->ionumber) == -1)
    {
        fprintf(stderr, "dup2 failed\n");
        _exit(DUP2_FAIL); // Here
    }

    int ret_val = EXIT_SUCCESS; // Here
    if (ast_redir->next != NULL)
    {
        ret_val = eval_ast(ast_redir->next, mem);
    }

    fflush(NULL);

    // Restoring closed fds
    dup2(stdout_dup, ast_redir->ionumber);
    close(stdout_dup);

    return ret_val;
}

/// <& redirection
static int eval_redirection_LESSAND(struct ast *ast,
                                    struct mem *mem) // TODO: Check returns code
{
    // assert(ast->type == AST_REDIRECTION);
    struct ast_redirection *ast_redir = (struct ast_redirection *)ast;
    // assert(ast_redir->redirection_type == REDIR_LESSAND);

    if (!strcmp(ast_redir->target, "-"))
    {
        close(ast_redir->ionumber);
        return eval_ast(ast_redir->next, mem);
    }
    int fd = atoi(ast_redir->target);

    // Save potentialy closed fd
    // Should be stdin by default here
    int stdout_dup = dup(ast_redir->ionumber);

    // Assigning fd to ionumber
    if (dup2(fd, ast_redir->ionumber) == -1)
    {
        perror("");
        _exit(DUP2_FAIL); // Here
    }

    int ret_val = EXIT_SUCCESS; // Here
    if (ast_redir->next != NULL)
    {
        ret_val = eval_ast(ast_redir->next, mem);
    }

    fflush(NULL);

    // Restoring closed fds
    dup2(stdout_dup, ast_redir->ionumber);
    close(stdout_dup);

    return ret_val;
}

/// >> redirection
static int eval_redirection_DGREAT(struct ast *ast,
                                   struct mem *mem) // TODO: Check returns code
{
    // assert(ast->type == AST_REDIRECTION);
    struct ast_redirection *ast_redir = (struct ast_redirection *)ast;
    // assert(ast_redir->redirection_type == REDIR_DGREAT);

    int fd = open(ast_redir->target, O_CREAT | O_APPEND | O_WRONLY,
                  CREATE_FILE_PERM);
    if (fd == -1)
    {
        fprintf(stderr, "failed to open redirection file\n");
        _exit(OPEN_FAIL); // Here
    }

    // Save potentialy closed fd
    int stdout_dup = dup(ast_redir->ionumber);

    // Assigning fd to ionumber
    if (dup2(fd, ast_redir->ionumber) == -1)
    {
        fprintf(stderr, "dup2 failed\n");
        _exit(DUP2_FAIL); // Here
    }

    int ret_val = EXIT_SUCCESS; // Here
    if (ast_redir->next != NULL)
    {
        ret_val = eval_ast(ast_redir->next, mem);
    }

    fflush(NULL);

    // Restoring closed fds
    dup2(stdout_dup, ast_redir->ionumber);
    close(stdout_dup);

    return ret_val;
}

// Should be at the end of the file
typedef int (*eval_type)(struct ast *ast, struct mem *mem);
int eval_redirection(struct ast *ast, struct mem *mem)
{
    if (!ast)
    {
        return 0;
    }

    static const eval_type functions[] = {
        [REDIR_GREAT] = &eval_redirection_GREAT,
        [REDIR_CLOBBER] = &eval_redirection_GREAT,
        [REDIR_LESSGREAT] = &eval_redirection_LESSGREAT,
        [REDIR_DGREAT] = &eval_redirection_DGREAT,
        [REDIR_LESS] = &eval_redirection_LESS,
        [REDIR_GREATAND] = &eval_redirection_GREATAND,
        [REDIR_LESSAND] = &eval_redirection_LESSAND,
    };

    return (*functions[((struct ast_redirection *)ast)->redirection_type])(ast,
                                                                           mem);
}
