#define _XOPEN_SOURCE 500 // For strdup

#include "special_variables.h"

#include <getopt.h>
#include <stdlib.h>
#include <string.h>

void set_cdpath(struct hash_map *memory)
{
    char *tmp = getenv("CDPATH");
    if (tmp == NULL)
    {
        return;
    }

    char *home_value = strdup(tmp);
    char **value_array = calloc(2, sizeof(char *));
    value_array[0] = home_value;

    char *key = calloc(7, sizeof(char));
    key[0] = 'C';
    key[1] = 'D';
    key[2] = 'P';
    key[3] = 'A';
    key[4] = 'T';
    key[5] = 'H';
    key[6] = '\0';

    memory_set(memory, key, value_array);
}
