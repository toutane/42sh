#include <stdio.h>

#include "exec.h"

int eval_simple_command(struct ast *ast, struct hash_map *gv_hash_map)
{
    int status = 0;
    struct ast_cmd *ast_cmd = (struct ast_cmd *)ast;

    /* Expand the quoting of arguments, this contains backslash, double quote,
     * single quote and parameter ('$' and '${' ) expansion. For the parameter
     * expansion, the algorithm will search into the global variables hash_map
     * and properly expand variable identifiers. */
    for (int i = 0; i < ast_cmd->argc - 1; i++)
    {
        expand_string(&(ast_cmd->argv[i]), gv_hash_map);
    }

    /* Determine if the node is completely composed of assignment words, if so,
     * we update the global variables hash map with the new values and do not
     * enter the execution step. */
    int are_all_args_assignment_w = only_assignment_words(ast_cmd->argv);

    /* The node is only composed of assignment words, we update the global
     * variables hash map and return. */
    if (are_all_args_assignment_w)
    {
        update_gv_hash_map(ast_cmd->argv, gv_hash_map);
        return 0;
    }

    // as we perform arithmetic of pointer we will need at the end to set the
    // pointer to his former value, in order to be able to free it
    int offset = 0;
    while (is_str_assignment_word(ast_cmd->argv[0]))
    {
        // if the first word of argv is an assignement word, we pop it thanks to
        // arithmetic pointer
        free(ast_cmd->argv[0]);
        ast_cmd->argv[0] = NULL;

        ast_cmd->argv++;
        ast_cmd->argc--;

        ++offset;
    }

    /* Execution step */
    if (is_builtin_word(ast_cmd->argv[0]))
    {
        status =
            (builtin_fun(ast_cmd->argv[0]))(ast_cmd->argc - 1, ast_cmd->argv);
        ast_cmd->argv -= offset; // to free the argv array, we need to set it to
                                 // his original value
        fflush(NULL); // Flush stdout to avoid mixing output
        return status;
    }

    int pid = fork();

    // Check if fork suceed
    if (pid == -1)
    {
        perror("");
        ast_cmd->argv -= offset;
        _exit(127); // Check this value
    }
    if (pid == 0)
    {
        execvp(ast_cmd->argv[0], ast_cmd->argv);
        ast_cmd->argv -= offset;
        _exit(execvp_error(errno));
    }
    else
    {
        waitpid(pid, &status, 0);
    }

    ast_cmd->argv -= offset;
    // Check if the command was interrupted by a signal
    if (WIFSIGNALED(status))
    {
        fprintf(stderr, "42sh: command terminated because of a signal");
        return 129;
    }

    // Otherwise we return the status error code
    return WEXITSTATUS(status);
}
