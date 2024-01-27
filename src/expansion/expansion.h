#ifndef EXPANSION_H
#define EXPANSION_H

#include "io/io.h"
#include "lexer/lexer.h"
#include "utils/hash_map/hash_map.h"
#include "utils/memory/memory.h"
#include "utils/printers/printers.h"
#include "utils/variables/variables.h"

char *expand_string(char **str, struct hm *hm_var);

char **argv_expansions(char **original_argv, int *argc, struct hm *hm_var);

#endif /* ! EXPANSION_H */
