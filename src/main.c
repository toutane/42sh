#include "execution/exec.h"
#include "io/io.h"
#include "options/opt_parser.h"
#include "utils/memory/memory.h"
#include "utils/variables/variables.h"

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

    // Create shell memory hashmap
    struct hash_map *memory = memory_new();
    set_default_variables(argc, argv, memory);

    // Launch execution loop
    return execution_loop(&opts, stream, memory);
}
