#ifndef SPECIAL_VARIABLES_H
#define SPECIAL_VARIABLES_H

#include "../utils/hash_map/hash_map.h"
#include "../utils/strings/strings.h"

void fill_at_sign_var(int argc, char *argv[], struct hash_map *gv_hash_map);

void fill_star_sign_var(int argc, char *argv[], struct hash_map *gv_hash_map);

void fill_arguments_var(int argc, char *argv[], struct hash_map *gv_hash_map);

void fill_dollar_var(struct hash_map *gv_hash_map);

void fill_arguments_amount(int argc, struct hash_map *gv_hash_map);

#endif /* ! SPECIAL_VARIABLES_H */
