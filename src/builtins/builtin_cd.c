#define _XOPEN_SOURCE 500
#define _POSIX_C_SOURCE 200112L

#define PATH_MAX 4096

#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "builtins.h"
#include "utils/my_realpath/my_realpath.h"

static char *curpath = NULL;
static char *pwd = NULL;

static void set_curpath(char *str)
{
    if (!curpath)
    {
        curpath = strdup(str);
    }
    else
    {
        free(curpath);
        curpath = strdup((str ? str : ""));
    }
}

static int rule_10(void)
{
    if (chdir(curpath) == -1)
    {
        fprintf(stderr, "42sh: cd: Error changing directory\n");
        return 1;
    }

    if (!pwd)
    {
        fprintf(stderr, "42sh: cd: Error getting PWD\n");
        exit(1);
    }

    setenv("OLDPWD", pwd, 1);
    setenv("PWD", curpath, 1);

    free(curpath);
    free(pwd);
    pwd = NULL;
    curpath = NULL;

    return 0;
}

static int rule_8(void)
{
    // To canonical form
    char *rlpath = my_realpath(curpath);

    set_curpath(rlpath);
    free(rlpath);

    // Rule 9 skipped at the moment
    return rule_10();
}

static int rule_7(void)
{
    if (!pwd)
    {
        fprintf(stderr, "42sh: cd: PWD not existing\n");
        exit(1);
    }

    if (curpath[0] != '/')
    {
        size_t len_pwd = strlen(pwd);
        size_t len_curpath = strlen(curpath);
        size_t len = 0;
        char *built_path = NULL;

        if (pwd[len_pwd - 1] == '/')
        {
            len = len_pwd + len_curpath;
            built_path = malloc(sizeof(char) * (len + 1));
            memcpy(built_path, pwd, len_pwd);
            memcpy(built_path + (len_pwd), curpath, len_curpath);
        }
        else
        {
            len = len_pwd + 1 + len_curpath;
            built_path = malloc(sizeof(char) * (len + 1));
            memcpy(built_path, pwd, len_pwd);
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

    return rule_8();
}

static int rule_6(char *arg)
{
    set_curpath(arg);

    return rule_7();
}

int builtin_cd(int argc, char *argv[], struct mem *mem)
{
    char *home = getenv("HOME");
    pwd = getcwd(NULL, PATH_MAX);

    if (argc > 2)
    {
        fprintf(stderr, "42sh: cd: too many arguments\n");
        free(pwd);
        return 1;
    }

    // no args see HOME env var
    char *arg = NULL;
    if (argc == 1)
    {
        if (!home || home[0] == '\0')
        {
            // if HOME is empty or undefined
            // Rule 1
            free(pwd);
            return 0;
        }
        else
        {
            // else not empty: behave as if argv[1] was $HOME
            // Rule 2
            arg = home;
        }
    }
    else
    {
        arg = argv[1];
        char *oldpwd = getenv("OLDPWD");
        if (strlen(arg) == 1 && arg[0] == '-' && oldpwd != NULL)
        {
            char **argv2 = calloc(2, sizeof(char *));
            argv2[0] = argv[0];
            argv2[1] = oldpwd;
            free(pwd);
            int res = builtin_cd(argc, argv2, mem);
            free(argv2);
            printf("%s\n", getenv("PWD"));
            return res;
        }
    }

    if (arg[0] == '/')
    {
        // Rule 3
        set_curpath(arg);
        return rule_7();
    }
    char *first_elt = (strchr(arg, '/') ? strchr(arg, '/') : strchr(arg, '\0'));
    if (((first_elt - arg == 1) && !memcmp(arg, ".", 1))
        || ((first_elt - arg == 2) && !memcmp(arg, "..", 2)))
    {
        // Rule 4 -> Rule 6
        return rule_6(argv[1]);
    }
    else
    {
        // Rule 5
        return rule_6(argv[1]);
    }
}
