#ifndef PARAM_EXPANSION_H
#define PARAM_EXPANSION_H

#include "utils/assignment_words/assignment_words.h"
#include "utils/hash_map/hash_map.h"

void expand_parameter(char **str, struct hash_map *gv_hash_map);

#endif /* ! PARAM_EXPANSION_H */
