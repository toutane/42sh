#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "ast/ast.h"
#include "io/io.h"
#include "lexer/lexer.h"
#include "utils/token/token.h"
#include "options/opt_parser.h"
#include "parser/parser.h"

#define GRAMMAR_ERROR 2;

/*
 * @brief Prints the error message and exits the program.
 */
void free_all(struct ast *ast, struct lexer *lexer, struct stream_info *stream);

/*
 * @brief Prints the error message and exits the program.
 */
void error(struct ast *ast, struct lexer *lexer, struct stream_info *stream,
           const char *str);

#endif /* ! ERROR_HANDLING_H */
