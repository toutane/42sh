#define _POSIX_C_SOURCE

#include <dirent.h>
#include <string.h>
#include <sys/types.h>

#include "builtins.h"

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

static int rule_2(char *arg)
{
    // TODO: chdir to $HOME
    return 0;
}

static int rule_3(char *arg)
{
    // TODO: chdir to argv[1]
    set_curpath(argv[1]);
    // Goto step 7
    return rule_8();
}

static int rule_5(char *arg)
{
    char *CDPATH = "/home/alexandre:::/afs/devine::/hoorible/man";

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
        }
        free(built_path);

        if (*next_path == '\0')
        {
            break;
        }
        seek = next_path + 1;
        next_path = (strchr(seek, ':') ? strchr(seek, ':') : strchr(seek, '\0'));
    }
    return rule_8();
}

static int rule_6(char *arg)
{
    char *PWD = "";

    size_t len_pwd = strlen(PWD);
    size_t len = len_pwd + 1 + strlen(arg);
    char *built_path = malloc(sizeof(char) * (len + 1));

    memcpy(built_path, PWD, strlen(PWD));

    built_path[len_pwd] = '/';

    memcpy(built_path + (len_pwd) + 1, arg, strlen(arg));
    built_path[len] = '\0';

    set_curpath(built_path);
    free(built_path);

    return rule_8();
}

static int rule_8(char *arg)
{
    // To canonical form
    if (!curpath)
    {
        return 0;
    }

    char *seek = curpath;
    char *next_elt = (strchr(curpath, '/') ? strchr(curpath, '/') : strchr(curpath, '\0'));
    while (next_elt)
    {
        if (next_path != seek)
        {
            // "//"
        }
        else
        {
            if ((next_elt - seek == 1) && !memcmp(seek, ".", 1))
            {
                // Remove ./
            }
        }

        if (*next_elt == '\0')
        {
            break;
        }
        next_elt = (strchr(curpath, '/') ? strchr(curpath, '/') : strchr(curpath, '\0'));
    }
}

int builtin_cd(int argc, char *argv[])
{
    char *HOME = NULL;
    if (argc > 2)
    {
        fprintf(stderr, "42sh: cd: too many arguments\n");
        return 1;
    }

    // no args see HOME env var
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
            return rule_2(argv[1]);
        }
    }
    if (argv[1][0] == '/')
    {
        // Rule 3
        return rule_3(argv[1]);
    }
    else
    {
        char *first_elt = (strchr(argv[1], '/') ? strchr(argv[1], '/') : strchr(argv[1], '\0'));
        if (((first_elt - argv[1] == 1) && !memcmp(argv[1], ".", 1))
            || ((first_elt - argv[1] == 2) && !memcmp(argv[1], "..", 2)))
        {
            // Rule 4 -> Rule 6
            return rule_6(argv[1]);
        }
        else
        {
            // Rule 5
            return rule_5(argv[1]);
        }
    }
    free(curpath);
    return 0;
}
