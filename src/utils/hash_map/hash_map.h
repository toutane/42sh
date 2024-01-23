#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <stddef.h>

#include "utils/hash_map/hash_map_setup.h"

int hash_map_contains(const struct hash_map *hash_map, char *key);

char **hash_map_get(const struct hash_map *hash_map, char *key);

void hash_map_insert(struct hash_map *hash_map, char *key, char **value);

void hash_map_update(struct hash_map *hash_map, char *key, char **value);

void hash_map_map(struct hash_map *hash_map, void (*f)(char *, char **));

void hash_map_print(struct hash_map *hash_map);

#endif /* ! HASH_MAP_H */
