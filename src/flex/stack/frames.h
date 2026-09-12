#ifndef FRAMES_H
#define FRAMES_H

#include <stdio.h>
#include "stack.h"

// Lexer types forward declaration (Flex)
#ifndef YY_TYPEDEF_YY_BUFFER_STATE
#define YY_TYPEDEF_YY_BUFFER_STATE
typedef struct yy_buffer_state *YY_BUFFER_STATE;
#endif

/* --- Call Frame (File Inclusion Stack) --- */
typedef struct {
    YY_BUFFER_STATE buf_state;
    FILE *fp;
} CallFrame;

/* --- Goto / Repeat Frame Stack --- */
#define STR_LEN 32
typedef struct {
    char label[STR_LEN + 1];
    size_t line;
    long byte_offset;
} JmpFrame;

YY_BUFFER_STATE get_current_buffer(void);
int terminate_lexer(void);

/* Context Wrappers */
int push_call_frame(Stack *stack, FILE *new_fp, YY_BUFFER_STATE current_buf, FILE *current_fp);
int pop_call_frame(Stack *stack, YY_BUFFER_STATE active_buf, YY_BUFFER_STATE *prev_buf, FILE **prev_fp);

int push_jmp_frame(Stack *stack, const char *label, size_t line, long byte_offset);

#endif
