#include <stdio.h>
#include <stdlib.h>

#include "utils/hash_map/hash_map.h"

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

void pl_free(struct pl *pair, data_free_type data_free)
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

void hm_clear(struct hm *hm)
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
            hm->pairs[i] = NULL;
        }
    }
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
