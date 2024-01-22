#define _POSIX_C_SOURCE 200112L // For setenv and unsetenv
#define _XOPEN_SOURCE 500 // For strdup

#include "variables.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *get_key_from_assignment_word(char *assignment_word)
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

static char *get_value_from_assignment_word(char *assignment_word)
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

/*
void initialize_memory(struct hash_map *memory)
{
    char *pwd_value = strdup(getenv("PWD"));
    char **value_array = calloc(2, sizeof(char *));
    value_array[0] = pwd_value;

    char *key = calloc(4, sizeof(char));
    key[0] = 'P';
    key[1] = 'W';
    key[2] = 'D';

    memory_set(memory, key, value_array);
}
*/