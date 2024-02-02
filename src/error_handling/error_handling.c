#include "error_handling.h"

void free_all(struct to_be_freed *to_be_freed)
{
    ast_free(to_be_freed->ast);
    lexer_free(to_be_freed->lexer);
    // stream_free(to_be_freed->stream);
    mem_free(to_be_freed->mem);
    return;
}

void error(struct to_be_freed *to_be_freed, const char *str)
{
    free_all(to_be_freed);

    if (errno != 0)
    {
        fprintf(stderr, "%s (%s)", str, strerror(errno));
    }
    else
    {
        fprintf(stderr, "%s", str);
    }

    return;
}
