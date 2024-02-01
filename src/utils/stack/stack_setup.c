#include "stack.h"

#include <stdio.h>
#include <unistd.h>

struct stack *stack_new(data_free_type free_func)
{
    struct stack *new = calloc(1, sizeof(struct stack));
    if (new == NULL)
    {
        fprintf(stderr, "42sh: stack: stack_new: calloc failed\n");
        _exit(EXIT_FAILURE);
    }

    new->head = NULL;
    new->free_func = free_func;

    return new;
}

static void item_free(struct item *item, data_free_type free_func)
{
    if (item == NULL)
    {
        return;
    }

    item_free(item->next, free_func);

    // free current item
    free_func(item->data);
    free(item);
}

void stack_clear(struct stack *stack)
{
    if (stack == NULL)
    {
        return;
    }

    // clear stack head
    item_free(stack->head, stack->free_func);
    
    // reset stack info
    stack->head = NULL;
    stack->size = 0;
}

void stack_free(struct stack *stack)
{
    if (stack == NULL)
    {
        return;
    }

    stack_clear(stack);
    free(stack);
}
