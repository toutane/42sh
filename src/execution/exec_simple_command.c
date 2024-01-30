#define _XOPEN_SOURCE

#include <stdio.h>
#include <stdlib.h>

#include "exec.h"
#include "utils/hash_map/hash_map.h"
#include "utils/memory/memory.h"
#include "utils/variables/variables.h"

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

static int handle_builtin_execution(int expanded_argc, char **expanded_argv,
                                    struct hm *hm_prefixes, struct mem *mem)
{
    save_env(hm_prefixes);

    int status =
        (builtin_fun(expanded_argv[0]))(expanded_argc, expanded_argv, mem);

    // Flush stdout to avoid mixing output
    fflush(NULL);

    restore_env(hm_prefixes);

    return status;
}

static int handle_empty_command(struct ast_cmd *ast_cmd, struct hm *hm_var)
{
    char **prefixes_copy = calloc(ast_cmd->prefix_count + 1, sizeof(char *));

    for (int i = 0; i < ast_cmd->prefix_count; i++)
    {
        /* Expand the quoting of prefixes, this contains backslash, double
         * quote, single quote and parameter ('$' and '${' ) expansion. For the
         * parameter expansion, the algorithm will search into the given
         * hash_map and properly expand variable identifiers. */

        prefixes_copy[i] = expand_string(&(ast_cmd->prefix[i]), hm_var);
        assign_variable_from_ass_word(prefixes_copy[i], hm_var);
    }

    for (int i = 0; i < ast_cmd->prefix_count; ++i)
    {
        free(prefixes_copy[i]);
    }
    free(prefixes_copy);

    return 0;
}

int eval_simple_command(struct ast *ast, struct mem *mem)
{
    int status = 0;
    struct ast_cmd *ast_cmd = (struct ast_cmd *)ast;

    /* If the command is empty, we only need to expand the prefixes and assign
     * the variables to the shell environment or to the shell internal
     * variables. Example of empty command: foo=bar baz=toto (two prefixes
     * only). Example of non-empty command: foo=bar echo 'toto' (one prefix and
     * two arguments). */

    int is_command_empty = (ast_cmd->argc == 0);
    if (is_command_empty)
    {
        return handle_empty_command(ast_cmd, mem->hm_var);
    }

    /* We expand the arguments of the command, this contains backslash,
     * double quote, single quote and parameter ('$' and '${' ) expansion. For
     * the parameter expansion, the algorithm will search into the given
     * hash_map and properly expand variable identifiers. This will not free the
     * ast_cmd->argv original array and will return a new heap allocated array.
     */

    int expanded_argc = ast_cmd->argc;
    char **expanded_argv =
        argv_expansions(ast_cmd->argv, &expanded_argc, mem->hm_var);

    /* If the command is not empty, we need to expand the prefixes if any and
     * assign the variables to the environment of the command that will be
     * executed. */

    struct hm *hm_prefixes = hm_new(HM_VARIABLE, 10, free);

    char **prefixes_copy = calloc(ast_cmd->prefix_count + 1, sizeof(char *));

    for (int i = 0; i < ast_cmd->prefix_count; i++)
    {
        /* Expand the quoting of prefixes, this contains backslash, double
         * quote, single quote and parameter ('$' and '${' ) expansion. For
         * the parameter expansion, the algorithm will search into the given
         * hash_map and properly expand variable identifiers. */

        prefixes_copy[i] = expand_string(&(ast_cmd->prefix[i]), mem->hm_var);

        char *word = prefixes_copy[i];
        char *key = get_key_from_assignment_word(word);
        char *value = get_value_from_assignment_word(word);

        hm_set_var(hm_prefixes, key, value);

        free(key);
        free(value);
    }

    // check if word is a function -> no need to fork
    if (hm_contains(mem->hm_fun, expanded_argv[0]))
    {
    }

    // check if word is a build -> no need to fork either
    if (is_builtin_word(expanded_argv[0]))
    {
        status = handle_builtin_execution(expanded_argc, expanded_argv,
                                          hm_prefixes, mem);

        hm_free(hm_prefixes);
        free_copies(expanded_argv, expanded_argc, prefixes_copy,
                    ast_cmd->prefix_count);
        return status;
    }

    int pid = fork();

    // Check if fork suceed
    if (pid == -1)
    {
        perror("");

        hm_free(hm_prefixes);
        free_copies(expanded_argv, expanded_argc, prefixes_copy,
                    ast_cmd->prefix_count);

        _exit(127); // Check this value
    }
    if (pid == 0)
    {
        setenv_from_hm(hm_prefixes);

        execvp(expanded_argv[0], expanded_argv);

        fprintf(stderr, "42sh: %s: command not found\n", expanded_argv[0]);

        hm_free(hm_prefixes);
        free_copies(expanded_argv, expanded_argc, prefixes_copy,
                    ast_cmd->prefix_count);

        _exit(execvp_error(errno));
    }
    else
    {
        waitpid(pid, &status, 0);
    }

    hm_free(hm_prefixes);
    free_copies(expanded_argv, expanded_argc, prefixes_copy,
                ast_cmd->prefix_count);

    // Check if the command was interrupted by a signal
    if (WIFSIGNALED(status))
    {
        fprintf(stderr, "42sh: command terminated because of a signal");
        return 129;
    }

    // Otherwise we return the status error code
    return WEXITSTATUS(status);
}
