#include "builtins.h"
#include "execution/exec.h"

static char *path_search(char *arg)
{
    char *PATH = getenv("PATH");

    char *seek = PATH;
    char *next_path =
        (strchr(seek, ':') ? strchr(seek, ':') : strchr(seek, '\0'));
    char *built_path = NULL;
    while (next_path)
    {
        if (next_path != seek)
        {
            // Build the path : path + / +  arg
            size_t len = next_path - seek + strlen(arg) + 1;
            built_path = malloc(sizeof(char) * (len + 1));

            memcpy(built_path, seek, next_path - seek);

            built_path[next_path - seek] = '/';

            memcpy(built_path + (next_path - seek) + 1, arg, strlen(arg));
            built_path[len] = '\0';
        }
        else
        {
            // pathname is null
            // Build the path : . + / +  arg
            size_t len = strlen(arg) + 2;
            built_path = malloc(sizeof(char) * (len + 1));

            built_path[0] = '.';

            built_path[1] = '/';

            memcpy(built_path + 2, arg, strlen(arg));
            built_path[len] = '\0';
        }

        // Check if exists
        FILE *file = fopen(built_path, "r");
        if (file)
        {
            fclose(file);
            return built_path;
        }
        free(built_path);

        if (*next_path == '\0')
        {
            break;
        }
        seek = next_path + 1;
        next_path =
            (strchr(seek, ':') ? strchr(seek, ':') : strchr(seek, '\0'));
    }
    return NULL;
}

static struct stream_info *create_stream_from_file(char *file, int *status)
{
    struct stream_info *stream = NULL;

    char *first_elt = (strchr(file, '/') ? strchr(file, '/') : strchr(file, '\0'));

    // If path starts with ../ or ./
    if (((first_elt - file == 1) && !memcmp(file, ".", 1))
        || ((first_elt - file == 2) && !memcmp(file, "..", 2)))
    {
        stream = stream_new(file, NULL, status);
    }
    else if (file[0] == '/')
    {
        stream = stream_new(file, NULL, status);
    }
    else
    {
        char *path_concat = path_search(file);
        if (!path_concat)
        {
            fprintf(stderr, "42sh: .: File not found in PATH\n");
            _exit(1);
        }
        stream = stream_new(path_concat, NULL, status);
        free(path_concat);
    }
    return stream;
}

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
    struct stream_info *stream = create_stream_from_file(argv[1], &status);
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
