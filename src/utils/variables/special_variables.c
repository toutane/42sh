#define _POSIX_C_SOURCE 200112L

#include <dirent.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "utils/variables/variables.h"

static void set_uid(struct hm *hm_var)
{
    char *uid = int_to_string(getuid());
    hm_set_var(hm_var, "UID", uid);
    free(uid);
}

static void set_pwd(void)
{
    char *pwd = getcwd(NULL, 0);
    setenv("PWD", pwd, 1);
    free(pwd);
}

static void set_oldpwd(void)
{
    char *oldpwd = getenv("OLDPWD");
    if (oldpwd)
    {
        // Check if the oldpwd is a valid directory
        DIR *dir = opendir(oldpwd);
        if (dir == NULL)
        {
            unsetenv("OLDPWD");
        }
        else
        {
            closedir(dir);
        }
    }
}

void set_random(struct hm *hm_var)
{
    int random_number = rand() % 32768;
    char *pwd_value = int_to_string(random_number);
    assign_variable("RANDOM", pwd_value, hm_var);
    free(pwd_value);
}

static void set_dollar(struct hm *hm_var)
{
    char *pid = int_to_string(getpid());
    assign_variable("$", pid, hm_var);
    free(pid);
}

static void set_question_mark(struct hm *hm_var)
{
    assign_variable("?", "0", hm_var);
}

static void set_sharp(struct hm *hm_var)
{
    assign_variable("#", "0", hm_var);
}

static void set_ifs(struct hm *hm_var)
{
    char *ifs = getenv("IFS");
    if (ifs == NULL)
    {
        assign_variable("IFS", " \t\n", hm_var);
    }
}

void set_default_variables(struct hm *hm_var)
{
    // Seed the random number generator
    srand((unsigned int)time(NULL));

    // Set special variables
    set_question_mark(hm_var);
    set_sharp(hm_var);
    set_dollar(hm_var);
    set_random(hm_var);
    set_uid(hm_var);

    // Set environment variables
    set_pwd();
    set_oldpwd();
    set_ifs(hm_var);
}
