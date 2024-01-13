#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "../ast/ast.h"
#include "../io_backend/io_backend.h"
#include "../lexer/lexer.h"
#include "../lexer/token.h"
#include "../options/opt_parser.h"
#include "../parser/parser.h"

#define GRAMMAR_ERROR 2;

void free_all(struct ast *ast, struct lexer *lexer, struct stream_info *stream);
void error(struct ast *ast, struct lexer *lexer, struct stream_info *stream,
           const char *str);

#endif /* ! ERROR_HANDLING_H */
