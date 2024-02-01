#include <stdio.h>
#include <unistd.h>

#include "lexer/lexer.h"
#include "stack.h"

struct stack *stack_new(data_free_stack free_func)
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

static void items_free(struct item *item, data_free_stack free_func)
{
    if (item == NULL)
    {
        return;
    }

    items_free(item->next, free_func);

    struct item_info *item_data = item->data;
    stream_free(item_data->stream);
    free(item_data);
    free(item);
}

void stack_clear(struct stack *stack)
{
    if (stack == NULL)
    {
        return;
    }

    // clear stack head
    items_free(stack->head, stack->free_func);

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
