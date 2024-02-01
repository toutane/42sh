#include "stack.h"

#include <stdio.h>
#include <unistd.h>

int is_empty(struct stack *stack)
{
    if (stack == NULL)
    {
        return 0;
    }

    return stack->size == 0 ? 1 : 0;
}

void stack_push(struct stack *stack, void *data)
{
    if (stack == NULL)
    {
        return;
    }

    struct item *new_item = calloc(1, sizeof(struct item));
    if (new_item == NULL)
    {
        fprintf(stderr, "42sh: stack: stack_push: calloc item failed\n");
        _exit(EXIT_FAILURE);
    }

    new_item->data = data;
    
    // link new item to the stack
    new_item->next = stack->head;
    stack->head = new_item;
    stack->size += 1;
}

void *stack_pop(struct stack *stack)
{
    if (stack == NULL || stack->size == 0)
    {
        return NULL;
    }

    struct item *item_pop = stack->head;

    // Update stack
    stack->head = stack->head->next;
    stack->size -= 1;

    // Save data and free item
    void *data = item_pop->data;
    free(item_pop);

    return data;
}

void *stack_peek(struct stack *stack)
{
    if (stack == NULL || stack->size == 0)
    {
        return NULL;
    }

    return stack->head->data;
}
