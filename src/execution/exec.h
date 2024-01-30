#ifndef EXEC_H
#define EXEC_H

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#include "ast/ast.h"
#include "builtins/builtins.h"
#include "error_handling/error_handling.h"
#include "expansion/expansion.h"
#include "io/io.h"
#include "lexer/lexer.h"
#include "options/opt_parser.h"
#include "parser/parser.h"
#include "utils/assignment_words/assignment_words.h"
#include "utils/dot_file/dot_file.h"
#include "utils/hash_map/hash_map.h"
#include "utils/printers/printers.h"
#include "utils/token/token.h"
#include "utils/variables/variables.h"

/* @brief: Executes the program in a loop, reading from the input stream while
 * eof is not reached. It creates a AST while reading the input stream and
 * executes it. It returns 0 if the execution was successful, an error code
 * otherwise. */
int execution_loop(struct options *opts, struct stream_info *stream,
                   struct mem *mem);

int eval_ast(struct ast *ast, struct mem *mem);

int eval_simple_command(struct ast *ast, struct mem *mem);

int eval_condition(struct ast *ast, struct mem *mem);

int eval_list(struct ast *ast, struct mem *mem);

int eval_pipeline(struct ast *ast, struct mem *mem);

int eval_redirection(struct ast *ast, struct mem *mem);

int eval_neg(struct ast *ast, struct mem *mem);

int eval_for(struct ast *ast, struct mem *mem);

int eval_and(struct ast *ast, struct mem *mem);

int eval_or(struct ast *ast, struct mem *mem);

int eval_while(struct ast *ast, struct mem *mem);

int eval_until(struct ast *ast, struct mem *mem);

int eval_func(struct ast *ast, struct mem *mem);

void set_break_number(int new_val);

int get_break_number(void);

void set_continue_number(int new_val);

int get_continue_number(void);

#endif /* ! EXEC_H */
