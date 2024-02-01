#define _POSIX_C_SOURCE 200809L

#include <criterion/criterion.h>
#include <stdlib.h>
#include <string.h>

#include "utils/stack/stack.h"

TestSuite(stack, .timeout = 2);

Test(stack, stack_new)
{
    struct stack *stack = stack_new(free);

    cr_expect(stack->size == 0);
    cr_expect(stack->head == NULL);
    cr_expect(stack->free_func != NULL);

    free(stack);
}

Test(stack, stack_clear_empty_stack)
{
    struct stack *stack = stack_new(free);
    stack_clear(stack);

    cr_expect(stack->size == 0);
    cr_expect(stack->head == NULL);
    cr_expect(stack->free_func != NULL);

    free(stack);
}

Test(stack, stack_free_empty_stack)
{
    struct stack *stack = stack_new(free);

    cr_expect(stack->size == 0);
    cr_expect(stack->head == NULL);
    cr_expect(stack->free_func != NULL);

    stack_free(stack);
}

Test(stack, stack_push_one)
{
    struct stack *stack = stack_new(free);
    char *str = strdup("one");

    stack_push(stack, str);

    cr_expect(stack->head != NULL);
    cr_expect(stack->size == 1);
    cr_expect(stack->head->next == NULL);
    cr_expect(strcmp((char *)stack->head->data, "one") == 0);

    stack_free(stack);
}

Test(stack, stack_push_mult)
{
    struct stack *stack = stack_new(free);
    char *str1 = strdup("one");
    char *str2 = strdup("two");
    char *str3 = strdup("three");

    stack_push(stack, str1);
    stack_push(stack, str2);
    stack_push(stack, str3);

    cr_expect(stack->head != NULL);
    cr_expect(stack->size == 3);
    cr_expect(stack->head->next != NULL);
    cr_expect(stack->head->next->next != NULL);
    cr_expect(stack->head->next->next->next == NULL);
    cr_expect(strcmp((char *)stack->head->data, "three") == 0);
    cr_expect(strcmp((char *)stack->head->next->data, "two") == 0);
    cr_expect(strcmp((char *)stack->head->next->next->data, "one") == 0);

    stack_free(stack);
}

Test(stack, stack_pop_empty)
{
    struct stack *stack = stack_new(free);
    void *data = stack_pop(stack);

    cr_expect(data == NULL);
    cr_expect(stack->size == 0);

    free(data);
    stack_free(stack);
}

Test(stack, stack_pop_one)
{
    struct stack *stack = stack_new(free);
    char *str = strdup("one");

    stack_push(stack, str);
    void *data = stack_pop(stack);

    cr_expect(strcmp((char *)data, "one") == 0);
    cr_expect(stack->size == 0);
    cr_expect(stack->head == NULL);

    free(data);
    stack_free(stack);
}

Test(stack, stack_pop_mult)
{
    struct stack *stack = stack_new(free);
    char *str1 = strdup("one");
    char *str2 = strdup("two");
    char *str3 = strdup("three");

    stack_push(stack, str1);
    stack_push(stack, str2);
    stack_push(stack, str3);

    void *data = stack_pop(stack);
    cr_expect(stack->head != NULL);
    cr_expect(stack->size == 2);
    cr_expect(strcmp((char *)data, "three") == 0);

    data = stack_pop(stack);
    cr_expect(stack->head != NULL);
    cr_expect(stack->size == 1);
    cr_expect(strcmp((char *)data, "two") == 0);

    data = stack_pop(stack);
    cr_expect(stack->head == NULL);
    cr_expect(stack->size == 0);
    cr_expect(strcmp((char *)data, "one") == 0);

    free(str1);
    free(str2);
    free(str3);
    stack_free(stack);
}

Test(stack, stack_peek_empty)
{
    struct stack *stack = stack_new(free);
    void *data = stack_peek(stack);

    cr_expect(data == NULL);
    cr_expect(stack->size == 0);

    free(data);
    stack_free(stack);
}

Test(stack, stack_peek_mult)
{
    struct stack *stack = stack_new(free);
    char *str1 = strdup("one");
    char *str2 = strdup("two");
    char *str3 = strdup("three");

    stack_push(stack, str1);
    stack_push(stack, str2);
    stack_push(stack, str3);

    void *data = stack_peek(stack);
    cr_expect(stack->head != NULL);
    cr_expect(stack->size == 3);
    cr_expect(strcmp((char *)data, "three") == 0);

    data = stack_peek(stack);
    cr_expect(stack->head != NULL);
    cr_expect(stack->size == 3);
    cr_expect(strcmp((char *)data, "three") == 0);

    stack_free(stack);
}

Test(stack, stack_size_empty)
{
    struct stack *stack = stack_new(free);

    cr_expect(is_empty(stack) == 1);

    stack_free(stack);
}

Test(stack, stack_size_non_empty)
{
    struct stack *stack = stack_new(free);

    char *str1 = strdup("one");

    stack_push(stack, str1);

    cr_expect(is_empty(stack) == 0);

    stack_free(stack);
}
