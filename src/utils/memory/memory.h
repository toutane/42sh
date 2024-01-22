#ifndef MEMORY_H
#define MEMORY_H

#include "utils/hash_map/hash_map.h"

struct hash_map *memory_new(void);

void memory_free(struct hash_map *memory);

char **memory_get(struct hash_map *memory, char *key);

void memory_set(struct hash_map *memory, char *key, char **value);

void memory_print(struct hash_map *memory);

#endif /* ! MEMORY_H */
