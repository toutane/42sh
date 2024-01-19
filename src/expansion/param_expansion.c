#include "param_expansion.h"

#include <stdlib.h>
#include <string.h>

// Return the expression to replace, allocated on the heap.
static char *get_expression(char *str, size_t *start, size_t *end)
{
    char *res = NULL;

    size_t original_len = strlen(str);
    str = strchr(str, '$');

    if (str == NULL)
    {
        return NULL;
    }

    // Set the index of the beginning of the expression
    *start = original_len - strlen(str);

    str++; // Skip the '$'

    size_t len = strlen(str);
    if (len < 1)
    {
        return NULL;
    }

    if (str[0] == '{')
    {
        // Skip the '{'
        str++;

        // The expression is between braces
        size_t i = 0;
        while (str[i] != '\0' && str[i] != '}')
        {
            i++;
        }

        // Set the index of the end of the expression
        *end = i + *start + 2;

        // Allocate the expression
        res = calloc(i + 1, sizeof(char));
        strncpy(res, str, i);
    }
    else
    {
        // The expression is the longest valid name
        size_t i = 0;
        while (str[i] != '\0' && is_valid_name_char(str[i]))
        {
            i++;
        }

        // Set the index of the end of the expression
        *end = i + *start + 1;

        // Allocate the expression
        res = calloc(i + 1, sizeof(char));
        strncpy(res, str, i);
    }

    return res;
}

// Replace the expression by its value in the string.
static void replace_parameter(char **original_str, size_t start, size_t end,
                              const char *replacement)
{
    if (original_str == NULL || *original_str == NULL || replacement == NULL)
    {
        return;
    }

    size_t original_len = strlen(*original_str);
    size_t replacement_len = strlen(replacement);

    size_t new_len = original_len - (end - start) + replacement_len;

    char *new_str = calloc(new_len + 1, sizeof(char));

    strncpy(new_str, *original_str, start);
    strcat(new_str, replacement);
    strcat(new_str, *original_str + end);

    free(*original_str);
    *original_str = new_str;
}

void expand_parameter(char **str, struct hash_map *gv_hash_map)
{
    if (*str == NULL || strchr(*str, '$') == NULL || gv_hash_map == NULL)
    {
        return;
    }

    /* Retrieve the expression to replace.
     * If the parameter introduction sequence is '$', the expression is the
     * longest valid name, whether or not the variable represented by the name
     * exists. If the introduction sequence is '${', the expression is the
     * string which is between braces, in this case the expression is consider
     * always valid, indeed the bad substituion check is done before, by the
     * lexer.
     */
    size_t exp_start_idx = 0;
    size_t exp_end_idx = 0;
    char *expression = get_expression(*str, &exp_start_idx, &exp_end_idx);

    if (expression == NULL)
    {
        return;
    }

    // Retrieve the value of the expression from hash map if it exists
    const char *value = hash_map_get(gv_hash_map, expression);

    replace_parameter(str, exp_start_idx, exp_end_idx, value);

    free(expression);
}
