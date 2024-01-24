#define _XOPEN_SOURCE 500 // For strdup

#include "special_variables.h"

#include <getopt.h>
#include <stdlib.h>
#include <string.h>

void set_home(char *key, struct hash_map *memory)
{
    char *tmp = getenv("HOME");
    if (tmp == NULL)
    {
        return;
    }

    char *home_value = strdup(tmp);
    char **value_array = calloc(2, sizeof(char *));
    value_array[0] = home_value;

    char *key = calloc(5, sizeof(char));
    key[0] = 'H';
    key[1] = 'O';
    key[2] = 'M';
    key[3] = 'E';
    key[4] = '\0';

    memory_set(memory, key, value_array);
}
