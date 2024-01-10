#include "opt_parser.h"

#include <getopt.h>
#include <stdio.h>

void parse_options(int argc, char *argv[], struct options *options)
{
    int opt;
    int option_index = 0;

    // Define long options structure
    static struct option long_options[] = { { "verbose", no_argument, 0, 'v' },
                                            { "pretty-print", no_argument, 0,
                                              'p' },
                                            { 0, 0, 0, 0 } };

    while ((opt = getopt_long(argc, argv, "c", long_options, &option_index))
           != -1)
    {
        switch (opt)
        {
        case 'c':
            options->command = 1;
            break;
        case 'v':
            options->verbose = 1;
            break;
        case 'p':
            options->pretty_print = 1;
            break;
        default:
            break;
        }
    }
}
