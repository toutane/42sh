#ifndef ASSIGNMENT_WORDS_H
#define ASSIGNMENT_WORDS_H

#include "utils/hash_map/hash_map.h"
#include "utils/memory/memory.h"
#include "utils/strings/strings.h"

int is_valid_name_char(char c);

int only_assignment_words(char **argv);

int is_str_assignment_word(char *str);

#endif /* ! ASSIGNMENT_WORDS_H */
