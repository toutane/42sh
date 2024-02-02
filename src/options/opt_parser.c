#include "opt_parser.h"

#include <getopt.h>
#include <stdio.h>

#include "../utils/printers/printers.h"

/*
 * Print usage message
 */
static void print_usage(void)
{
    printf("Usage: 42sh [options] [script] [arguments...]\n");
    printf("Options:\n");
    printf("  -a: Output AST in dot format\n");
    printf("  -c: Execute input as a command\n");
    printf("  -p: Pretty print AST\n");
    printf("  -v: Verbose mode\n");
}

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

        if (status != 0)
        {
            print_usage();
            return status;
        }
    }

    // Skip '-' arguments
    while (argc > optind && argv[optind][0] == '-')
    {
        optind++;
    }

    // Put first positional argument in options->input
    options->input = argv[optind];

    if (options->verbose)
    {
        options_print(options);
    }
    return status;
}
