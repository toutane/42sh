#include "builtins.h"
#include "execution/exec.h"

int builtin_dot(int argc, char *argv[], struct mem *mem)
{
    if (argc != 2)
    {
        fprintf(stderr, "42sh: dot: Wrong number of arguments\n");
        return 1;
    }

    // Options structure, filled by parse_options
    struct options opts = {
        .ast_dot = 0, .command = 0, .pretty_print = 0, .verbose = 0
    };

    int status = 0;

    // Create stream from file
    struct stream_info *stream = stream_new(argv[1], NULL, &status);
    if (!stream)
    {
        _exit(1);
    }

    struct to_be_freed to_be_freed = {
        .ast = NULL, .lexer = NULL, .stream = stream, .mem = NULL
    };

    // Create the lexer
    struct lexer *lexer = lexer_new(stream, &opts);
    to_be_freed.lexer = lexer;

    while (lexer_peek(lexer).type != TOKEN_EOF)
    {
        set_break_number(0);
        set_continue_number(0);

        // Create a new AST
        struct ast *ast = NULL;

        if (parse(&ast, lexer) != PARSER_OK)
        {
            // If the lexer has encountered an bad substituion, 42sh shall exit
            // with 1
            int err = lexer->last_error == BAD_SUBSTITUTION ? 1 : GRAMMAR_ERROR;

            to_be_freed.ast = ast;
            error(&to_be_freed, "42sh: grammar error during parsing\n");
            return err;
        }

        // Evaluate the AST
        status = eval_ast(ast, mem);

        // Free the AST
        ast_free(ast);

        // Pop the current token
        lexer_pop(lexer);
    }

    free_all(&to_be_freed);

    return status;
}
