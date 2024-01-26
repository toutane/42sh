#ifndef VARIABLES_H
#define VARIABLES_H

#include "expansion/special_variables.h"
#include "utils/memory/memory.h"
#include "utils/strings/strings.h"

void set_var_from_assignment_word(struct hash_map *memory,
                                  char *assignment_word_str);

void setenv_from_memory(struct hash_map *memory);

void unsetenv_from_memory(struct hash_map *memory);

void setup_memory_with_environ(struct hash_map *memory);

void set_default_variables(int argc, char *argv[], struct hash_map *memory);

void assign_variable(char *assignment_word, struct hm *hm);

char *get_variable(char *key, struct hm *hm);

void setenv_from_hm(struct hm *hm);

char *get_key_from_assignment_word(char *assignment_word);

char *get_value_from_assignment_word(char *assignment_word);

#endif /* ! VARIABLES_H */
