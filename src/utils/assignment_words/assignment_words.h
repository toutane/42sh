#ifndef ASSIGNMENT_WORDS_H
#define ASSIGNMENT_WORDS_H

#include "utils/hash_map/hash_map.h"

int is_valid_name_char(char c);

int only_assignment_words(char **argv);

void update_gv_hash_map(char **argv, struct hash_map *gv_hash_map);

#endif /* ! ASSIGNMENT_WORDS_H */
