#define NB_BUILTINS 12
#define BUILTIN_ERROR 2

#include "builtins.h"

static struct builtin_matchs builtin_words[NB_BUILTINS] = {
    { .name = "echo", .fun = builtin_echo },
    { .name = "false", .fun = builtin_false },
    { .name = "true", .fun = builtin_true },
    { .name = "exit", .fun = builtin_exit },
    { .name = "break", .fun = builtin_break },
    { .name = "continue", .fun = builtin_continue },
    { .name = "export", .fun = builtin_export },
    { .name = "cd", .fun = builtin_cd },
    { .name = ".", .fun = builtin_dot },
    { .name = "unset", .fun = builtin_unset },
    { .name = "alias", .fun = builtin_alias },
    { .name = "unalias", .fun = builtin_unalias },
};

// check if the cord is a builtin command
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

int (*builtin_fun(char *word))(int, char **, struct mem *mem)
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
