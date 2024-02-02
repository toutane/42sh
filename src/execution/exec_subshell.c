#include "exec.h"

int eval_subshell(struct ast *ast, struct mem *mem)
{
    struct ast_subshell *ast_subshell = (struct ast_subshell *)ast;

    pid_t pid = fork();
    if (pid == -1)
    {
        fprintf(stderr, "42sh: (: Failed to fork for subshell\n");
        _exit(127);
    }

    int ret_val = 0;
    if (pid == 0)
    {
        _exit(eval_ast(ast_subshell->compound_list, mem));
    }
    else
    {
        waitpid(pid, &ret_val, 0);
    }

    return WEXITSTATUS(ret_val);
}
