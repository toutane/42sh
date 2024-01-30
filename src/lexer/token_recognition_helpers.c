#include "lexer/token_recognition_helpers.h"

#include <ctype.h>

void fill_token(struct token *tok, enum token_type type, char *value)
{
    if (tok != NULL)
    {
        tok->type = type;
        tok->value = value;
    }
}

/* Append the given character to current token value and
 * consume it. */
void append_consume(struct lexer *lexer, char c)
{
    append_char_to_token_value(&lexer->next_tok, c);
    stream_pop(lexer->stream);
}

/* Set the current token type to the given type and append
 * the given character its value. */
void set_append_consume(struct lexer *lexer, enum token_type type, char c)
{
    fill_token(&lexer->next_tok, type, NULL);
    append_consume(lexer, c);
}

int is_delimiter(char c)
{
    return isblank(c) || c == '\n' || c == ';';
}
