#define _XOPEN_SOURCE 500

#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "builtins.h"
#include "../expansion/special_variables.h"

static char *curpath = NULL;

static void set_curpath(char *str)
{
    if (!curpath)
    {
        curpath = strdup(str);
    }
    else
    {
        free(curpath);
        curpath = strdup(str);
    }
}

static int rule_10(struct hash_map *memory)
{
    if (chdir(curpath) == -1)
    {
        fprintf(stderr, "42sh: cd: Error changing directory\n");
        return 1;
    }
    char **tab = calloc(2, sizeof(char *));
    char **tmp = memory_get(memory, "PWD");
    if (!tmp)
    {
        fprintf(stderr, "42sh: cd: Error getting OLDPWD\n");
        exit(1);
    }
    tab[0] = strdup(tmp[0]);
    memory_set(memory, strdup("OLDPWD"), tab);

    tab = calloc(2, sizeof(char *));
    tab[0] = strdup(curpath);
    memory_set(memory, strdup("PWD"), tab);

    free(curpath);
    curpath = NULL;
    return 0;
}

static int rule_8(struct hash_map *memory)
{
    // To canonical form
    char *tmp = realpath(curpath, NULL);
    set_curpath(tmp);
    free(tmp);

    // Rule 9 skipped at the moment
    return rule_10(memory);
}

static int rule_7(struct hash_map *memory)
{
    char **tmp = memory_get(memory, "PWD");
    if (!tmp)
    {
        fprintf(stderr, "42sh: cd: PWD not existing\n");
        exit(1);
    }
    char *PWD = tmp[0];

    if (curpath[0] != '/')
    {
        size_t len_pwd = strlen(PWD);
        size_t len_curpath = strlen(curpath);
        size_t len = 0;
        char *built_path = NULL;
        if (PWD[len_pwd - 1] == '/')
        {
            len = len_pwd + len_curpath;
            built_path = malloc(sizeof(char) * (len + 1));
            memcpy(built_path, PWD, len_pwd);
            memcpy(built_path + (len_pwd), curpath, len_curpath);
        }
        else
        {
            len = len_pwd + 1 + len_curpath;
            built_path = malloc(sizeof(char) * (len + 1));
            memcpy(built_path, PWD, len_pwd);
            built_path[len_pwd] = '/';
            memcpy(built_path + (len_pwd) + 1, curpath, len_curpath);
        }
        if (!built_path)
        {
            fprintf(stderr, "42sh: cd: Error allocation memory\n");
            exit(1);
        }
        built_path[len] = '\0';

        set_curpath(built_path);
        free(built_path);
    }
    return rule_8(memory);
}

static int rule_6(char *arg, struct hash_map *memory)
{
    set_curpath(arg);
    return rule_7(memory);
}

static int rule_5(char *arg, struct hash_map *memory)
{
    char *CDPATH = memory_get(memory, "CDPATH")[0];

    char *seek = CDPATH;
    char *next_path = (strchr(seek, ':') ? strchr(seek, ':') : strchr(seek, '\0'));
    while (next_path)
    {
        char *built_path = NULL;
        if (next_path != seek)
        {
            // Build the path : path + / +  arg
            size_t len = next_path - seek + strlen(arg) + 1;
            built_path = malloc(sizeof(char) * (len + 1));

            memcpy(built_path, seek, next_path - seek);

            built_path[next_path - seek] = '/';

            memcpy(built_path + (next_path - seek) + 1, arg, strlen(arg));
            built_path[len] = '\0';
        }
        else
        {
            // pathname is null
            // Build the path : . + / +  arg
            size_t len = strlen(arg) + 2;
            built_path = malloc(sizeof(char) * (len + 1));

            built_path[0] = '.';

            built_path[1] = '/';

            memcpy(built_path + 2, arg, strlen(arg));
            built_path[len] = '\0';
        }

        // Check if exists
        DIR *dir = opendir(built_path);
        if (dir)
        {
            set_curpath(built_path);
            closedir(dir);
            free(built_path);
            return rule_7(memory);
        }
        free(built_path);

        if (*next_path == '\0')
        {
            break;
        }
        seek = next_path + 1;
        next_path = (strchr(seek, ':') ? strchr(seek, ':') : strchr(seek, '\0'));
    }
    return rule_6(arg, memory);
}

int builtin_cd(int argc, char *argv[], struct hash_map *memory)
{
    char *HOME = NULL;
    if (argc > 2)
    {
        fprintf(stderr, "42sh: cd: too many arguments\n");
        return 1;
    }

    // no args see HOME env var
    char *arg = NULL;
    if (argc == 1)
    {
        if (!HOME || HOME[0] == '\0')
        {
            // if HOME is empty or undefined
            // Rule 1
            return 0;
        }
        else
        {
            // else not empty: behave as if argv[1] was $HOME
            // Rule 2
            arg = HOME;
        }
    }
    else
    {
        arg = argv[1];
        char **tmp = memory_get(memory, "OLDPWD");
        if (strlen(arg) == 1 && arg[0] == '-' && tmp)
        {
            char **argv2 = calloc(2, sizeof(char *));
            argv2[0] = argv[0];
            argv2[1] = tmp[0];
            int res = builtin_cd(argc, argv2, memory);
            free(argv2);
            tmp = memory_get(memory, "PWD");
            printf("%s\n", tmp[0]);
            return res;
        }
    }

    if (arg[0] == '/')
    {
        // Rule 3
        set_curpath(arg);
        return rule_7(memory);
    }
    char *first_elt = (strchr(arg, '/') ? strchr(arg, '/') : strchr(arg, '\0'));
    if (((first_elt - arg == 1) && !memcmp(arg, ".", 1))
        || ((first_elt - arg == 2) && !memcmp(arg, "..", 2)))
    {
        // Rule 4 -> Rule 6
        return rule_6(argv[1], memory);
    }
    else
    {
        // Rule 5
        return rule_5(argv[1], memory);
    }
}
