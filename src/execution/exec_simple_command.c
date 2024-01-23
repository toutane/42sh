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

static void prefixes_expansion(struct ast_cmd *ast_cmd, struct hash_map *memory)
{
    /* Expand the quoting of prefixes, this contains backslash, double quote,
     * single quote and parameter ('$' and '${' ) expansion. For the parameter
     * expansion, the algorithm will search into the given hash_map and properly
     * expand variable identifiers. */

    for (int i = 0; i < ast_cmd->prefix_count - 1; i++)
    {
        expand_string(&(ast_cmd->prefix[i]), memory);
        set_var_from_assignment_word(memory, ast_cmd->prefix[i]);
    }
}

static int handle_builtin_execution(struct ast_cmd *ast_cmd,
                                    struct hash_map *memory)
{
    setenv_from_memory(memory);

    int status =
        (builtin_fun(ast_cmd->argv[0]))(ast_cmd->argc - 1, ast_cmd->argv);

    // Flush stdout to avoid mixing output
    fflush(NULL);

    unsetenv_from_memory(memory);
    memory_free(memory);

    return status;
}

int eval_simple_command(struct ast *ast, struct hash_map *gv_hash_map)
{
    int status = 0;
    struct ast_cmd *ast_cmd = (struct ast_cmd *)ast;

    arguments_expansion(ast_cmd, gv_hash_map);

    if (ast_cmd->argc == 0)
    {
        prefixes_expansion(ast_cmd, gv_hash_map);
        return 0;
    }

    struct hash_map *cmd_env_memory = memory_new();
    if (ast_cmd->prefix_count > 0)
    {
        prefixes_expansion(ast_cmd, cmd_env_memory);
    }

    if (is_builtin_word(ast_cmd->argv[0]))
    {
        return handle_builtin_execution(ast_cmd, cmd_env_memory);
    }

    int pid = fork();

    // Check if fork suceed
    if (pid == -1)
    {
        perror("");

        _exit(127); // Check this value
    }
    if (pid == 0)
    {
        setenv_from_memory(cmd_env_memory);

        execvp(ast_cmd->argv[0], ast_cmd->argv);

        fprintf(stderr, "42sh: %s: command not found\n", ast_cmd->argv[0]);
        _exit(execvp_error(errno));
    }
    else
    {
        waitpid(pid, &status, 0);
    }

    // Check if the command was interrupted by a signal
    if (WIFSIGNALED(status))
    {
        fprintf(stderr, "42sh: command terminated because of a signal");
        memory_free(cmd_env_memory);
        return 129;
    }

    // Otherwise we return the status error code
    memory_free(cmd_env_memory);
    return WEXITSTATUS(status);
}
