#include "exec.h"

typedef int (*eval_type)(struct ast *ast);

int eval_ast(struct ast *ast)
{
    if (ast == NULL)
    {
        return 0;
    }

    static const eval_type functions[] = {
        [AST_SIMPLE_COMMAND] = &eval_simple_command,
        [AST_COMMAND_LIST] = &eval_list,
        [AST_CONDITION] = &eval_condition,
    };

    return (*functions[ast->type])(ast);
}

int execution_loop(struct options *opts, struct stream_info *stream)
{
    int status = 0;

    // Create the lexer
    struct lexer *lexer = lexer_new(stream, opts);

    struct ast *ast = NULL;

    while (lexer_peek(lexer).type != TOKEN_EOF)
    {
        /* !! FOR DEBUGGING PURPOSES !!
         * This is a simple loop that prints all the tokens in the stream
         * and then exits the program. It helps to verify the token recognition
         * algorithm on tokens that are not implemented by the parser yet */
        /*
        struct token tok = lexer_pop(lexer);
        printf("%s: %s\n", token_type_to_str(tok.type), tok.value);
        while (tok.type != TOKEN_EOF)
        {
            tok = lexer_pop(lexer);
            printf("%s: %s\n", token_type_to_str(tok.type), tok.value);
        }
        */
        /* !! FOR DEBUGGING PURPOSES !! */

        // Create a new AST
        if (parse(&ast, lexer) != PARSER_OK)
        {
            error(ast, lexer, stream, "42sh: grammar error during parsing\n");
            return GRAMMAR_ERROR;
        }

        // Print the AST if pretty_print option is enabled
        if (opts->pretty_print)
        {
            ast_pretty_print(ast);
        }

        // Print the AST int dot if ast_dot option is enabled
        if (opts->ast_dot)
        {
            status = create_dot_file(ast, "ast.dot");
            if (status != 0)
            {
                free_all(ast, lexer, stream);
                return EXIT_FAILURE;
            }
        }

        // Evaluate the AST
        status = eval_ast(ast);
        /*
        if (status != 0)
        {
            free_all(ast, lexer, stream);
            return status;
        }
        */

        // Free the AST
        ast_free(ast);
        ast = NULL;

        // Pop the current token
        lexer_pop(lexer);
    }

    ast = NULL;

    // Free the lexer, the AST and the stream, AST is NULL so it won't be freed
    free_all(ast, lexer, stream);

    return status;
}
