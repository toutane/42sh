#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TODO: put in utils module
static int is_char_in_string(char c, char *str)
{
    int i = 0;
    while (str[i] != '\0')
    {
        if (str[i] == c)
        {
            return 1;
        }
        i++;
    }
    return 0;
}

/*
 * @brief: Check if argv is an option, and set the corresponding flag if it is
 */
static int parse_options(char *argv, char *optionString, int *nflag,
                         int *interpret)
{
    if (argv == NULL)
    {
        return 0;
    }

    int len = strlen(argv);
    int j = 0;

    if (argv[j] == '-')
    {
        // We check if all the characters in the argument are in the option
        // string We don't want '-npe' to be interpreted as '-n -e', but as a
        // non option argument (because 'p' is not in the option string)
        j++;
        while (argv[j] != '\0' && is_char_in_string(argv[j], optionString))
        {
            j++;
        }

        if (j == len)
        {
            for (int i = 1; i < len; i++)
            {
                char c = argv[i];
                switch (c)
                {
                case 'n':
                    *nflag = 1;
                    break;
                case 'e':
                    *interpret = 1;
                    break;
                case 'E':
                    *interpret = 0;
                    break;
                default:
                    // Should not happen
                    break;
                }
            }
        }
    }

    return j == 1 ? 0 : j == len;
}

/*
 * @brief: Interpret the string and print it
 */
void interpret_print(char *str)
{
    int len = strlen(str);
    for (int j = 0; j < len; j++)
    {
        if (str[j] == '\\' && j < len - 1)
        {
            switch (str[j + 1])
            {
            case 'n':
                printf("\n");
                break;
            case 't':
                printf("\t");
                break;
            case '\\':
                printf("\\");
                break;
            default:
                printf("\\%c", str[j + 1]);
                break;
            }
            j++;
        }
        else
        {
            printf("%c", str[j]);
        }
    }
}

/*
 * @brief: echo builtin command
 * Handle the following options:
 * -n: do not output the trailing newline
 * -e: enable interpretation of backslash escapes
 * -E: disable interpretation of backslash escapes (default)
 *  Return 0 on success, -1 on error
 */
int builtin_echo(int argc, char *argv[])
{
    int nflag = 0;
    int interpret = 0;

    char *optionString = "neE";

    int start_index = 1;
    while (parse_options(argv[start_index], optionString, &nflag, &interpret))
    {
        start_index++;
    }

    // Print the non-oprion arguments
    for (int i = start_index; i < argc; i++)
    {
        // Print the non-option argument
        if (interpret)
        {
            interpret_print(argv[i]);
        }
        else
        {
            printf("%s", argv[i]);
        }

        // Print a space between arguments
        if (i < argc - 1)
        {
            printf(" ");
        }
    }

    if (!nflag)
    {
        printf("\n");
    }

    return 0;
}