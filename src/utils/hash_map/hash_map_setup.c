#include "hash_map_setup.h"

#include <stdlib.h>

size_t hash(const char *key)
{
    size_t i = 0;
    size_t hash = 0;

    for (i = 0; key[i] != '\0'; ++i)
    {
        hash += key[i];
    }

    hash += i;
    return hash;
}

struct hash_map *hash_map_new(size_t size)
{
    struct hash_map *new_hash_map = calloc(1, sizeof(struct hash_map));
    if (new_hash_map == NULL)
    {
        return NULL;
    }

    new_hash_map->size = size;
    new_hash_map->data = calloc(size, sizeof(struct pair_list *));
    return new_hash_map;
}

static void pair_list_free(struct pair_list *pair_list)
{
    if (pair_list == NULL)
    {
        return;
    }

    free(pair_list->key);

    size_t i = 0;
    while (pair_list->value[i] != NULL)
    {
        free(pair_list->value[i]);
        i++;
    }

    free(pair_list->value);
    free(pair_list);
    return;
}

static void chained_pair_lists_free(struct pair_list *pair_list)
{
    if (pair_list == NULL)
    {
        return;
    }

    struct pair_list *next = pair_list->next;
    pair_list_free(pair_list);

    while (next)
    {
        pair_list = next;
        next = pair_list->next;
        pair_list_free(pair_list);
    }

    return;
}

void hash_map_free(struct hash_map *hash_map)
{
    if (hash_map == NULL)
    {
        return;
    }

    for (size_t i = 0; i < hash_map->size; i++)
    {
        chained_pair_lists_free(hash_map->data[i]);
    }

    free(hash_map->data);
    free(hash_map);
    return;
}
