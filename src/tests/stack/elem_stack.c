//https://www.geeksforgeeks.org/c/implement-stack-in-c/
//removed bool type and renamed some vars
//also changed stored vars from int to Elem

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

int push(Stack *stack, Elem value) {
    if (is_full(stack)) {
        printf("Stack Overflow\n");
        return 0;
    }
    stack->top++;
    strncpy(stack->arr[stack->top].label, value.label, STR_LEN);
    stack->arr[stack->top].label[STR_LEN] = '\0';

    stack->arr[stack->top].line = value.line;
    stack->arr[stack->top].byte_offset = value.byte_offset;
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


int main() {
    Stack myStack;
    init_stack(&myStack);
    Elem temp;

    printf("--- Testing Stack Initialization ---\n");
    printf("Is empty? %s\n", is_empty(&myStack) ? "Yes" : "No");

    printf("\n--- Testing Push Operations ---\n");
    Elem e1 = {"Header", 10, 0xAF};
    Elem e2 = {"Body", 25, 0xBF};

    if (push(&myStack, e1)) printf("Pushed: %s\n", e1.label);
    if (push(&myStack, e2)) printf("Pushed: %s\n", e2.label);

    printf("\n--- Testing Peek ---\n");
    if (peek(&myStack, &temp)) {
        printf("Peeked Top: %s (Line: %zu, Offset: %ld)\n",
                temp.label, temp.line, temp.byte_offset);
    }

    printf("\n--- Testing Pop Operations ---\n");
    while (!is_empty(&myStack)) {
        if (pop(&myStack, &temp)) {
            printf("Popped: %s\n", temp.label);
        }
    }

    printf("\n--- Testing Underflow ---\n");
    if (!pop(&myStack, &temp)) {
        printf("Caught expected underflow.\n");
    }

    printf("\n--- Testing Overflow ---\n");
    // Fill the stack to its limit
    for (int i = 0; i < STACK_SIZE; i++) {
        Elem filler = {"Filler", (size_t)i, (long)i};
        push(&myStack, filler);
    }

    // Attempt one more push
    Elem extra = {"Overflow_Test", 999, 999};
    if (!push(&myStack, extra)) {
        printf("Caught expected overflow.\n");
    }

    return 0;
}
