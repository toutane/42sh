#include "utils/memory/memory.h"

#include <stdio.h>
#include <stdlib.h>

#define MEMORY_SIZE 128

struct hash_map *memory_new(void)
{
    struct hash_map *memory = hash_map_new(MEMORY_SIZE);
    if (memory == NULL)
    {
        fprintf(stderr, "42sh: memory_new: error: failed to allocate memory\n");
        exit(EXIT_FAILURE);
    }

    return memory;
}

void memory_free(struct hash_map *memory)
{
    if (memory == NULL)
    {
        return;
    }

    hash_map_free(memory);
    return;
}

char **memory_get(struct hash_map *memory, char *key)
{
    if (memory == NULL || key == NULL)
    {
        return NULL;
    }

    char **value = hash_map_get(memory, key);
    if (value == NULL)
    {
        return NULL;
    }

    return value;
}

void memory_set(struct hash_map *memory, char *key, char **value)
{
    if (memory == NULL || key == NULL || value == NULL)
    {
        return;
    }

    int is_key_in_memory = hash_map_contains(memory, key);
    if (is_key_in_memory == 0)
    {
        hash_map_insert(memory, key, value);
    }
    else
    {
        hash_map_update(memory, key, value);
    }

    return;
}

void memory_print(struct hash_map *memory)
{
    if (memory == NULL)
    {
        return;
    }

    hash_map_print(memory);
    return;
}
