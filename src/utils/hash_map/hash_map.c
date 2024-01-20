#include "hash_map.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static size_t hash(const char *key)
{
    size_t i = 0;
    size_t hash = 0;

    for (i = 0; key[i] != '\0'; ++i)
        hash += key[i];
    hash += i;

    return hash;
}

struct hash_map *hash_map_init(size_t size)
{
    struct hash_map *new_hash_map = calloc(1, sizeof(struct hash_map));
    if (!new_hash_map)
    {
        return NULL;
    }
    new_hash_map->size = size;
    new_hash_map->data = calloc(size, sizeof(struct pair_list *));
    return new_hash_map;
}

static int hash_map_insert_help(struct hash_map *hash_map, char *key,
                                char **value, int *updated)
{
    size_t index = hash(key) % hash_map->size;
    struct pair_list *cur = hash_map->data[index];
    while (cur && strcmp(cur->key, key) != 0)
    {
        cur = cur->next;
    }

    int has_to_be_updated = cur != NULL;
    if (has_to_be_updated)
    {
        cur->value = value;
        if (updated)
        {
            *updated = 1;
        }
        return 1;
    }
    else
    {
        struct pair_list *new_pair = calloc(1, sizeof(struct pair_list));
        if (!new_pair)
        {
            return 0;
        }
        new_pair->key = key;
        new_pair->value = value;
        struct pair_list *old_first_pair = hash_map->data[index];
        hash_map->data[index] = new_pair;
        new_pair->next = old_first_pair;
        if (updated)
        {
            *updated = 0;
        }
    }
    return 0;
}

int hash_map_insert(struct hash_map *hash_map, char *key, char **value,
                    int *updated)
{
    if (!hash_map || hash_map->size == 0)
    {
        return 0;
    }
    size_t index = hash(key) % hash_map->size;

    int is_already_a_pair = hash_map->data && hash_map->data[index] != NULL;
    if (is_already_a_pair)
    {
        hash_map_insert_help(hash_map, key, value, updated);
    }
    else
    {
        struct pair_list *new_pair = calloc(1, sizeof(struct pair_list));
        if (!new_pair)
        {
            return 0;
        }
        new_pair->key = key;
        new_pair->value = value;
        if (updated)
        {
            *updated = 0;
        }
        hash_map->data[index] = new_pair;
    }
    return 1;
}

void hash_map_free(struct hash_map *hash_map)
{
    if (!hash_map)
    {
        free(hash_map);
        return;
    }

    for (size_t i = 0; i < hash_map->size; i++)
    {
        struct pair_list *cur = hash_map->data[i];
        while (cur)
        {
            free(cur->key);
            int i = 0;
            while (cur->value[i] != NULL)
            {
                free(cur->value[i]);
                i++;
            }

            free(cur->value);
            struct pair_list *next = cur->next;
            free(cur);
            cur = next;
        }
    }
    free(hash_map->data);
    free(hash_map);
}

void hash_map_dump(struct hash_map *hash_map)
{
    printf("Hash map dump:\n");
    for (size_t i = 0; i < hash_map->size; i++)
    {
        struct pair_list *cur = hash_map->data[i];
        if (cur)
        {
            printf("%s:", cur->key);
            int i = 0;
            while (cur->value[i] != NULL)
            {
                printf(" %s", cur->value[i]);
                i++;
            }

            cur = cur->next;
            while (cur)
            {
                int j = 0;
                printf(", %s:", cur->key);
                while (cur->value[j] != NULL)
                {
                    printf(" %s", cur->value[j]);
                    j++;
                }
                cur = cur->next;
            }
            printf("\n");
        }
    }
}

char **hash_map_get(const struct hash_map *hash_map, char *key)
{
    if (!hash_map || !key)
    {
        return NULL;
    }
    for (size_t i = 0; i < hash_map->size; i++)
    {
        struct pair_list *cur = hash_map->data[i];
        while (cur && strcmp(cur->key, key) != 0)
        {
            cur = cur->next;
        }
        if (cur)
        {
            return cur->value;
        }
    }
    return NULL;
}

int hash_map_remove(struct hash_map *hash_map, char *key)
{
    if (!hash_map)
    {
        return 0;
    }
    for (size_t i = 0; i < hash_map->size; i++)
    {
        struct pair_list *prev = NULL;
        struct pair_list *cur = hash_map->data[i];
        while (cur && strcmp(cur->key, key) != 0)
        {
            prev = cur;
            cur = cur->next;
        }
        if (cur)
        {
            if (prev)
            {
                prev->next = cur->next;
            }
            else
            {
                hash_map->data[i] = cur->next;
            }
            free(cur);
            return 1;
        }
    }
    return 0;
}
