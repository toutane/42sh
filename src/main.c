#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>

#include "ast/ast.h"
#include "io_backend/io_backend.h"
#include "options/opt_parser.h"
#include "lexer/lexer.h"
#include "lexer/token.h"
#include "parser/parser.h"

int main(int argc, char *argv[])
{
    int status;

    // Parse command line options
    struct options options = { .command = 0, .pretty_print = 0, .verbose = 0 };
    status = parse_options(argc, argv, &options);
    if (status != 0)
    {
        return EXIT_FAILURE;
    }

    // Get input stream according to options
    struct stream_info *stream = get_stream(argc, argv, &options);
    if (stream == NULL)
    {
        return EXIT_FAILURE;
    }

    struct lexer *lexer = lexer_new(stream);
    struct ast *ast = NULL;

    if (parse(&ast, lexer) != PARSER_OK)
    {
        ast_free(ast);
        lexer_free(lexer);
        stream_free(stream);
        return EXIT_FAILURE;
    }

    ast_print(ast);
    ast_eval(ast);

    // Clean
    ast_free(ast);
    lexer_free(lexer);
    stream_free(stream);

    return EXIT_SUCCESS;
}
