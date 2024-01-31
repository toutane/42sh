#ifndef BUILTINS_H
#define BUILTINS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast/ast.h"
#include "execution/exec.h"
#include "utils/assignment_words/assignment_words.h"
#include "utils/token/token.h"
#include "utils/variables/variables.h"

struct builtin_matchs
{
    char *name;
    int (*fun)(int, char **, struct mem *mem);
};

/*
 * @brief Returns 1 if `node` is a builtin word
 */
char is_builtin_word(char *word);

/*
 * @brief Returns the matching function of the builtin
 */
int (*builtin_fun(char *word))(int, char **, struct mem *mem);

// Builtins commands
/*
 * @brief True builtin, returns 0
 */
int builtin_true(int argc, char *argv[], struct mem *mem);

/*
 * @brief False builtin, returns 1
 */
int builtin_false(int argc, char *argv[], struct mem *mem);

/*
 * @brief Echo builtin
 */
int builtin_echo(int argc, char *argv[], struct mem *mem);

/*
 * @brief exit builtin
 */
int builtin_exit(int argc, char *argv[], struct mem *mem);

/*
 * @brief break builtin
 */
int builtin_break(int argc, char *argv[], struct mem *mem);

/*
 * @brief break builtin
 */
int builtin_continue(int argc, char *argv[], struct mem *mem);

/* @brief export builtin */
int builtin_export(int argc, char *argv[], struct mem *mem);

/* @brief . builtin */
int builtin_dot(int argc, char *argv[], struct mem *mem);

/* @brief unset builtin */
int builtin_unset(int argc, char *argv[], struct mem *mem);

/* @brief cd builtin  */
int builtin_cd(int argc, char *argv[], struct mem *mem);

/* @brief alias builtin  */
int builtin_alias(int argc, char *argv[], struct mem *mem);

/*
 * @brief . builtin
 */
int builtin_dot(int argc, char *argv[], struct mem *mem);

#endif /* ! BUILTINS_H */
