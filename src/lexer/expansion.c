#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "lexer.h"

void single_quote_expansion(char **str)
{
    if (*str == NULL)
    {
        return;
    }

    //printf("single_quote_expansion: %s\n", *str);

    size_t len = strlen(*str);
    if (len < 2)
    {
        return;
    }

    // Calculate the number of single quotes in the token value
    int nb_single_quotes = 0;
    for (size_t i = 0; i < len; i++)
    {
        if ((*str)[i] == '\'')
        {
            nb_single_quotes++;
        }
    }

    // Remove every single quote from the token value
    char *new_val = calloc(len - nb_single_quotes + 1, sizeof(char));
    int j = 0;
    for (size_t i = 0; i < len; i++)
    {
        if ((*str)[i] != '\'')
        {
            new_val[j] = (*str)[i];
            j++;
        }
    }

    //printf("new value: %s\n", new_val);

    // Free the old value and replace it with the new one
    free(*str);
    *str = new_val;
    return;
}
