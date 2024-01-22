#ifndef HASH_MAP_H
#define HASH_MAP_H

#include "hash_map_setup.h"

#include <stddef.h>

int hash_map_contains(const struct hash_map *hash_map, char *key);

char **hash_map_get(const struct hash_map *hash_map, char *key);

void hash_map_insert(struct hash_map *hash_map, char *key, char **value);

void hash_map_update(struct hash_map *hash_map, char *key, char **value);

void hash_map_print(struct hash_map *hash_map);

#endif /* ! HASH_MAP_H */
