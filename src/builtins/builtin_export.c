#define _POSIX_C_SOURCE 200112L // For setenv and unsetenv

#include <stdio.h>

#include "builtins.h"

int builtin_export(int argc, char *argv[], struct mem *mem)
{
    if (mem == NULL)
    {
        _exit(EXIT_FAILURE);
    }

    for (int i = 1; i < argc; i++)
    {
        char *word = argv[i];

        if (is_str_assignment_word(word))
        {
            // Case "export name=word"
            char *key = get_key_from_assignment_word(word);
            char *value = get_value_from_assignment_word(word);

            setenv(key, value, 1);

            free(key);
            free(value);
        }
        else if (is_name(word))
        {
            // Case "export name"
            // TODO: Try temp hm implementation to follow bash --posix
            if (getenv(word) != NULL)
            {
                return 0;
            }

            char *internal_value = hm_get(mem->hm_var, word);
            setenv(word, !internal_value ? "" : internal_value, 1);
        }
        else
        {
            fprintf(stderr, "42sh: export: %s: not a valid identifer\n", word);
            return 1;
        }
    }

    return 0;
}
