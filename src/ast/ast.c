#include "ast.h"

void ast_free(struct ast *ast)
{
    if (!ast)
    {
        return;
    }

    // Free argv
    for (size_t i = 0; i < ast->nb_args; ++i)
    {
        free(ast->argv[i]);
    }
    free(ast->argv);

    // recursively free childs
    for (size_t i = 0; i < ast->nb_child; ++i)
    {
        ast_free(ast->children[i]);
    }
    free(ast->children);
    free(ast);
}

int eval_sc_node(struct ast *ast)
{
    int status = 0;
    if (is_builtin_word(ast->argv[0]))
    {
        status = (builtin_fun(ast->argv[0]))(ast->nb_args - 1, ast->argv);
        fflush(stdout); // Flush stdout to avoid mixing output
        return status;
    }

    int pid = fork();
    if (pid == 0)
    {
        execvp(ast->argv[0], ast->argv);
        exit(execvp_error(errno));
    }
    else
    {
        waitpid(pid, &status, 0);
    }

    // check if the command was interrupted by a signal
    if (WIFSIGNALED(status))
    {
        fprintf(stderr, "42sh: command terminated because of a signal");
        return 129;
    }

    // otherwise we return the status error code
    return WEXITSTATUS(status);
}

int ast_eval(struct ast *ast)
{
    if (!ast)
    {
        return 0;
    }
    int status = 0;
    if (ast->type == AST_SIMPLE_COMMAND)
    {
        status = eval_sc_node(ast);
    }
    else if (ast->type == AST_COMMAND_LIST)
    {
        for (size_t i = 0; i < ast->nb_child; i++)
        {
            status = ast_eval(ast->children[i]);
        }
    }
    else if (ast->type == AST_CONDITION)
    {
        if (ast_eval(ast->children[0]) == EXIT_SUCCESS)
        {
            status = ast_eval(ast->children[1]);
        }
        else if (ast->children[2] != NULL)
        {
            status = ast_eval(ast->children[2]);
        }
    }
    return status;
}
