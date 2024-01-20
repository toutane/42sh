#include "special_variables.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void fill_at_sign_var(int argc, char *argv[], struct hash_map *gv_hash_map)
{
    char **arguments_array = calloc((argc - optind), sizeof(char *));

    int k = 0;

    for (int i = optind + 1; i < argc; i++)
    {
        arguments_array[k] = calloc(strlen(argv[i]) + 1, sizeof(char));
        memcpy(arguments_array[k], argv[i], strlen(argv[i]));
        k++;
    }

    int i = 0;

    char *key = calloc(2, sizeof(char));
    key[0] = '@';
    key[1] = '\0';

    hash_map_insert(gv_hash_map, key, arguments_array, &i);
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

        int j = 0;
        hash_map_insert(gv_hash_map, key, arguments_array, &j);
        k++;
    }
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

    int i = 0;

    char *key = calloc(2, sizeof(char));
    key[0] = '*';
    key[1] = '\0';

    hash_map_insert(gv_hash_map, key, arguments_array, &i);
}
