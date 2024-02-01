#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast/ast.h"
#include "utils/hash_map/hash_map.h"

void *hm_get(struct hm *hm, const char *key)
{
    if (hm == NULL)
    {
        return NULL;
    }

    size_t index = hash(key) % hm->size;

    struct pl *cur = hm->pairs[index];
    while (cur != NULL && strcmp(cur->key, key) != 0)
    {
        cur = cur->next;
    }

    return cur ? cur->data : NULL;
}

int hm_contains(struct hm *hm, const char *key)
{
    if (hm == NULL || key == NULL)
    {
        return 0;
    }

    size_t index = hash(key) % hm->size;

    struct pl *cur = hm->pairs[index];
    while (cur != NULL && strcmp(cur->key, key) != 0)
    {
        cur = cur->next;
    }

    return cur != NULL;
}

void hm_set_var(struct hm *hm, const char *key, const char *value)
{
    size_t index = hash(key) % hm->size;

    if (hm_contains(hm, key))
    {
        // Update value and free old one
        struct pl *cur = hm->pairs[index];
        while (cur != NULL && strcmp(cur->key, key) != 0)
        {
            cur = cur->next;
        }

        hm->data_free(cur->data);
        cur->data = value ? strdup(value) : NULL;
    }
    else
    {
        // Create a new pair and set key and value by duplicate them
        struct pl *new_pl = calloc(1, sizeof(struct pl));
        if (new_pl == NULL)
        {
            fprintf(stderr, "42: hm_set_var: calloc failed\n");
            exit(EXIT_FAILURE);
        }

        new_pl->key = strdup(key);
        new_pl->data = strdup(value);
        new_pl->next = NULL;

        struct pl *prev = hm->pairs[index];
        if (prev == NULL)
        {
            hm->pairs[index] = new_pl;
        }
        else
        {
            while (prev->next != NULL)
            {
                prev = prev->next;
            }

            prev->next = new_pl;
        }
    }
}

void hm_set_fun(struct hm *hm, const char *key, void *ast_void)
{
    struct ast *ast = (struct ast *)ast_void;
    size_t index = hash(key) % hm->size;

    if (hm_contains(hm, key))
    {
        // Update value and free old one
        struct pl *cur = hm->pairs[index];
        while (cur != NULL && strcmp(cur->key, key) != 0)
        {
            cur = cur->next;
        }

        hm->data_free(cur->data);
        cur->data = ast;
    }
    else
    {
        // Create a new pair and set key and value by duplicate them
        struct pl *new_pl = calloc(1, sizeof(struct pl));
        if (new_pl == NULL)
        {
            fprintf(stderr, "42: hm_set_var: calloc failed\n");
            exit(EXIT_FAILURE);
        }

        new_pl->key = strdup(key);
        new_pl->data = ast;
        new_pl->next = NULL;

        struct pl *prev = hm->pairs[index];
        if (prev == NULL)
        {
            hm->pairs[index] = new_pl;
        }
        else
        {
            while (prev->next != NULL)
            {
                prev = prev->next;
            }

            prev->next = new_pl;
        }
    }
}

void hm_remove(struct hm *hm, const char *key)
{
    if (hm == NULL || !hm_contains(hm, key))
    {
        return;
    }

    size_t index = hash(key) % hm->size;

    struct pl *prev = NULL;
    struct pl *cur = hm->pairs[index];
    while (cur != NULL && strcmp(cur->key, key) != 0)
    {
        prev = cur;
        cur = cur->next;
    }

    if (cur != NULL)
    {
        struct pl *to_remove = cur;
        if (prev != NULL)
        {
            prev->next = cur->next;
        }

        // Free old pair list
        pl_free(to_remove, hm->data_free);

        if (prev == NULL)
        {
            hm->pairs[index] = NULL;
        }
    }
}

struct hm *cpy_hm_var(struct hm *hm)
{
    if (hm == NULL)
    {
        return NULL;
    }

    // create empty cpy hm
    struct hm *cpy_hm = hm_new(hm->type, hm->size, hm->data_free);

    // cpy pairs from hm to cpy_hm
    for (size_t i = 0; i < hm->size; ++i)
    {
        if (hm->pairs[i] == NULL)
        {
            continue;
        }

        struct pl *cur = hm->pairs[i];
        while (cur != NULL)
        {
            hm_set_var(cpy_hm, cur->key, cur->data);
            cur = cur->next;
        }
    }

    return cpy_hm;
}
