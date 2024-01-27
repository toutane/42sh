#include "utils/memory/memory.h"

#include <stdio.h>
#include <stdlib.h>

#define MEMORY_SIZE 128

struct mem *mem_new(void)
{
    struct mem *mem = calloc(1, sizeof(struct mem));
    if (mem == NULL)
    {
        fprintf(stderr, "42sh: mem_new: failed to calloc\n");
        exit(EXIT_FAILURE);
    }

    struct hm *hm_var = hm_new(HM_VARIABLE, MEMORY_SIZE, free);
    struct hm *hm_fun = hm_new(HM_FUNCTION, MEMORY_SIZE, ast_free);

    mem->hm_var = hm_var;
    mem->hm_fun = hm_fun;

    return mem;
}

void mem_free(struct mem *mem)
{
    if (mem == NULL)
    {
        return;
    }

    hm_free(mem->hm_var);
    hm_free(mem->hm_fun);

    free(mem);

    return;
}
