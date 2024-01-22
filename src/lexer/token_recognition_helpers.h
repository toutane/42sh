#ifndef TOKEN_RECOGNITION_HELPERS_H
#define TOKEN_RECOGNITION_HELPERS_H

#include "lexer/lexer.h"

void fill_token(struct token *tok, enum token_type type, char *value);

void append_consume(struct lexer *lexer, char c);

void set_append_consume(struct lexer *lexer, enum token_type type, char c);

int is_delimiter(char c);

#endif /* ! TOKEN_RECOGNITION_HELPERS_H */
