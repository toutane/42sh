#define _XOPEN_SOURCE 500

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "exec.h"
#include "utils/memory/memory.h"
#include "utils/strings/strings.h"
#include "utils/variables/variables.h"

int eval_for(struct ast *ast, struct mem *mem)
{
    int status = 0;
    struct ast_for *ast_for = (struct ast_for *)ast;
    if (!ast_for)
        return 0;

    // Ccheck error
    if (!is_name(ast_for->condition))
    {
        fprintf(stderr, "42sh: %s: not a valid identifier\n",
                ast_for->condition);
        return 1;
    }

    /* Expand the array of value. This correspond to the list of value
     * that will be assigned to the variable in the for loop,
     * ex: for i in 1 2 3 4 5, the array will be [1, 2, 3, 4, 5]. */
    int expanded_array_size = ast_for->array_size;
    char **expanded_array =
        argv_expansions(ast_for->array, &expanded_array_size, mem->hm_var);

    // Eval the ast loop
    for (int i = 0; i < expanded_array_size; ++i)
    {
        /* Assign variable for loop, the variable is stored in the internal
         * variable hashmap. */
        assign_variable(ast_for->condition, expanded_array[i], mem->hm_var);

        // Evaluation of for loop ast
        status = eval_ast(ast_for->data, mem);
    }

    // Free expanded array
    for (int j = 0; j < expanded_array_size; j++)
    {
        free(expanded_array[j]);
    }
    free(expanded_array);

    return status;
}
