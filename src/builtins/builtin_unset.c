#define _POSIX_C_SOURCE 200112L // For setenv and unsetenv

#include <getopt.h>

#include "builtins.h"

int builtin_unset(int argc, char *argv[], struct mem *mem)
{
    if (mem == NULL)
    {
        _exit(EXIT_FAILURE);
    }

    int var_flag = 0;
    int fun_flag = 0;

    // Parse options
    int opt;
    while ((opt = getopt(argc, argv, "fv")) != -1)
    {
        switch (opt)
        {
        case 'f':
            fun_flag = 1;
            break;
        case 'v':
            var_flag = 1;
            break;
        default:
            break;
        }
    }

    if (var_flag == 0 && fun_flag == 0)
    {
        var_flag = 1;
    }

    if (var_flag && fun_flag)
    {
        fprintf(stderr,
                "42sh: unset: cannot unset variables and functions at the same "
                "time\n");
        return 1;
    }

    for (int i = 1; i < argc; i++)
    {
        char *name = argv[i];

        if (var_flag)
        {
            unsetenv(name);
            hm_remove(mem->hm_var, name);
        }

        if (fun_flag)
        {
            hm_remove(mem->hm_fun, name);
        }
    }

    return 0;
}
