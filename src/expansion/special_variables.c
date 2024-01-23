#define _XOPEN_SOURCE 500 // For strdup

#include "special_variables.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
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

void fill_random(struct hash_map *memory)
{
    int random_number = rand() % 32768;
    char *pwd_value = int_to_string(random_number);

    char **value_array = calloc(2, sizeof(char *));
    value_array[0] = pwd_value;

    char *key = calloc(7, sizeof(char));
    key[0] = 'R';
    key[1] = 'A';
    key[2] = 'N';
    key[3] = 'D';
    key[4] = 'O';
    key[5] = 'M';
    key[6] = '\0';

    memory_set(memory, key, value_array);
}

void set_uid_env_var(struct hash_map *memory)
{
    char *pwd_value = int_to_string(getuid());
    char **value_array = calloc(2, sizeof(char *));
    value_array[0] = pwd_value;

    char *key = calloc(4, sizeof(char));
    key[0] = 'U';
    key[1] = 'I';
    key[2] = 'D';
    key[3] = '\0';

    memory_set(memory, key, value_array);
}

void set_pwd(struct hash_map *memory)
{
    char *tmp = getenv("PWD");
    if (tmp == NULL)
    {
        return;
    }

    char *pwd_value = strdup(tmp);
    char **value_array = calloc(2, sizeof(char *));
    value_array[0] = pwd_value;

    char *key = calloc(4, sizeof(char));
    key[0] = 'P';
    key[1] = 'W';
    key[2] = 'D';
    key[3] = '\0';

    memory_set(memory, key, value_array);
}

void set_oldpwd(struct hash_map *memory)
{
    char *tmp = getenv("OLDPWD");
    if (tmp == NULL)
    {
        return;
    }

    char *pwd_value = strdup(tmp);
    char **value_array = calloc(2, sizeof(char *));
    value_array[0] = pwd_value;

    char *key = calloc(7, sizeof(char));
    key[0] = 'O';
    key[1] = 'L';
    key[2] = 'D';
    key[3] = 'P';
    key[4] = 'W';
    key[5] = 'D';
    key[6] = '\0';

    memory_set(memory, key, value_array);
}

void set_ifs(struct hash_map *memory)
{
    char *tmp = getenv("IFS");
    if (tmp == NULL)
    {
        return;
    }

    char *pwd_value = strdup(tmp);
    char **value_array = calloc(2, sizeof(char *));
    value_array[0] = pwd_value;

    char *key = calloc(4, sizeof(char));
    key[0] = 'I';
    key[1] = 'F';
    key[2] = 'S';
    key[3] = '\0';

    memory_set(memory, key, value_array);
}
