#ifndef EXPANSION_H
#define EXPANSION_H

#include "../expansion/special_variables.h"
#include "../io/io.h"
#include "../lexer/lexer.h"
#include "../utils/hash_map/hash_map.h"
#include "../utils/memory/memory.h"

char *expand_string(char **str, struct hash_map *gv_hash_map);

char **argv_expansions(char **original_argv, int *argc,
                       struct hash_map *memory);

#endif /* ! EXPANSION_H */
