#include "strings.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void append_char_to_token_value(struct token *tok, char c)
{
    if (tok == NULL)
    {
        fprintf(stderr, "append_char_to_token_value: tok is NULL\n");
        return;
    }

    if (tok->value == NULL)
    {
        tok->value = calloc(2, sizeof(char));
        tok->value[0] = c;
        tok->value[1] = '\0';
        return;
    }

    size_t len = strlen(tok->value);
    tok->value = realloc(tok->value, len + 2);
    tok->value[len] = c;
    tok->value[len + 1] = '\0';
}

void append_char_to_string(char **str, char c)
{
    if (*str == NULL)
    {
        (*str) = calloc(2, sizeof(char));
        (*str)[0] = c;
        (*str)[1] = '\0';
        return;
    }

    size_t len = strlen(*str);
    *str = realloc(*str, (len + 2) * sizeof(char));
    (*str)[len] = c;
    (*str)[len + 1] = '\0';
}

// concat str2 in str1
void my_strcat(char **str1, char *str2)
{
    if (str2 == NULL)
    {
        return;
    }

    size_t i = 0;
    while (str2[i] != '\0')
    {
        append_char_to_string(str1, str2[i]);
        i++;
    }
}

int is_str_sequence_of_digits(char *str)
{
    size_t len = strlen(str);
    for (size_t i = 0; i < len; i++)
    {
        if (!isdigit(str[i]))
        {
            return 0;
        }
    }

    return 1;
}

int is_char_special_variable(char c)
{
    return c == '?' || c == '$' || c == '#' || c == '@' || c == '*';
}

int is_name(char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (!isalnum(str[i]) && str[i] != '_')
        {
            return 0;
        }
    }

    return 1;
}

char *int_to_string(int number)
{
    // Determine the number of digits in the integer
    int numDigits = snprintf(NULL, 0, "%d", number);

    // Allocate memory for the string (including space for null terminator)
    char *result = (char *)malloc(numDigits + 1);

    if (result != NULL)
    {
        // Convert the integer to a string
        snprintf(result, numDigits + 1, "%d", number);
    }

    return result;
}

int get_index_of_char(char *str, char target)
{
    size_t target_pos = 0;
    while (str[target_pos] != '\0')
    {
        if (str[target_pos] == target)
        {
            return target_pos;
        }

        target_pos++;
    }

    return 0;
}

int is_char_in_string(char c, char *str)
{
    int i = 0;
    while (str[i] != '\0')
    {
        if (str[i] == c)
        {
            return 1;
        }
        i++;
    }
    return 0;
}
