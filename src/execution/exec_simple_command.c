#include "exec.h"

int eval_simple_command(struct ast *ast)
{
    int status = 0;

    /* The expansion of quotes must be done here, (and not in the parser, nor in
     * the lexer) because the quotes are not part of the command arguments. The
     * quotes are only used to group arguments together. So we need to remove
     * the quotes before executing the command. It is the expansion algorithm
     * that will take care of different quotes cases.
     */
    for (size_t i = 0; i < ast->nb_args; i++)
    {
        single_quote_expansion(&(ast->argv[i]));
    }

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
