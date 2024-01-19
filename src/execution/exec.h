#ifndef EXEC_H
#define EXEC_H

#include <fcntl.h>
#include <stdio.h>

#include "../ast/ast.h"
#include "../builtins/builtins.h"
#include "../error_handling/error_handling.h"
#include "../io/io.h"
#include "../lexer/lexer.h"
#include "../options/opt_parser.h"
#include "../parser/parser.h"
#include "../utils/dot_file/dot_file.h"
#include "../utils/printers/printers.h"
#include "../utils/token/token.h"

/*
 * @brief: Executes the program in a loop, reading from the input stream while
 * eof is not reached. It creates a AST while reading the input stream and
 * executes it. It returns 0 if the execution was successful, an error code
 * otherwise.
 */
int execution_loop(struct options *opts, struct stream_info *stream);

int eval_ast(struct ast *ast);

int eval_simple_command(struct ast *ast);

int eval_condition(struct ast *ast);

int eval_list(struct ast *ast);

int eval_pipeline(struct ast *ast);

int eval_redirection(struct ast *ast);

int eval_neg(struct ast *ast);

#endif /* ! EXEC_H */