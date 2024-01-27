#define INT_MAX 2147483647

#include "builtins.h"

static int is_number(char *str)
{
    for (size_t i = 0; str[i] != '\0'; ++i)
    {
        if (str[i] < '0' || str[i] > '9')
            return 0;
    }
    return 1;
}

int builtin_continue(int argc, char *argv[], struct mem *mem)
{
    if (mem == NULL)
    {
        exit(EXIT_FAILURE);
    }

    int n = 1;
    if (argc == 2)
    {
        // check if the parameter is a number
        if (argc >= 2 && is_number(argv[1]) != 1)
        {
            // more than 1 parameter
            if (argc > 2)
            {
                fprintf(stderr, "42sh: invalid continue syntax error\n");
                _exit(1);
            }

            set_continue_number(INT_MAX);
            fprintf(stderr, "42sh: continue: not a number\n");
            _exit(1);
        }

        n = atoi(argv[1]);

        // if number of loop that need to be break is less than
        if (n < 1)
        {
            set_continue_number(INT_MAX);
            fprintf(stderr, "42sh: continue: < 0\n");
            return 1;
        }
    }

    set_continue_number(n);
    return 0;
}
