#include "exec.h"

int eval_pipeline(struct ast *ast, struct hash_map *gv_hash_map)
{
    struct ast_pipeline *ast_pipeline = (struct ast_pipeline *)ast;
    int fds[2];
    if (pipe(fds) == -1)
    {
        fprintf(stderr, "42sh: failed to create pipe\n");
        // TODO: check if the error code is 127
        return 127;
    }

    pid_t pid_1 = fork();
    if (pid_1 < 0)
    {
        fprintf(stderr, "42sh: failed to fork\n");
        // TODO: check if the error code is 127
        return 127;
    }
    // child process
    if (pid_1 == 0)
    {
        // close reading side
        close(fds[0]);

        if (dup2(fds[1], STDOUT_FILENO) == -1)
        {
            fprintf(stderr, "42sh: dup2 failed on writing\n");
            _exit(127);
        }

        int res = eval_ast(ast_pipeline->left, gv_hash_map);
        close(fds[1]);
        _exit(res);
    }
    // parent process
    else
    {
        // close writing part
        close(fds[1]);

        // return value of fork 2
        int status = 0;

        pid_t pid_2 = fork();
        if (pid_2 < 0)
        {
            fprintf(stderr, "42sh: failed to fork 2\n");
            // TODO: check if the error code is 127
            return 127;
        }
        // child part 2
        if (pid_2 == 0)
        {
            if (dup2(fds[0], STDIN_FILENO) == -1)
            {
                fprintf(stderr, "42sh: dup2 failed on reading\n");
                _exit(127);
            }

            int res = eval_ast(ast_pipeline->right, gv_hash_map);
            close(fds[0]);
            _exit(res);
        }
        // parent part 2
        else
        {
            waitpid(pid_1, NULL, 0);
            waitpid(pid_2, &status, 0);
        }

        return WEXITSTATUS(status);
    }
}
