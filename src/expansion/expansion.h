#ifndef EXPANSION_H
#define EXPANSION_H

#include "io/io.h"
#include "lexer/lexer.h"
#include "utils/hash_map/hash_map.h"
#include "utils/memory/memory.h"
#include "utils/printers/printers.h"
#include "utils/variables/variables.h"

char *expand_string(char **str, struct mem *mem);

char **argv_expansions(char **original_argv, int *argc, struct mem *mem);

void command_substitution(char **str, struct stream_info *stream,
                          struct mem *mem);

#endif /* ! EXPANSION_H */
