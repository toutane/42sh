#include "exec.h"

int eval_simple_command(struct ast *ast)
{
    int status = 0;
    struct ast_cmd *ast_cmd = (struct ast_cmd *)ast;

    if (is_builtin_word(ast_cmd->argv[0]))
    {
        status =
            (builtin_fun(ast_cmd->argv[0]))(ast_cmd->argc - 1, ast_cmd->argv);
        fflush(NULL); // Flush stdout to avoid mixing output
        return status;
    }

    int pid = fork();
    if (pid == 0)
    {
        execvp(ast_cmd->argv[0], ast_cmd->argv);
        exit(execvp_error(errno));
    }
    else
    {
        waitpid(pid, &status, 0);
    }

    // check if the command was interrupted by a signal
    if (WIFSIGNALED(status))
    {
        fprintf(stderr, "42sh: command terminated because of a signal");
        return 129;
    }

    // otherwise we return the status error code
    return WEXITSTATUS(status);
}
