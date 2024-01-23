#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <stdlib.h>

#include "exec.h"
#include "utils/memory/memory.h"
#include "utils/strings/strings.h"

static void arguments_expansion(struct ast_for *ast_for,
                                struct hash_map *memory)
{
    /* Expand the quoting of arguments, this contains backslash, double quote,
     * single quote and parameter ('$' and '${' ) expansion. For the parameter
     * expansion, the algorithm will search into the global variables hash_map
     * and properly expand variable identifiers. */

    for (size_t i = 0; i < ast_for->array_size; ++i)
    {
        char *to_be_freed = ast_for->array[i];
        ast_for->array[i] = expand_string(&(ast_for->array[i]), memory);
        free(to_be_freed);
    }
}

int eval_for(struct ast *ast, struct hash_map *gv_hash_map)
{
    int status = 0;
    struct ast_for *ast_for = (struct ast_for *)ast;
    if (!ast_for)
        return 0;

    if (!is_name(ast_for->condition))
    {
        fprintf(stderr, "42sh: %s: not a valid identifier\n",
                ast_for->condition);
        return 1;
    }

    arguments_expansion(ast_for, gv_hash_map);

    for (size_t i = 0; i < ast_for->array_size; ++i)
    {
        char **value_array = calloc(2, sizeof(char *));
        value_array[0] = strdup(ast_for->array[i]);

        memory_set(gv_hash_map, strdup(ast_for->condition), value_array);

        status = eval_ast(ast_for->data, gv_hash_map);
    }

    return status;
}
