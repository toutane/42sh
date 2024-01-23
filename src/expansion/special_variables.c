#include "special_variables.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

void fill_at_sign_var(int argc, char *argv[], struct hash_map *gv_hash_map)
{
    int l = argc - optind;
    l = (l <= 0) ? 1 : l;
    char **arguments_array = calloc(l, sizeof(char *));

    int k = 0;

    for (int i = optind + 1; i < argc; i++)
    {
        arguments_array[k] = calloc(strlen(argv[i]) + 1, sizeof(char));
        memcpy(arguments_array[k], argv[i], strlen(argv[i]));
        k++;
    }

    char *key = calloc(2, sizeof(char));
    key[0] = '@';
    key[1] = '\0';

    hash_map_insert(gv_hash_map, key, arguments_array);
}

void fill_arguments_var(int argc, char *argv[], struct hash_map *gv_hash_map)
{
    int k = 1;

    for (int i = optind + 1; i < argc; i++)
    {
        char *key = int_to_string(k);

        char **arguments_array = calloc(2, sizeof(char *));

        arguments_array[0] = calloc(strlen(argv[i]) + 1, sizeof(char));

        memcpy(arguments_array[0], argv[i], strlen(argv[i]));

        hash_map_insert(gv_hash_map, key, arguments_array);
        k++;
    }
}

void fill_dollar_var(struct hash_map *gv_hash_map)
{
    char **arguments_array = calloc(2, sizeof(char *));

    // get pid of the process
    pid_t pid = getpid();
    // get the size of the number
    int n = snprintf(NULL, 0, "%d", pid);

    arguments_array[0] = calloc(n + 1, sizeof(char));
    snprintf(arguments_array[0], n + 1, "%d", pid);

    char *key = calloc(2, sizeof(char));
    key[0] = '$';
    key[1] = '\0';

    hash_map_insert(gv_hash_map, key, arguments_array);

    return;
}

void fill_star_sign_var(int argc, char *argv[], struct hash_map *gv_hash_map)
{
    char **arguments_array = calloc(1, sizeof(char *));

    int n = 0;

    for (int i = optind + 1; i < argc; i++)
    {
        // Separate arguments by spaces

        char *token = strtok(argv[i], " ");
        while (token != NULL)
        {
            n++;
            arguments_array =
                realloc(arguments_array, (n + 1) * sizeof(char *));
            arguments_array[n] = NULL;

            arguments_array[n - 1] = calloc(strlen(token) + 1, sizeof(char));
            memcpy(arguments_array[n - 1], token, strlen(token));

            token = strtok(NULL, " ");
        }
    }

    char *key = calloc(2, sizeof(char));
    key[0] = '*';
    key[1] = '\0';

    hash_map_insert(gv_hash_map, key, arguments_array);
}

void fill_arguments_amount(int argc, struct hash_map *gv_hash_map)
{
    int arguments_amount = argc - optind;
    char *value = int_to_string(arguments_amount);

    char **arguments_array = calloc(2, sizeof(char *));
    arguments_array[0] = value;

    char *key = calloc(2, sizeof(char));
    key[0] = '#';
    key[1] = '\0';

    hash_map_insert(gv_hash_map, key, arguments_array);
}
