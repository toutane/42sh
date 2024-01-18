#include <stdlib.h>
#include <string.h>

#include "lexer.h"

void single_quote_expansion(struct lexer *lexer)
{
    if (lexer->cur_tok.type != TOKEN_WORD)
    {
        return;
    }

    char *val = lexer->cur_tok.value;
    size_t len = strlen(val);
    if (len < 2)
    {
        return;
    }

    // Calculate the number of single quotes in the token value
    int nb_single_quotes = 0;
    for (size_t i = 0; i < len; i++)
    {
        if (val[i] == '\'')
        {
            nb_single_quotes++;
        }
    }

    // Remove every single quote from the token value
    char *new_val = calloc(len - nb_single_quotes + 1, sizeof(char));
    int j = 0;
    for (size_t i = 0; i < len; i++)
    {
        if (val[i] != '\'')
        {
            new_val[j] = val[i];
            j++;
        }
    }

    free(lexer->cur_tok.value);
    lexer->cur_tok.value = new_val;
}

void double_quote_expansion(struct lexer *lexer)
{
    if (lexer->cur_tok.type != TOKEN_WORD)
    {
        return;
    }

    char *val = lexer->cur_tok.value;
    size_t len = strlen(val);
    if (len < 2)
    {
        return;
    }

    // Calculate the number of double quotes in the token value
    int nb_double_quotes = 0;
    for (size_t i = 0; i < len; i++)
    {
        if (val[i] == '"')
        {
            nb_double_quotes++;
        }
    }

    // Remove every double quote from the token value
    char *new_val = calloc(len - nb_double_quotes + 1, sizeof(char));
    int j = 0;
    for (size_t i = 0; i < len; i++)
    {
        if (nb_double_quotes > 0 && val[i] == '\\')
        {
            if (i + 1 < len && val[i + 1] == '\\')
            {
                new_val[j] = '\\';
                j++;
                i++;
            }
            else
            {
                new_val[j] = val[i];
                j++;
            }
        }
        else
        {
            if (val[i] != '"')
            {
                new_val[j] = val[i];
                j++;
            }
        }
    }

    free(lexer->cur_tok.value);
    lexer->cur_tok.value = new_val;
}

void expand_str(char **str)
{}
