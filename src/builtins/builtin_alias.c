#include "builtins.h"
#include "utils/printers/printers.h"

#define ALIAS_DELIMITER "="

int builtin_alias(int argc, char *argv[], struct mem *mem)
{
    if (argc > 2 || !mem)
    {
        printf("42sh: alias: too many arguments\n");
        return 1;
    }

    if (argc == 1)
    {
        // Print all aliases word='substitute', don't follow SCL
        hm_print(mem->hm_alias);
        return 0;
    }

    char *key = NULL;
    char *value = NULL;
    char *equal_ptr = strchr(argv[1], '=');
    if (!equal_ptr)
    {
        value = hm_get(mem->hm_alias, argv[1]);
        key = argv[1];
        if (value == NULL)
        {
            // Error unknow argv past to alias
            fprintf(stderr, "42sh: alias: %s: not found\n", argv[1]);
            return 1;
        }
        printf("%s='%s'\n", key, value);

        return 0;
    }

    // Split function base to =
    key = strtok(argv[1], ALIAS_DELIMITER);
    value = strtok(NULL, ALIAS_DELIMITER);

    // set alias
    hm_set_var(mem->hm_alias, key, value);

    return 0;
}
