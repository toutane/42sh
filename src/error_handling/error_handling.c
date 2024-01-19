#include "error_handling.h"

void free_all(struct to_be_freed *to_be_freed)
{
    ast_free(to_be_freed->ast);
    lexer_free(to_be_freed->lexer);
    stream_free(to_be_freed->stream);
    hash_map_free(to_be_freed->gv_hash_map);
    return;
}

void error(struct to_be_freed *to_be_freed, const char *str)
{
    free_all(to_be_freed);

    fprintf(stderr, "%s (%s)", str, strerror(errno));

    return;
}
