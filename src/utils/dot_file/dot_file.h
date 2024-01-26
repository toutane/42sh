#ifndef DOT_FILE_H
#define DOT_FILE_H

#include <stdio.h>

#include "../../ast/ast.h"

/*
 * @brief Create a dot file from an ast
 */
int create_dot_file(struct ast *ast, char *filename);

#endif /* ! DOT_FILE_H */
