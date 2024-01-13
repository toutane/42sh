#include "error_handling.h"

void free_all(struct ast *ast, struct lexer *lexer, struct stream_info *stream)
{
    ast_free(ast);
    lexer_free(lexer);
    stream_free(stream);
    return;
}

void error(struct ast *ast, struct lexer *lexer, struct stream_info *stream,
           const char *str)
{
    free_all(ast, lexer, stream);

    fprintf(stderr, "%s", str);

    return;
}
