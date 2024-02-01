#include <stdlib.h>

#include "lexer.h"

struct lexer *lexer_new(struct stream_info *stream, struct options *opts,
                        struct hm *hm_alias)
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

    lexer->next_tok.type = TOKEN_NONE;
    lexer->next_tok.value = NULL;

    lexer->must_parse_next_tok = 1;
    lexer->last_error = NO_ERROR;

    lexer->hm_alias = hm_alias;

    // init stack and push default stream
    lexer->stream_stack = stack_new(stream_free);
    stack_push(lexer->stream_stack, stream);

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

    if (lexer->next_tok.value != NULL)
    {
        free(lexer->next_tok.value); // Free the next token value if any
    }

    stack_free(lexer->stream_stack);

    int is_verbose = lexer->opts->verbose;

    free(lexer);

    if (is_verbose)
    {
        printf("[LEXER] Lexer freed\n");
    }
}
