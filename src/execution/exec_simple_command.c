#define _XOPEN_SOURCE

#include <stdio.h>
#include <stdlib.h>

#include "exec.h"
#include "utils/memory/memory.h"

static void arguments_expansion(struct ast_cmd *ast_cmd,
                                struct hash_map *memory)
{
    /* Expand the quoting of arguments, this contains backslash, double quote,
     * single quote and parameter ('$' and '${' ) expansion. For the parameter
     * expansion, the algorithm will search into the global variables hash_map
     * and properly expand variable identifiers. */

    for (int i = 0; i < ast_cmd->argc - 1; i++)
    {
        expand_string(&(ast_cmd->argv[i]), memory);
    }
}

static int handle_global_variables(struct ast_cmd *ast_cmd,
                                   struct hash_map *memory)
{
    /* Determine if the node is completely composed of assignment words, if so,
     * we update the global variables hash map with the new values and do not
     * enter the execution step. */

    int are_all_args_assignment_w = only_assignment_words(ast_cmd->argv);

    /* The node is only composed of assignment words, we update the global
     * variables hash map and return. */

    if (are_all_args_assignment_w)
    {
        for (int i = 0; i < ast_cmd->argc - 1; i++)
        {
            set_var_from_assignment_word(memory, ast_cmd->argv[i]);
        }

        return 1;
    }

    return 0;
}

static void handle_environment_variables(struct ast_cmd *ast_cmd,
                                         struct hash_map *temp_env_memory,
                                         int *offset)
{
    while (is_str_assignment_word(ast_cmd->argv[0]))
    {
        /* If the first word of argv is an assignement word, we pop it thanks to
         * arithmetic pointer. */
        set_var_from_assignment_word(temp_env_memory, ast_cmd->argv[0]);

        /* As we perform arithmetic of pointer we will need at the end to set
         * the pointer to his former value, in order to be able to free it. */
        free(ast_cmd->argv[0]);
        ast_cmd->argv[0] = NULL;

        ast_cmd->argv++;
        ast_cmd->argc--;

        ++(*offset);
    }
}

static int handle_builtin_execution(struct ast_cmd *ast_cmd,
                                    struct hash_map *temp_env_memory,
                                    int offset)
{
    setenv_from_memory(temp_env_memory);

    int status =
        (builtin_fun(ast_cmd->argv[0]))(ast_cmd->argc - 1, ast_cmd->argv);

    // To free the argv array, we need to set it to its original value
    ast_cmd->argv -= offset;
    ast_cmd->argc += offset;

    // Flush stdout to avoid mixing output
    fflush(NULL);

    // Restore the environment variables
    unsetenv_from_memory(temp_env_memory);

    memory_free(temp_env_memory);

    return status;
}

int eval_simple_command(struct ast *ast, struct hash_map *gv_hash_map)
{
    int status = 0;
    struct ast_cmd *ast_cmd = (struct ast_cmd *)ast;

    arguments_expansion(ast_cmd, gv_hash_map);

    if (handle_global_variables(ast_cmd, gv_hash_map))
    {
        return 0;
    }

    // We create a temporary memory to store the environment variables
    struct hash_map *temp_env_memory = memory_new();

    int offset = 0;
    handle_environment_variables(ast_cmd, temp_env_memory, &offset);

    if (is_builtin_word(ast_cmd->argv[0]))
    {
        return handle_builtin_execution(ast_cmd, temp_env_memory, offset);
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
        // Set environment variables
        setenv_from_memory(temp_env_memory);

        execvp(ast_cmd->argv[0], ast_cmd->argv);
        ast_cmd->argv -= offset;
        ast_cmd->argc += offset;

        _exit(execvp_error(errno));
    }
    else
    {
        waitpid(pid, &status, 0);
    }

    ast_cmd->argv -= offset;
    ast_cmd->argc += offset;

    // Check if the command was interrupted by a signal
    if (WIFSIGNALED(status))
    {
        fprintf(stderr, "42sh: command terminated because of a signal");
        memory_free(temp_env_memory);
        return 129;
    }

    // Otherwise we return the status error code
    memory_free(temp_env_memory);
    return WEXITSTATUS(status);
}
