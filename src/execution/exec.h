#ifndef EXEC_H
#define EXEC_H

#include "../ast/ast.h"
#include "../error_handling/error_handling.h"
#include "../io/io.h"
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

#endif /* ! EXEC_H */
