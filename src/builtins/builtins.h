#ifndef BUILTINS_H
#define BUILTINS_H

#include <stdio.h>
#include <string.h>

#include "ast/ast.h"
#include "utils/token/token.h"

struct builtin_matchs
{
    char *name;
    int (*fun)(int, char **);
};

/*
 * @brief True builtin, returns 0
 */
int builtin_true(int argc, char *argv[]);

/*
 * @brief False builtin, returns 1
 */
int builtin_false(int argc, char *argv[]);

/*
 * @brief TODO: Echo builtin
 */
int builtin_echo(int argc, char *argv[]);

/*
 * @brief Returns 1 if `node` is a builtin word
 */
char is_builtin_word(char *word);

/*
 * @brief Returns the matching function of the builtin
 */
int (*builtin_fun(char *word))(int, char **);

#endif /* ! BUILTINS_H */
