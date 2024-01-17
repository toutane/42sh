#include <stdlib.h>

#include "lexer.h"

struct lexer *lexer_new(struct stream_info *stream, struct options *opts)
{
    struct lexer *lexer = calloc(1, sizeof(struct lexer));
    if (lexer == NULL)
    {
        fprintf(stderr, "lexer_new: calloc failed\n");
        return NULL;
    }

    lexer->opts = opts;
    lexer->stream = stream;
    lexer->cur_tok.type = TOKEN_NONE;
    lexer->cur_tok.value = NULL;
    lexer->must_parse_next_tok = 1;

    if (lexer->opts->verbose)
    {
        printf("[LEXER] Lexer created\n");
    }

    return lexer;
}

void lexer_free(struct lexer *lexer)
{
    if (lexer == NULL)
    {
        printf("lexer_free: lexer is NULL\n");
        return;
    }

    if (lexer->cur_tok.value != NULL)
    {
        free(lexer->cur_tok.value); // Free the current token value if any
    }

    int is_verbose = lexer->opts->verbose;

    free(lexer);

    if (is_verbose)
    {
        printf("[LEXER] Lexer freed\n");
    }
}
