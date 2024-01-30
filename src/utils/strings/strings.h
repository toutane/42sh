#ifndef STRINGS_H
#define STRINGS_H

#include "../token/token.h"

/*
 * @brief: Appends a character to the value of a token.
 */
void append_char_to_token_value(struct token *tok, char c);

void append_char_to_string(char **str, char c);

void my_strcat(char **str1, char *str2);

/*
 * @brief: Returns 1 if the string is a sequence of digits, 0 otherwise.
 */
int is_str_sequence_of_digits(char *str);

int is_char_valid_in_name(char c);

int is_char_special_variable(char c);

int is_name(char *str);

char *int_to_string(int number);

int get_index_of_char(char *str, char target);

int is_char_in_string(char c, char *str);

#endif /* ! STRINGS_H */
