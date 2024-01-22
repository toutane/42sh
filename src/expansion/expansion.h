#ifndef EXPANSION_H
#define EXPANSION_H

#include "../io/io.h"
#include "../lexer/lexer.h"
#include "../utils/hash_map/hash_map.h"
#include "utils/memory/memory.h"

void expand_string(char **str, struct hash_map *gv_hash_map);

#endif /* ! EXPANSION_H */
