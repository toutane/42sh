#ifndef TOKEN_RECOGNITION_H
#define TOKEN_RECOGNITION_H

#include "lexer/lexer.h"
#include "lexer/token_recognition_helpers.h"

void recognize_token(struct lexer *lexer, enum QUOTING_CONTEXT *quoting_ctx);

void handle_escape_quote(struct lexer *lexer,
                         enum QUOTING_CONTEXT *quoting_ctx);

void handle_single_quote(struct lexer *lexer,
                         enum QUOTING_CONTEXT *quoting_ctx);

void handle_double_quote(struct lexer *lexer,
                         enum QUOTING_CONTEXT *quoting_ctx);

#endif /* ! TOKEN_RECOGNITION_H */
