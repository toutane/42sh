#define _XOPEN_SOURCE 500

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "exec.h"
#include "utils/memory/memory.h"
#include "utils/strings/strings.h"

int eval_for(struct ast *ast, struct hash_map *gv_hash_map)
{
    int status = 0;
    struct ast_for *ast_for = (struct ast_for *)ast;
    if (!ast_for)
        return 0;

    // check error
    if (!is_name(ast_for->condition))
    {
        fprintf(stderr, "42sh: %s: not a valid identifier\n",
                ast_for->condition);
        return 1;
    }

    // expand the array of value
    int expanded_array_size = ast_for->array_size;
    char **expanded_array =
        argv_expansions(ast_for->array, &expanded_array_size, gv_hash_map);

    // eval the ast loop
    for (int i = 0; i < expanded_array_size; ++i)
    {
        // set condition in memory
        char **value_array = calloc(2, sizeof(char *));
        value_array[0] = strdup(expanded_array[i]);
        memory_set(gv_hash_map, strdup(ast_for->condition), value_array);

        // evaluation of for loop ast
        status = eval_ast(ast_for->data, gv_hash_map);
    }

    // free expanded array
    for (int j = 0; j < expanded_array_size; j++)
    {
        free(expanded_array[j]);
    }
    free(expanded_array);

    return status;
}
