#ifndef LEXER_ERROR_H
#define LEXER_ERROR_H

#include "lexer.h"

char *get_lexer_error_msg(enum LEXER_ERROR error);

void set_lexer_last_error(struct lexer *lexer, struct ctx_info *ctx);

#endif /* ! LEXER_ERROR_H */
