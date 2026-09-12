#ifndef STACK_H
#define STACK_H

#include <stddef.h>

#define STACK_SIZE 100

typedef struct {
    void *data;
    size_t elem_size;
    int top;
    int capacity;
} Stack;

void stack_init(Stack *stack, size_t elem_size, int capacity);
void stack_free(Stack *stack);
int is_empty(const Stack *stack);
int is_full(const Stack *stack);
int push(Stack *stack, const void *elem);
int pop(Stack *stack, void *out_elem);
int peek(const Stack *stack, void *out_elem);

#endif
