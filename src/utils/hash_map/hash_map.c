#include "hash_map.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int hash_map_contains(const struct hash_map *hash_map, char *key)
{
    if (hash_map == NULL || key == NULL)
    {
        return 0;
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
            return 1;
        }
    }

    return 0;
}

char **hash_map_get(const struct hash_map *hash_map, char *key)
{
    if (hash_map == NULL || key == NULL)
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

static void pair_list_insert(struct pair_list *new_pair,
                             struct pair_list *pair_list)
{
    if (pair_list == NULL)
    {
        return;
    }

    struct pair_list *cur = pair_list;
    while (cur->next)
    {
        cur = cur->next;
    }

    cur->next = new_pair;
    return;
}

void hash_map_insert(struct hash_map *hash_map, char *key, char **value)
{
    if (hash_map == NULL || hash_map->size == 0)
    {
        return;
    }

    size_t index = hash(key) % hash_map->size;

    // Create the new pair
    struct pair_list *new_pair = calloc(1, sizeof(struct pair_list));
    if (!new_pair)
    {
        fprintf(stderr, "42sh: hash_map_insert: calloc failed\n");
        exit(EXIT_FAILURE);
    }

    new_pair->key = key;
    new_pair->value = value;

    // Insert the new pair
    int is_already_a_pair = hash_map->data && hash_map->data[index] != NULL;
    if (is_already_a_pair)
    {
        pair_list_insert(new_pair, hash_map->data[index]);
    }
    else
    {
        hash_map->data[index] = new_pair;
    }

    return;
}

void hash_map_update(struct hash_map *hash_map, char *key, char **value)
{
    if (hash_map == NULL || key == NULL || value == NULL)
    {
        return;
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
            // Free the old key and value
            free(cur->key);
            size_t j = 0;
            while (cur->value[j] != NULL)
            {
                free(cur->value[j]);
                j++;
            }
            free(cur->value);

            // Update the key and value
            cur->key = key;
            cur->value = value;
        }
    }

    return;
}

static void pair_list_print(struct pair_list *pair_list)
{
    if (pair_list == NULL)
    {
        return;
    }

    printf("%s: [ ", pair_list->key == NULL ? "(null)" : pair_list->key);

    if (pair_list->value == NULL)
    {
        printf("(null) ]\n");
        return;
    }

    printf("%s", pair_list->value[0]);

    size_t i = 1;
    while (pair_list->value[i] != NULL)
    {
        printf(", %s", pair_list->value[i]);
        i++;
    }
    printf(" ]\n");

    return;
}

static void chained_pair_lists_print(struct pair_list *pair_list)
{
    if (pair_list == NULL)
    {
        return;
    }

    pair_list_print(pair_list);

    struct pair_list *next = pair_list->next;
    while (next)
    {
        printf("; ");
        pair_list_print(next);
        next = next->next;
    }

    return;
}

void hash_map_print(struct hash_map *hash_map)
{
    if (hash_map == NULL)
    {
        return;
    }

    for (size_t i = 0; i < hash_map->size; i++)
    {
        chained_pair_lists_print(hash_map->data[i]);
    }
    printf("\n");
}
