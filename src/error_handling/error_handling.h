#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "../ast/ast.h"
#include "../io/io.h"
#include "../lexer/lexer.h"
#include "../options/opt_parser.h"
#include "../parser/parser.h"
#include "../utils/hash_map/hash_map.h"
#include "../utils/token/token.h"
#include "lexer/lexer_setup.h"

#define GRAMMAR_ERROR 2;

struct to_be_freed
{
    struct ast *ast;
    struct hash_map *gv_hash_map;
    struct lexer *lexer;
    struct stream_info *stream;
};

/*
 * @brief Prints the error message and exits the program.
 */
void free_all(struct to_be_freed *to_be_freed);

/*
 * @brief Prints the error message and exits the program.
 */
void error(struct to_be_freed *to_be_freed, const char *str);

#endif /* ! ERROR_HANDLING_H */
