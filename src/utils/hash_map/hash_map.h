#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <stddef.h>

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

size_t hash(const char *key);

struct hm *hm_new(enum hm_type type, size_t size, data_free_type data_free);

void hm_free(struct hm *hm);

void pl_free(struct pl *pair, data_free_type data_free);

void *hm_get(struct hm *hm, const char *key);

void hm_set_var(struct hm *hm, const char *key, const char *value);

void hm_remove(struct hm *hm, const char *key);

void hm_set_fun(struct hm *hm, const char *key, void *ast_void);

int hm_contains(struct hm *hm, const char *key);

struct hm *cpy_hm_var(struct hm *hm);

#endif /* ! HASH_MAP_H */
