#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>

#include "ast/ast.h"
#include "error_handling/error_handling.h"
#include "io_backend/io_backend.h"
#include "lexer/lexer.h"
#include "utils/token/token.h"
#include "options/opt_parser.h"
#include "parser/parser.h"
#include "utils/dot_file/dot_file.h"

int main(int argc, char *argv[])
{
    int status; // Gather error status, passed to functions

    // Parse command line options
    struct options options = {
        .ast_dot = 0, .command = 0, .pretty_print = 0, .verbose = 0
    };
    status = parse_options(argc, argv, &options);
    if (status != 0)
    {
        return status;
    }

    // get input stream according to options
    struct stream_info *stream = get_stream(argc, &options, &status);
    if (stream == NULL)
    {
        return status != 0 ? status
                           : EXIT_SUCCESS; // If the input string is NULL, the
                                           // program should exit with success
    }

    struct lexer *lexer = lexer_new(stream);
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

    // clean
    free_all(ast, lexer, stream);

    return status;
}
