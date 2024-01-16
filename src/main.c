#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>

#include "ast/ast.h"
#include "error_handling/error_handling.h"
#include "io/io.h"
#include "lexer/lexer.h"
#include "options/opt_parser.h"
#include "parser/parser.h"
#include "utils/dot_file/dot_file.h"
#include "utils/printers/printers.h"
#include "utils/token/token.h"

int main(int argc, char *argv[])
{
    int status; // Gather error status, passed to functions

    // Options structure, filled by parse_options
    struct options options = {
        .ast_dot = 0, .command = 0, .pretty_print = 0, .verbose = 0
    };

    // Parse command line options
    status = parse_options(argc, argv, &options);
    if (status != 0)
    {
        return status;
    }

    // Get input stream according to options
    struct stream_info *stream = get_stream(argc, &options, &status);
    if (stream == NULL)
    {
        return status != 0 ? status
                           : EXIT_SUCCESS; // If the input string is NULL, the
                                           // program should exit with success
    }

    struct lexer *lexer = lexer_new(stream, &options);
    struct ast *ast = NULL;

    if (parse(&ast, lexer) != PARSER_OK)
    {
        error(ast, lexer, stream, "42sh: grammar error during parsing\n");
        return GRAMMAR_ERROR;
    }

    if (options.pretty_print)
    {
        ast_pretty_print(ast);
    }

    if (options.ast_dot)
    {
        status = create_dot_file(ast, "ast.dot");
        if (status != 0)
        {
            free_all(ast, lexer, stream);
            return EXIT_FAILURE;
        }
    }

    status = ast_eval(ast);

    free_all(ast, lexer, stream);

    return status;
}
