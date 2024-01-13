#include "opt_parser.h"

#include <getopt.h>
#include <stdio.h>

int parse_options(int argc, char *argv[], struct options *options)
{
    int status = 0;
    int opt;

    opterr = 0; // Disable getopt error messages

    // Parse positional arguments
    while ((opt = getopt(argc, argv, "acpv")) != -1)
    {
        switch (opt)
        {
        case 'a':
            options->ast_dot = 1;
            break;
        case 'c':
            options->command = 1;
            break;
        case 'p':
            options->pretty_print = 1;
            break;
        case 'v':
            options->verbose = 1;
            break;
        case '?':
            status = 2;
            fprintf(stderr, "42sh: -%c: invalid option\n", optopt);
            break;
        default:
            // Should never happen
            status = 2;
            break;
        }
    }

    // Skip '-' arguments
    while (argc > optind && argv[optind][0] == '-')
    {
        optind++;
    }

    // Put first positional argument in options->input
    options->input = argv[optind];

    return status;
}
