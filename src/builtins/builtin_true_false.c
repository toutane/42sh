#include "builtins.h"

int builtin_false(int argc, char *argv[], struct mem *mem)
{
    // Need to use useless (because generic) arguments
    if (!argc || !argv || !mem)
    {
        return 1;
    }
    return 1;
}

int builtin_true(int argc, char *argv[], struct mem *mem)
{
    // Need to use useless (because generic) arguments
    if (!argc || !argv || !mem)
    {
        return 0;
    }
    return 0;
}
