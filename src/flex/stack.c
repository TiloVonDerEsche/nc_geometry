#include <stdio.h>
#include <string.h>

#define STACK_SIZE 100
#define STR_LEN 32 //32 chars for labels allowed, 1 char for \0

typedef struct {
    char label[STR_LEN+1];
    size_t line;
    long byte_offset;
} Elem;

// Define a structure for the stack
typedef struct {
    Elem arr[STACK_SIZE];
    int top;
} Stack;

void init_stack(Stack *stack) {
    stack->top = -1;
}

int is_empty(Stack *stack) {
    return stack->top == -1;
}

int is_full(Stack *stack) {
    return stack->top >= STACK_SIZE - 1;
}

int push(Stack *stack, char* label, size_t line, long byte_offset) { //Elem value) {
    if (is_full(stack)) {
        printf("Stack Overflow\n");
        return 0;
    }
    stack->top++;
    strncpy(stack->arr[stack->top].label, label, STR_LEN);
    stack->arr[stack->top].label[STR_LEN] = '\0';

    stack->arr[stack->top].line = line;
    stack->arr[stack->top].byte_offset = byte_offset;
    //printf("Pushed %s onto the stack\n", value);
    return 1;
}

int pop(Stack *stack, Elem *out) {
    if (is_empty(stack)) {
        printf("Stack Underflow\n");
        return 0;}
    *out = stack->arr[stack->top--];
    return 1;
}

int peek(Stack *stack,  Elem *out) {
    if (is_empty(stack)) {
        printf("Stack is empty\n");
        return 0;}

    *out = stack->arr[stack->top];
    return 1;
}
