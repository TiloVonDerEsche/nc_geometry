#ifndef FRAMES_H
#define FRAMES_H

#include <stdio.h>
#include "stack.h"

/* --- Call Frame (File Inclusion Stack) --- */
typedef struct {
    //FILE *fp;
    char *fname;
    size_t line;
} CallFrame;

/* --- Goto / Repeat Frame Stack --- */
#define STR_LEN 32
typedef struct {
    char label[STR_LEN + 1];
    size_t line;
    long byte_offset;
} JmpFrame;


/* Context Wrappers */
int push_call_frame(Stack *stack, char* fname, size_t line); //save current file_name and line
int pop_call_frame(Stack *stack, char** fname, size_t* line); //resore previous file_name and line

int push_jmp_frame(Stack *stack, const char *label, size_t line, long byte_offset);

#endif
