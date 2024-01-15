#ifndef AST_TO_DOT_H
#define AST_TO_DOT_H

#include <stdio.h>

#include "ast/ast.h"

/*
 * @brief Create a dot file from an ast
 */
int create_dot_file(struct ast *ast, char *filename);

#endif /* ! AST_TO_DOT_H */
