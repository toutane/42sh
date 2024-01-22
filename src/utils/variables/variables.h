#ifndef VARIABLES_H
#define VARIABLES_H

#include "utils/memory/memory.h"
#include "utils/strings/strings.h"

void set_var_from_assignment_word(struct hash_map *memory,
                                  char *assignment_word_str);

void setenv_from_memory(struct hash_map *memory);

void unsetenv_from_memory(struct hash_map *memory);

void setup_memory_with_environ(struct hash_map *memory);

#endif /* ! VARIABLES_H */
