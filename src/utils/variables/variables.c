#define _POSIX_C_SOURCE 200112L // For setenv and unsetenv

#include "variables.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "expansion/special_variables.h"

char *get_key_from_assignment_word(char *assignment_word)
{
    char *key = NULL;

    // Get the position of the '=' character if any, NULL otherwise
    int equal_sign_pos = get_index_of_char(assignment_word, '=');

    // Allocate memory for the key
    key = calloc((equal_sign_pos + 1), sizeof(char));

    // Copy the key into the allocated memory
    strncpy(key, assignment_word, equal_sign_pos);

    return key;
}

char *get_value_from_assignment_word(char *assignment_word)
{
    char *value = NULL;

    // Get the position of the '=' character if any, NULL otherwise
    int equal_sign_pos = get_index_of_char(assignment_word, '=');

    // Allocate memory for the value
    value = malloc(sizeof(char) * (strlen(assignment_word) - equal_sign_pos));

    // Copy the value into the allocated memory
    strncpy(value, assignment_word + equal_sign_pos + 1,
            strlen(assignment_word) - equal_sign_pos);

    return value;
}

/* Set a variable from an assignment word.
 * The variable is set in the given memory space.
 * An assignment_word takes the following form: <name>=<word>
 * The <name> should be valid (see XBD), but the <word> can be anything.
 * This function handles the logic of checking if the variable already exists,
 * in this case, the value of the variable is updated.  */

void set_var_from_assignment_word(struct hash_map *memory,
                                  char *assignment_word_str)
{
    if (memory == NULL || assignment_word_str == NULL)
    {
        return;
    }

    char *key = get_key_from_assignment_word(assignment_word_str);
    char *value = get_value_from_assignment_word(assignment_word_str);

    /* Because memory stores variables as arrays of strings (null terminated),
     * we need to convert the value into an array of strings. */

    // Allocate memory for the array of strings
    char **value_array = calloc(2, sizeof(char *));
    value_array[0] = value;

    memory_set(memory, key, value_array);

    return;
}

static void setenv_from_var(char *key, char **value)
{
    if (key == NULL || value == NULL)
    {
        return;
    }

    if (setenv(key, value[0], 1))
    {
        fprintf(stderr, "42sh: setenv: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void setenv_from_memory(struct hash_map *memory)
{
    hash_map_map(memory, setenv_from_var);
}

static void unsetenv_from_var(char *key, char **value)
{
    if (key == NULL || value == NULL)
    {
        return;
    }

    if (unsetenv(key))
    {
        fprintf(stderr, "42sh: unsetenv: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void unsetenv_from_memory(struct hash_map *memory)
{
    hash_map_map(memory, unsetenv_from_var);
}

void set_default_variables(int argc, char **argv, struct hash_map *memory)
{
    // Seed the random number generator
    srand((unsigned int)time(NULL));

    /* Add special variables to shell memory:
     * $@, $*, $1...n, $$, $# */

    // fill_at_sign_var(argc, argv, memory);
    fill_arguments_var(argc, argv, memory);
    // fill_star_sign_var(argc, argv, memory);
    fill_dollar_var(memory);
    fill_arguments_amount(argc, memory);
    set_uid_env_var(memory);
    fill_random(memory);

    /* Add environment variables to shell memory:
     * PWD, OLDPWD, IFS */

    set_pwd(memory);
    set_oldpwd(memory);
    set_ifs(memory);
}

static void update_env_var(const char *key, const char *value)
{
    int status = setenv(key, value, 1);
    if (status != 0)
    {
        fprintf(stderr, "42sh: update_env_var: fail to setenv\n");
        return;
    }
}

void assign_variable(char *assignment_word, struct hm *hm)
{
    char *key = get_key_from_assignment_word(assignment_word);
    char *value = get_value_from_assignment_word(assignment_word);

    if (key == NULL || value == NULL)
    {
        free(key);
        free(value);
        return;
    }

    // Check if the variable is exported (so in the environment)
    int is_exported = getenv(key) != NULL;

    if (is_exported)
    {
        // We must update the environment with the new value of the variable
        update_env_var(key, value);
    }
    else
    {
        // Variable isn't exported, we must update the shell internal variables
        hm_set_var(hm, key, value);
    }

    free(key);
    free(value);
}

char *get_variable(char *key, struct hm *hm)
{
    if (key == NULL || hm == NULL)
    {
        return NULL;
    }

    char *value = NULL;

    // Check if the variable is exported (so in the environment)
    int is_exported = getenv(key) != NULL;

    if (is_exported)
    {
        // We must update the environment with the new value of the variable
        value = getenv(key);
    }
    else
    {
        // Variable isn't exported, we must update the shell internal variables
        value = hm_get(hm, key);
    }

    return value;
}

void setenv_from_hm(struct hm *hm)
{
    if (hm == NULL)
    {
        return;
    }

    for (size_t i = 0; i < hm->size; i++)
    {
        struct pl *cur = hm->pairs[i];
        while (cur)
        {
            setenv(cur->key, cur->data, 1);
            cur = cur->next;
        }
    }
}
