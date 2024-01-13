#define NB_BUILTINS 3

#include "builtins.h"

static struct builtin_matchs builtin_words[NB_BUILTINS] = {
    { .name = "echo", .fun = builtin_echo },
    { .name = "false", .fun = builtin_false },
    { .name = "true", .fun = builtin_true }
};

char is_builtin_word(char *word)
{
    for (int i = 0; i < NB_BUILTINS; i++)
    {
        if (!strcmp(word, builtin_words[i].name))
        {
            return 1;
        }
    }
    return 0;
}

int (*builtin_fun(char *word))(int, char **)
{
    for (int i = 0; i < NB_BUILTINS; i++)
    {
        if (!strcmp(word, builtin_words[i].name))
        {
            return builtin_words[i].fun;
        }
    }
    return 0;
}

int builtin_false(int argc, char *argv[])
{
    // Need to use useless (because generic) arguments
    if (!argc || !argv)
    {
        return 1;
    }
    return 1;
}

int builtin_true(int argc, char *argv[])
{
    // Need to use useless (because generic) arguments
    if (!argc || !argv)
    {
        return 0;
    }
    return 0;
}
