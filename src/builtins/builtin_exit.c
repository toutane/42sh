#include "builtins.h"

static int check_number(char *str)
{
    size_t i = 0;
    while (str[i] != 0 && str[i] >= '0' && str[i] <= '9')
    {
        i += 1;
    }

    return str[i] == 0;
}

int builtin_exit(int argc, char *argv[], struct mem *mem)
{
    if (mem == NULL)
    {
        _exit(EXIT_FAILURE);
    }

    // check number of argument
    if (argc > 2)
    {
        fprintf(stderr, "42sh: exit: too many arguments\n");
        _exit(EXIT_FAILURE);
    }

    // exit default value
    int status = 0;

    if (argc == 2)
    {
        // check if
        if (check_number(argv[1]))
        {
            status = atoi(argv[1]);
        }
        else
        {
            status = 2;
            fprintf(stderr, "42sh: exit: %s: numeric argument required\n",
                    argv[1]);
        }
    }

    _exit(status);
}
