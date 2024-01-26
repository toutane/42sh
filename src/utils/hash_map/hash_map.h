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

/* ==============NEW=STUFF=============== */

typedef void (*data_free_type)(void *);

enum hm_type
{
    HM_VARIABLE,
    HM_FUNCTION,
};

struct pl
{
    char *key;
    void *data;
    struct pl *next;
};

struct hm
{
    enum hm_type type;
    size_t size;
    data_free_type data_free;
    struct pl **pairs;
};

struct hm *hm_new(enum hm_type type, size_t size, data_free_type data_free);

void hm_free(struct hm *hm);

void *hm_get(struct hm *hm, const char *key);

void hm_set_var(struct hm *hm, const char *key, const char *value);

/* ==============NEW=STUFF=============== */

#endif /* ! HASH_MAP_H */
