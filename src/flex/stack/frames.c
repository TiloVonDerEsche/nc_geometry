#include "frames.h"
#include <string.h>

extern YY_BUFFER_STATE yy_create_buffer(FILE *, int);
extern void yy_switch_to_buffer(YY_BUFFER_STATE);
extern void yy_delete_buffer(YY_BUFFER_STATE);
extern YY_BUFFER_STATE yy_current_buffer(void);
extern FILE *yyin;
#define YY_BUF_SIZE 16384

/* Handles file switching & state saving */
int push_call_frame(Stack *stack, FILE *new_fp, YY_BUFFER_STATE current_buf, FILE *current_fp) {
    if (is_full(stack)) {
        fprintf(stderr, "Include depth exceeded!\n");
        return 0;
    }

    CallFrame frame = {
        .buf_state = current_buf,
        .fp = current_fp
    };

    if (!push(stack, &frame)) {
        return 0;
    }

    // Switch Lexer state to new file
    yyin = new_fp;
    yy_switch_to_buffer(yy_create_buffer(yyin, YY_BUF_SIZE));
    return 1;
}

/* Pops frame, closes finished file, and restores Lexer buffer */
int pop_call_frame(Stack *stack, YY_BUFFER_STATE active_buf, YY_BUFFER_STATE *prev_buf, FILE **prev_fp) {
    CallFrame frame;

    // Close current active file and delete buffer before unwinding
    if (yyin) {
        fclose(yyin);
    }
    if (active_buf) {
        yy_delete_buffer(active_buf);
    }

    if (!pop(stack, &frame)) {
        return 0; // Root file reached
    }

    // Restore previous state
    *prev_buf = frame.buf_state;
    *prev_fp = frame.fp;

    yyin = frame.fp;
    yy_switch_to_buffer(frame.buf_state);
    return 1;
}

/* Handles GOTO / REPEAT label tracking */
int push_jmp_frame(Stack *stack, const char *label, size_t line, long byte_offset) {
    JmpFrame frame;
    strncpy(frame.label, label, STR_LEN);
    frame.label[STR_LEN] = '\0';
    frame.line = line;
    frame.byte_offset = byte_offset;

    return push(stack, &frame);
}
