#ifndef HASH_MAP_SETUP_H
#define HASH_MAP_SETUP_H

#include <stddef.h>

struct pair_list
{
    char *key;
    char **value;
    struct pair_list *next;
};

struct hash_map
{
    struct pair_list **data;
    size_t size;
};

size_t hash(const char *key);

struct hash_map *hash_map_new(size_t size);

void hash_map_free(struct hash_map *hash_map);

#endif /* ! HASH_MAP_SETUP_H */
