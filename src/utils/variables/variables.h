#ifndef VARIABLES_H
#define VARIABLES_H

#include "expansion/special_variables.h"
#include "utils/memory/memory.h"
#include "utils/printers/printers.h"
#include "utils/strings/strings.h"

// void set_default_variables(int argc, char *argv[], struct hash_map *memory);

char *get_key_from_assignment_word(char *assignment_word);

char *get_value_from_assignment_word(char *assignment_word);

void assign_variable_from_ass_word(char *assignment_word, struct hm *hm);

void assign_variable(char *key, char *value, struct hm *hm);

char *get_variable(char *key, struct hm *hm);

void setenv_from_hm(struct hm *hm);

void save_env(struct hm *hm_prefixes);

void restore_env(struct hm *hm_prefixes);

#endif /* ! VARIABLES_H */
