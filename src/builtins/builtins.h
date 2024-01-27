#ifndef BUILTINS_H
#define BUILTINS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../ast/ast.h"
#include "../execution/exec.h"
#include "../utils/token/token.h"

struct builtin_matchs
{
    char *name;
    int (*fun)(int, char **);
};

/*
 * @brief Returns 1 if `node` is a builtin word
 */
char is_builtin_word(char *word);

/*
 * @brief Returns the matching function of the builtin
 */
int (*builtin_fun(char *word))(int, char **);

// Builtins commands
/*
 * @brief True builtin, returns 0
 */
int builtin_true(int argc, char *argv[]);

/*
 * @brief False builtin, returns 1
 */
int builtin_false(int argc, char *argv[]);

/*
 * @brief Echo builtin
 */
int builtin_echo(int argc, char *argv[]);

/*
 * @brief exit builtin
 */
int builtin_exit(int argc, char *argv[]);

/*
 * @brief break builtin
 */
int builtin_break(int argc, char *argv[]);

/*
 * @brief break builtin
 */
int builtin_continue(int argc, char *argv[]);

#endif /* ! BUILTINS_H */
