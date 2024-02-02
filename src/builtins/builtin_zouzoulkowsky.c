#include "builtins.h"

int builtin_zouzoulkowsky(int argc, char *argv[], struct mem *mem)
{
    if (argc == 0 || argv == NULL || mem == NULL)
    {
        printf("Tiger\n");
        return 127;
    }

    printf("Tiger\n");
    return 127;
}
