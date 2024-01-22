#ifndef VARIABLES_H
#define VARIABLES_H

#include "utils/memory/memory.h"
#include "utils/strings/strings.h"

void set_var_from_assignment_word(struct hash_map *memory,
                                  char *assignment_word_str);

#endif /* ! VARIABLES_H */
