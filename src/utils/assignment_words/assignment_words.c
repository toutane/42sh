#include "utils/assignment_words/assignment_words.h"

#include <stdlib.h>
#include <string.h>

int is_valid_name_char(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'
        || (c >= '0' && c <= '9');
}

/* Check if the given string contains the given char, return the position of the
 * target if it exists, 0 otherwise */
static int contains_char(char *str, char target)
{
    size_t target_pos = 0;
    while (str[target_pos] != '\0')
    {
        if (str[target_pos] == target)
        {
            return target_pos;
        }
        target_pos++;
    }

    return 0;
}

static int is_str_assignment_word(char *str)
{
    // Get the position of the '=' character if any, NULL otherwise
    char equal_sign_pos = contains_char(str, '=');

    if (equal_sign_pos == 0)
    {
        return 0;
    }

    // Check if the string before the equal sign is a valid name
    while (equal_sign_pos > 0)
    {
        if (!is_valid_name_char(str[equal_sign_pos - 1]))
        {
            return 0;
        }
        equal_sign_pos--;
    }

    return 1;
}

int only_assignment_words(char **argv)
{
    int i = 0;
    while (argv[i] != NULL)
    {
        if (!is_str_assignment_word(argv[i]))
        {
            return 0;
        }
        i++;
    }
    return 1;
}

static void get_key_from_assignment_word(char **key, char *assignment_word)
{
    // Get the position of the '=' character if any, NULL otherwise
    int equal_sign_pos = contains_char(assignment_word, '=');

    // Allocate memory for the key
    *key = malloc(sizeof(char) * equal_sign_pos);

    // Copy the key into the allocated memory
    strncpy(*key, assignment_word, equal_sign_pos);
    (*key)[equal_sign_pos] = '\0';
}

static void get_value_from_assignment_word(char **value, char *assignment_word)
{
    // Get the position of the '=' character if any, NULL otherwise
    int equal_sign_pos = contains_char(assignment_word, '=');

    // Allocate memory for the value
    *value = malloc(sizeof(char) * (strlen(assignment_word) - equal_sign_pos));

    // Copy the value into the allocated memory
    strncpy(*value, assignment_word + equal_sign_pos + 1,
            strlen(assignment_word) - equal_sign_pos);
    (*value)[strlen(assignment_word) - equal_sign_pos] = '\0';
}

void update_gv_hash_map(char **argv, struct hash_map *gv_hash_map)
{
    int i = 0;
    while (argv[i] != NULL)
    {
        char *key = NULL;
        get_key_from_assignment_word(&key, argv[i]);

        char *value = NULL;
        get_value_from_assignment_word(&value, argv[i]);

        int updated = 0;
        hash_map_insert(gv_hash_map, key, value, &updated);

        i++;
    }
}
