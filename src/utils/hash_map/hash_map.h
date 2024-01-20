#ifndef HASH_MAP_H
#define HASH_MAP_H

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

struct hash_map *hash_map_init(size_t size);

int hash_map_insert(struct hash_map *hash_map, char *key, char **value,
                    int *updated);

void hash_map_free(struct hash_map *hash_map);

void hash_map_dump(struct hash_map *hash_map);

char **hash_map_get(const struct hash_map *hash_map, char *key);

int hash_map_remove(struct hash_map *hash_map, char *key);

#endif /* ! HASH_MAP_H */
