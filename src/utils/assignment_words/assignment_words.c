#include "utils/assignment_words/assignment_words.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int is_valid_name_char(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'
        || (c >= '0' && c <= '9');
}

int is_str_assignment_word(char *str)
{
    // Get the position of the '=' character if any, NULL otherwise
    char equal_sign_pos = get_index_of_char(str, '=');

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
