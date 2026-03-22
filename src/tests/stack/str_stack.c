//https://www.geeksforgeeks.org/c/implement-stack-in-c/
//removed bool type and renamed some vars
//also changed stored vars from int to char[STR_LEN]

#include <stdio.h>
#include <string.h>

#define STACK_SIZE 100
#define STR_LEN 32 //32 chars for labels allowed, 1 char for \0

// Define a structure for the stack
typedef struct {
    char arr[STACK_SIZE][STR_LEN+1];
    int top;
} Stack;

void initialize(Stack *stack) {
    stack->top = -1;
}

int is_empty(Stack *stack) {
    return stack->top == -1;
}

int is_full(Stack *stack) {
    return stack->top >= STACK_SIZE - 1;
}

void push(Stack *stack, char* value) {
    if (is_full(stack)) {
        printf("Stack Overflow\n");
        return;
    }

    strncpy(stack->arr[++stack->top], value, STR_LEN);
    stack->arr[stack->top][STR_LEN] = '\0';
    printf("Pushed %s onto the stack\n", value);
}

char* pop(Stack *stack) {
    if (is_empty(stack)) {
        printf("Stack Underflow\n");
        return NULL;
    }

    char* popped = stack->arr[stack->top];
    stack->top--;
    printf("Popped %s from the stack\n", popped);
    return popped;
}

char* peek(Stack *stack) {
    if (is_empty(stack)) {
        printf("Stack is empty\n");
        return NULL;
    }
    return stack->arr[stack->top];
}

int main() {
    Stack stack;
    initialize(&stack);

    char* str1 = "JUPA";
    char* str2 = "CON1";
    char* str3 = "CON2";
    char* str4 = "BOUCLE";

    push(&stack, str1);
    printf("Top element: %s\n", peek(&stack));

    push(&stack, str2);
    printf("Top element: %s\n", peek(&stack));

    push(&stack, str3);
    printf("Top element: %s\n", peek(&stack));

    push(&stack, str4);
    printf("Top element: %s\n", peek(&stack));

    while (!is_empty(&stack)) {
        printf("Top element: %s\n", peek(&stack));
        printf("Popped element: %s\n", pop(&stack));
    }
    printf("Popped element: %s\n", pop(&stack));

    return 0;
}
