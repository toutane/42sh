#include "error_handling_execvp.h"

// handling error when using execvp in ast
int execvp_error(int err)
{
    switch (err)
    {
    case EACCES:
        fprintf(stderr, "42sh: command is not an executable utility");
        return 126;
    default:
        fprintf(stderr, "42sh: command error");
        return 127;
    }
}
