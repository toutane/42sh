#include <stdlib.h>

#include "error_handling/error_handling.h"
#include "execution/exec.h"
#include "expansion.h"
#include "lexer/lexer.h"

static void handle_escape_quote(struct stream_info *stream,
                                enum QUOTING_CONTEXT *quoting_ctx, char **str,
                                int is_backquoted)
{
    // Consume the <blackslash> to get the next character
    stream_pop(stream);

    char next_char = stream_peek(stream);

    // If a <newline> follows the <backslash>, the shell shall interpret
    // this as line continuation.
    if (next_char == '\n' && *quoting_ctx != SINGLE_QUOTE)
    {
        stream_pop(stream);
        return;
    }

    if (*quoting_ctx == SINGLE_QUOTE)
    {
        append_char_to_string(str, '\\');
        return;
    }

    // We append the <backslash> and the next character to the current token
    if (!is_backquoted)
    {
        append_char_to_string(str, '\\');
    }
    append_char_to_string(str, next_char);
    stream_pop(stream);
}

static void handle_single_quote(struct stream_info *stream,
                                enum QUOTING_CONTEXT *quoting_ctx, char **str)
{
    if (*quoting_ctx == NONE)
    {
        *quoting_ctx = SINGLE_QUOTE;

        append_char_to_string(str, '\'');
        stream_pop(stream);
    }
    else
    {
        if (*quoting_ctx != DOUBLE_QUOTE)
        {
            *quoting_ctx = NONE;
        }
        append_char_to_string(str, '\'');
        stream_pop(stream);
    }

    return;
}

static void handle_double_quote(struct stream_info *stream,
                                enum QUOTING_CONTEXT *quoting_ctx, char **str)
{
    if (*quoting_ctx == NONE)
    {
        *quoting_ctx = DOUBLE_QUOTE;

        append_char_to_string(str, '"');
        stream_pop(stream);
    }
    else
    {
        if (*quoting_ctx != SINGLE_QUOTE)
        {
            *quoting_ctx = NONE;
        }
        append_char_to_string(str, '"');
        stream_pop(stream);
    }

    return;
}

static int modify_quoting_context(struct stream_info *stream,
                                  enum QUOTING_CONTEXT *ctx, char **str,
                                  int is_backquoted)
{
    char cur_char = stream_peek(stream);

    switch (cur_char)
    {
    case '\\':
        handle_escape_quote(stream, ctx, str, is_backquoted);
        return 1;
    case '\'':
        handle_single_quote(stream, ctx, str);
        return 1;
    case '"':
        handle_double_quote(stream, ctx, str);
        return 1;
    default:
        return 0;
    }
}

static void get_command_in_backquotes(char **command,
                                      struct stream_info *stream)
{
    enum QUOTING_CONTEXT ctx = NONE;

    char cur_char;
    while (1)
    {
        cur_char = stream_peek(stream);
        if (cur_char == EOF)
        {
            break;
        }

        if (modify_quoting_context(stream, &ctx, command, 1))
            continue;

        if (ctx == NONE && cur_char == '`')
        {
            // Delimit command
            stream_pop(stream);
            break;
        }

        append_char_to_string(command, cur_char);
        stream_pop(stream);
    }
}

static void get_command_in_paren(char **command, struct stream_info *stream)
{
    enum QUOTING_CONTEXT ctx = NONE;

    // We are already inside parenthesis
    int depth = 1;

    char cur_char;
    while (1)
    {
        cur_char = stream_peek(stream);
        if (cur_char == EOF)
        {
            break;
        }

        if (modify_quoting_context(stream, &ctx, command, 0))
            continue;

        if (ctx == NONE && depth == 1 && cur_char == ')')
        {
            // Delimit command
            stream_pop(stream);
            break;
        }

        if (ctx == NONE && cur_char == '(')
        {
            depth++;
        }

        if (ctx == NONE && cur_char == ')')
        {
            depth--;
        }

        append_char_to_string(command, cur_char);
        stream_pop(stream);
    }
}

static void get_string_command(char **command, struct stream_info *stream)
{
    /* In this step, we read until we find a closing brace. All the
     * characters that forming the expression are removed from the token_word
     * string. */

    char cur_char = stream_pop(stream);
    int is_backquoted = cur_char == '`';

    if (is_backquoted)
    {
        get_command_in_backquotes(command, stream);
    }
    else
    {
        get_command_in_paren(command, stream);
    }
}

static int execute_substitution(char *command, struct mem *mem)
{
    struct options opts = {
        .ast_dot = 0, .command = 0, .pretty_print = 0, .verbose = 0
    };

    int status = 0;

    struct stream_info *cmd_stream = stream_new(NULL, command, &status);

    struct to_be_freed to_be_freed = {
        .ast = NULL, .lexer = NULL, .stream = cmd_stream, .mem = NULL
    };

    // Create the lexer
    struct lexer *lexer = lexer_new(cmd_stream, &opts);
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
            error(&to_be_freed,
                  "42sh: grammar error during (in command substitution)\n");
            return err;
            ;
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

static void wait_and_replace(pid_t pid, char **str, int fds[2])
{
    // Close writing part
    close(fds[1]);

    // In the parent process, wait for the child to finish
    waitpid(pid, NULL, 0);

    // Read the output of the command
    while (1)
    {
        char next_char = '\0';
        if (read(fds[0], &next_char, 1) == 0)
        {
            break;
        }

        append_char_to_string(str, next_char);
    }

    // Remove the trailing newlines
    while (strlen(*str) != 0 && (*str)[strlen(*str) - 1] == '\n')
    {
        (*str)[strlen(*str) - 1] = '\0';
    }

    close(fds[0]);
}

void command_substitution(char **str, struct stream_info *stream,
                          struct mem *mem)
{
    char cur_char = stream_peek(stream);
    if (cur_char != '(' && cur_char != '`')
    {
        return;
    }

    // Gather the command in a string
    char *command = NULL;
    get_string_command(&command, stream);
    // printf("command: %s\n", command);

    // If the command is empty, we do nothing and return
    if (command == NULL || command[0] == '\0')
    {
        free(command);
        return;
    }

    // Create pipe
    int fds[2];
    if (pipe(fds) == -1)
    {
        fprintf(stderr,
                "42sh: failed to create pipe (in command substitution)\n");
        _exit(EXIT_FAILURE);
    }

    // Re-launch 42sh with the command in a subshell
    pid_t pid = fork();
    if (pid < 0)
    {
        error(NULL, "42sh: fork failed\n");
        return;
    }

    if (pid == 0)
    {
        // Close reading side of the pipe
        close(fds[0]);

        if (dup2(fds[1], STDOUT_FILENO) == -1)
        {
            fprintf(stderr,
                    "42sh: dup2 failed on writing (in command substitution)\n");
            _exit(EXIT_FAILURE);
        }

        int status = execute_substitution(command, mem);

        close(fds[1]);
        _exit(status);
    }
    else
    {
        wait_and_replace(pid, str, fds);
    }

    free(command);
    return;
}
