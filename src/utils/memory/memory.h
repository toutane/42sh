#ifndef MEMORY_H
#define MEMORY_H

#include "ast/ast.h"
#include "utils/hash_map/hash_map.h"

struct mem
{
    struct hm *hm_var;
    struct hm *hm_fun;
};

struct mem *mem_new(void);

void mem_free(struct mem *mem);

#endif /* ! MEMORY_H */
