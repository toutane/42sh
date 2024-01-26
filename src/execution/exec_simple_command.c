#define _XOPEN_SOURCE

#include <stdio.h>
#include <stdlib.h>

#include "exec.h"
#include "utils/memory/memory.h"

static void free_copies(char **argv_copy, int argc, char **prefixes_copy,
                        int prefix_count)
{
    for (int i = 0; i < argc; ++i)
    {
        free(argv_copy[i]);
    }
    free(argv_copy);

    for (int i = 0; i < prefix_count; ++i)
    {
        free(prefixes_copy[i]);
    }
    free(prefixes_copy);

    return;
}

static void arguments_expansion(struct ast_cmd *ast_cmd, char **argv_copy,
                                struct hash_map *memory)
{
    /* Expand the quoting of arguments, this contains backslash, double quote,
     * single quote and parameter ('$' and '${' ) expansion. For the parameter
     * expansion, the algorithm will search into the global variables hash_map
     * and properly expand variable identifiers. */

    for (int i = 0; i < ast_cmd->argc - 1; i++)
    {
        argv_copy[i] = expand_string(&(ast_cmd->argv[i]), memory);
    }
}

static void prefixes_expansion(struct ast_cmd *ast_cmd, char **prefixes_copy,
                               struct hash_map *memory)
{
    /* Expand the quoting of prefixes, this contains backslash, double quote,
     * single quote and parameter ('$' and '${' ) expansion. For the parameter
     * expansion, the algorithm will search into the given hash_map and properly
     * expand variable identifiers. */

    for (int i = 0; i < ast_cmd->prefix_count - 1; i++)
    {
        prefixes_copy[i] = expand_string(&(ast_cmd->prefix[i]), memory);
        set_var_from_assignment_word(memory, prefixes_copy[i]);
    }
}

static int handle_builtin_execution(struct ast_cmd *ast_cmd, char **argv_copy,
                                    struct hash_map *memory, struct hash_map *gv_hash_map)
{
    setenv_from_memory(memory);

    int status = (builtin_fun(argv_copy[0]))(ast_cmd->argc - 1, argv_copy, gv_hash_map);

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

    char **argv_copy = calloc(ast_cmd->argc, sizeof(char *));
    arguments_expansion(ast_cmd, argv_copy, gv_hash_map);

    char **prefixes_copy = calloc(ast_cmd->prefix_count, sizeof(char *));
    if (ast_cmd->argc == 0)
    {
        prefixes_expansion(ast_cmd, prefixes_copy, gv_hash_map);
        free_copies(argv_copy, ast_cmd->argc, prefixes_copy,
                    ast_cmd->prefix_count);
        return 0;
    }

    struct hash_map *cmd_env_memory = memory_new();
    if (ast_cmd->prefix_count > 0)
    {
        prefixes_expansion(ast_cmd, prefixes_copy, cmd_env_memory);
    }

    if (is_builtin_word(argv_copy[0]))
    {
        status = handle_builtin_execution(ast_cmd, argv_copy, cmd_env_memory, gv_hash_map);
        free_copies(argv_copy, ast_cmd->argc, prefixes_copy,
                    ast_cmd->prefix_count);
        return status;
    }

    int pid = fork();

    // Check if fork suceed
    if (pid == -1)
    {
        perror("");

        free_copies(argv_copy, ast_cmd->argc, prefixes_copy,
                    ast_cmd->prefix_count);

        _exit(127); // Check this value
    }
    if (pid == 0)
    {
        setenv_from_memory(cmd_env_memory);

        execvp(argv_copy[0], argv_copy);

        fprintf(stderr, "42sh: %s: command not found\n", argv_copy[0]);
        free_copies(argv_copy, ast_cmd->argc, prefixes_copy,
                    ast_cmd->prefix_count);
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
        free_copies(argv_copy, ast_cmd->argc, prefixes_copy,
                    ast_cmd->prefix_count);
        memory_free(cmd_env_memory);
        return 129;
    }

    // Otherwise we return the status error code
    memory_free(cmd_env_memory);
    free_copies(argv_copy, ast_cmd->argc, prefixes_copy, ast_cmd->prefix_count);
    return WEXITSTATUS(status);
}
