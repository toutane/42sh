#include "builtins.h"

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
