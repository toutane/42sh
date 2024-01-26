#include "utils/variables/variables.h"
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

static void prefixes_expansion(struct ast_cmd *ast_cmd, char **prefixes_copy,
                               struct hash_map *memory)
{
    /* Expand the quoting of prefixes, this contains backslash, double quote,
     * single quote and parameter ('$' and '${' ) expansion. For the parameter
     * expansion, the algorithm will search into the given hash_map and properly
     * expand variable identifiers. */

    for (int i = 0; i < ast_cmd->prefix_count; i++)
    {
        prefixes_copy[i] = expand_string(&(ast_cmd->prefix[i]), memory);
        set_var_from_assignment_word(memory, prefixes_copy[i]);
    }
}

static int handle_builtin_execution(int expanded_argc, char **expanded_argv,
                                    struct hash_map *memory)
{
    setenv_from_memory(memory);

    int status = (builtin_fun(expanded_argv[0]))(expanded_argc, expanded_argv);

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

    // Expand arguments
    int expanded_argc = ast_cmd->argc;
    char **expanded_argv =
        argv_expansions(ast_cmd->argv, &expanded_argc, gv_hash_map);

    char **prefixes_copy = calloc(ast_cmd->prefix_count + 1, sizeof(char *));

    if (ast_cmd->argc == 0)
    {
        prefixes_expansion(ast_cmd, prefixes_copy, gv_hash_map);

        struct hm *hm = hm_new(HM_VARIABLE, 10, free);

        for (int i = 0; i < ast_cmd->prefix_count; i++)
        {
            assign_variable(prefixes_copy[i], hm);
        }

        hm_print(hm);

        printf("value of a: %s\n", get_variable("a", hm));

        hm_free(hm);
        free_copies(expanded_argv, expanded_argc, prefixes_copy,
                    ast_cmd->prefix_count);
        return 0;
    }

    struct hash_map *cmd_env_memory = memory_new();

    struct hm *temp_hm = hm_new(HM_VARIABLE, 10, free);

    if (ast_cmd->prefix_count > 0)
    {
        prefixes_expansion(ast_cmd, prefixes_copy, cmd_env_memory);

        for (int i = 0; i < ast_cmd->prefix_count; i++)
        {
            char *word = prefixes_copy[i];
            char *key = get_key_from_assignment_word(word);
            char *value = get_value_from_assignment_word(word);
            hm_set_var(temp_hm, key, value);
        }
    }

    // check if word is a build -> no need to fork
    if (is_builtin_word(expanded_argv[0]))
    {
        status = handle_builtin_execution(expanded_argc, expanded_argv,
                                          cmd_env_memory);
        free_copies(expanded_argv, expanded_argc, prefixes_copy,
                    ast_cmd->prefix_count);
        return status;
    }

    int pid = fork();

    // Check if fork suceed
    if (pid == -1)
    {
        perror("");

        free_copies(expanded_argv, expanded_argc, prefixes_copy,
                    ast_cmd->prefix_count);

        _exit(127); // Check this value
    }
    if (pid == 0)
    {
        // setenv_from_memory(cmd_env_memory);
        setenv_from_hm(temp_hm);

        execvp(expanded_argv[0], expanded_argv);

        fprintf(stderr, "42sh: %s: command not found\n", expanded_argv[0]);
        free_copies(expanded_argv, expanded_argc, prefixes_copy,
                    ast_cmd->prefix_count);
        _exit(execvp_error(errno));
    }
    else
    {
        waitpid(pid, &status, 0);
    }
    hm_free(temp_hm);

    // Check if the command was interrupted by a signal
    if (WIFSIGNALED(status))
    {
        fprintf(stderr, "42sh: command terminated because of a signal");
        free_copies(expanded_argv, expanded_argc, prefixes_copy,
                    ast_cmd->prefix_count);
        memory_free(cmd_env_memory);
        return 129;
    }

    // Otherwise we return the status error code
    memory_free(cmd_env_memory);
    free_copies(expanded_argv, expanded_argc, prefixes_copy,
                ast_cmd->prefix_count);
    return WEXITSTATUS(status);
}
