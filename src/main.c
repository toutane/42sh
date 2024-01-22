#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>

#include "execution/exec.h"
#include "expansion/special_variables.h"
#include "io/io.h"
#include "options/opt_parser.h"
#include "utils/memory/memory.h"

int main(int argc, char *argv[])
{
    int status; // Gather error status, passed to functions

    // Options structure, filled by parse_options
    struct options opts = {
        .ast_dot = 0, .command = 0, .pretty_print = 0, .verbose = 0
    };

    // Parse command line options
    status = parse_options(argc, argv, &opts);
    if (status != 0)
    {
        return status;
    }

    // Get input stream according to options
    struct stream_info *stream = get_stream(argc, &opts, &status);
    if (stream == NULL)
    {
        return status != 0 ? status
                           : EXIT_SUCCESS; // If the input string is NULL, the
                                           // program should exit with success
    }

    // Create global variables hash table
    struct hash_map *gv_hash_map = memory_new();
    fill_at_sign_var(argc, argv, gv_hash_map);
    fill_arguments_var(argc, argv, gv_hash_map);
    fill_star_sign_var(argc, argv, gv_hash_map);
    fill_dollar_var(gv_hash_map);
    fill_arguments_amount(argc, gv_hash_map);

    // Launch execution loop
    return execution_loop(&opts, stream, gv_hash_map);
}
