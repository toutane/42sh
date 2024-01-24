#include "builtins.h"

int builtin_cd(int argc, char *argv[])
{
    if (argc > 2 || !argv)
    {
        fprintf(stderr, "42sh: cd: too many arguments\n");
        return 1;
    }

    // no args see HOME env var
    if (argc == 1)
    {
        
        return 0;
    }
    return 0;
}
