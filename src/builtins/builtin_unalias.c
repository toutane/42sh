#include <getopt.h>

#include "builtins.h"
#include "utils/printers/printers.h"

#define ALIAS_DELIMITER "="

int builtin_unalias(int argc, char *argv[], struct mem *mem)
{
    if (mem == NULL)
    {
        _exit(EXIT_FAILURE);
    }

    if (argc == 1)
    {
        fprintf(stderr, "unalias: usage: unalias [-a] name [name ...]\n");
        return 2;
    }

    int unalias_all_flags = 0;

    // Parse options
    int opt = 0;
    optind = 0;
    while ((opt = getopt(argc, argv, "a")) != -1)
    {
        switch (opt)
        {
        case 'a':
            unalias_all_flags = 1;
            break;
        default:
            break;
        }
    }

    if (unalias_all_flags)
    {
        // Unalias all the hash_map
        hm_clear(mem->hm_alias);

        return 0;
    }

    // Unalias alias past in arguments
    int status = 0;
    for (int i = 1; i < argc; ++i)
    {
        if (hm_contains(mem->hm_alias, argv[i]))
        {
            hm_remove(mem->hm_alias, argv[i]);
        }
        else
        {
            status = 1;
            fprintf(stderr, "bash: unalias: %s: not found\n", argv[i]);
        }
    }

    return status;
}
