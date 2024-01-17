#include "exec.h"

int eval_simple_command(struct ast *ast)
{
    int status = 0;

    if (is_builtin_word(ast->argv[0]))
    {
        status = (builtin_fun(ast->argv[0]))(ast->nb_args - 1, ast->argv);
        fflush(stdout); // Flush stdout to avoid mixing output
        return status;
    }

    int pid = fork();
    if (pid == 0)
    {
        execvp(ast->argv[0], ast->argv);
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
