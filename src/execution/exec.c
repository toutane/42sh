#include "exec.h"

typedef int (*eval_type)(struct ast *ast, struct hash_map *gv_hash_map);

int eval_ast(struct ast *ast, struct hash_map *gv_hash_map)
{
    if (!ast)
    {
        return 0;
    }

    static const eval_type functions[] = {
        [AST_SIMPLE_COMMAND] = &eval_simple_command,
        [AST_COMMAND_LIST] = &eval_list,
        [AST_CONDITION] = &eval_condition,
        [AST_PIPELINE] = &eval_pipeline,
        [AST_REDIRECTION] = &eval_redirection,
        [AST_NEG] = &eval_neg,
    };

    return (*functions[ast->type])(ast, gv_hash_map);
}

int execution_loop(struct options *opts, struct stream_info *stream,
                   struct hash_map *gv_hash_map)
{
    struct to_be_freed to_be_freed = {
        .ast = NULL, .lexer = NULL, .stream = stream, .gv_hash_map = gv_hash_map
    };

    int status = 0;

    // Create the lexer
    struct lexer *lexer = lexer_new(stream, opts);
    to_be_freed.lexer = lexer;

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
        struct ast *ast = NULL;

        if (parse(&ast, lexer) != PARSER_OK)
        {
            to_be_freed.ast = ast;
            error(&to_be_freed, "42sh: grammar error during parsing\n");
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
                to_be_freed.ast = ast;
                free_all(&to_be_freed);
                return EXIT_FAILURE;
            }
        }

        // Evaluate the AST
        status = eval_ast(ast, gv_hash_map);

        // hash_map_dump(gv_hash_map);

        // Free the AST
        ast_free(ast);

        // Pop the current token
        lexer_pop(lexer);
    }

    free_all(&to_be_freed);

    return status;
}
