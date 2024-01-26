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

int builtin_continue(int argc, char *argv[])
{
    // more than 1 parameter
    if (argc > 2)
    {
        fprintf(stderr, "42sh: invalid continue syntax error\n");
        return 2;
    }

    int n = 1;
    if (argc == 2)
    {
        // check if the parameter is a number
        if (is_number(argv[1]) != 1)
        {
            fprintf(stderr, "42sh: continue: not a number\n");
            return 1;
        }

        n = atoi(argv[1]);

        // if number of loop that need to be continue is less than
        if (n < 1)
        {
            fprintf(stderr, "42sh: continue: < 0\n");
            return 1;
        }
    }

    set_continue_number(n);
    return 0;
}
