#include "builtins.h"

int builtin_alias(int argc, char *argv[], struct mem *mem)
{
    if (argc > 2 || !mem)
    {
        printf("42sh: alias: too many arguments\n");
        return 1;
    }
    if (argc == 1)
    {
        // Print all aliases word='substitute'
        printf("42sh: alias: -- Printing all aliases\n");
        return 0;
    }
    char *equal_ptr = strchr(argv[1], '=');
    if (!equal_ptr)
    {
        // Print alias word='substitute'
        printf("42sh: alias: -- Printing alias for %s\n", argv[1]);
        return 0;
    }
    printf("42sh: alias: -- Defining alias for %s\n", argv[1]);
    return 0;
}
