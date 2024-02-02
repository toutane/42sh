#ifndef STACK_H
#define STACK_H

#include <stdlib.h>

struct item
{
    void *data;
    struct item *next;
};

typedef void (*data_free_stack)(void *);
struct stack
{
    struct item *head;
    data_free_stack free_func;
    size_t size;
};

/*
 * @brief set up the stack structure
 */
struct stack *stack_new(data_free_stack free_func);

void stack_clear(struct stack *stack);

void stack_free(struct stack *stack);

/*
 * @brief set up the stack structure
 */
int is_empty(struct stack *stack);

void stack_push(struct stack *stack, void *data);

void *stack_pop(struct stack *stack);

void *stack_peek(struct stack *stack);

#endif /* ! STACK_H */
