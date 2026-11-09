#include <stdio.h>
#include <string.h>

#define STACK_SIZE 100 //Include Depth

typedef struct {
    YY_BUFFER_STATE buf_state;
    FILE* fp;
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

int push(Stack *stack, YY_BUFFER_STATE buf_state, FILE* fp) { //Elem value) {
    if (is_full(stack)) {
        printf("Stack is full!\n");
        return 0;
    }
    stack->top++;
    stack->arr[stack->top].buf_state = YY_CURRENT_BUFFER;
    stack->arr[stack->top].fp = yyin;
    //printf("Pushed %s onto the stack\n", value);
    yyin = ncf;
    yy_switch_to_buffer(yy_create_buffer(yyin, YY_BUF_SIZE));

    return 1;
}

int pop(Stack *stack, Elem *out) {
    if (is_empty(stack)) {
        printf("Stack is empty!\n");
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
