#define _XOPEN_SOURCE 500

#include <ctype.h>
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

static char **args_to_list(struct ast_for *ast_for, size_t *items_amount)
{
    char **items_list = calloc(1, sizeof(char *));

    for (size_t i = 0; i < ast_for->array_size; i++)
    {
        items_list = realloc(items_list, (*items_amount + 1) * sizeof(char *));
        items_list[*items_amount] = NULL;
        size_t k = 0;

        char c = ast_for->array[i][k];
        while (c != '\0')
        {
            while (1 && c != '\0')
            {
                if (isblank(c))
                {
                    while (isblank(c))
                    {
                        k++;
                        c = ast_for->array[i][k];
                    }
                    break;
                }

                append_char_to_string(&(items_list[*items_amount]), c);
                k++;
                c = ast_for->array[i][k];
            }

            (*items_amount)++;
            if (c != '\0')
            {
                items_list =
                    realloc(items_list, (*items_amount + 1) * sizeof(char *));
                items_list[*items_amount] = NULL;
            }
        }
    }

    return items_list;
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

    size_t items_amount = 0;
    char **items_list = args_to_list(ast_for, &items_amount);

    for (size_t i = 0; i < items_amount; ++i)
    {
        char **value_array = calloc(2, sizeof(char *));
        value_array[0] = strdup(items_list[i]);

        memory_set(gv_hash_map, strdup(ast_for->condition), value_array);

        status = eval_ast(ast_for->data, gv_hash_map);
    }

    for (size_t j = 0; j < items_amount; j++)
    {
        free(items_list[j]);
    }

    free(items_list);
    return status;
}
