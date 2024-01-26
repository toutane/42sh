#include "hash_map_setup.h"

#include <stdio.h>
#include <stdlib.h>

#include "hash_map.h"

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

/* ==============NEW=STUFF=============== */

struct hm *hm_new(enum hm_type type, size_t size, data_free_type data_free)
{
    struct hm *new_hm = calloc(1, sizeof(struct hm));
    if (new_hm == NULL)
    {
        fprintf(stderr, "42sh: hm_new: fail to calloc hash_map\n");
        exit(EXIT_FAILURE);
    }

    new_hm->type = type;
    new_hm->size = size;
    new_hm->data_free = data_free;

    new_hm->pairs = calloc(size, sizeof(struct pl *));
    if (new_hm->pairs == NULL)
    {
        fprintf(stderr, "42sh: hm_new: fail to calloc pairs\n");
        exit(EXIT_FAILURE);
    }

    return new_hm;
}

static void pl_free(struct pl *pair, data_free_type data_free)
{
    if (pair == NULL)
    {
        return;
    }

    pl_free(pair->next, data_free);

    data_free(pair->data);

    free(pair->key);

    free(pair);
}

void hm_free(struct hm *hm)
{
    if (hm == NULL)
    {
        return;
    }

    if (hm->pairs != NULL)
    {
        // Free each pairs of the array
        for (size_t i = 0; i < hm->size; i++)
        {
            pl_free(hm->pairs[i], hm->data_free);
        }

        // Free the array
        free(hm->pairs);
    }

    free(hm);
}

/* ==============NEW=STUFF=============== */
