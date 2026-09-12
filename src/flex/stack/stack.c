#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void stack_init(Stack *stack, size_t elem_size, int capacity) {
    stack->elem_size = elem_size;
    stack->capacity = capacity > 0 ? capacity : STACK_SIZE;
    stack->top = -1;
    stack->data = malloc(stack->elem_size * stack->capacity);
}

void stack_free(Stack *stack) {
    if (stack->data) {
        free(stack->data);
        stack->data = NULL;
    }
    stack->top = -1;
}

int is_empty(const Stack *stack) {
    return stack->top == -1;
}

int is_full(const Stack *stack) {
    return stack->top >= stack->capacity - 1;
}

int push(Stack *stack, const void *elem) {
    if (is_full(stack)) {
        fprintf(stderr, "Error: Stack full, while trying to push!\n");
        return 0;
    }
    stack->top++;
    void *target = (char *)stack->data + (stack->top * stack->elem_size);
    memcpy(target, elem, stack->elem_size);
    return 1;
}

int pop(Stack *stack, void *out_elem) {
    if (is_empty(stack)) {
        fprintf(stderr, "Error: Stack empty, while trying to pop!\n");
        return 0;
    }
    void *source = (char *)stack->data + (stack->top * stack->elem_size);
    if (out_elem) {
        memcpy(out_elem, source, stack->elem_size);
    }
    stack->top--;
    return 1;
}

int peek(const Stack *stack, void *out_elem) {
    if (is_empty(stack)) {
        fprintf(stderr, "Error: Stack empty, while trying to peek!\n");
        return 0;
    }
    void *source = (char *)stack->data + (stack->top * stack->elem_size);
    memcpy(out_elem, source, stack->elem_size);
    return 1;
}
