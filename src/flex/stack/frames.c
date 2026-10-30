#include "frames.h"
#include <string.h>

/* Handles file switching & state saving */
int push_call_frame(Stack *stack, char* old_fname, size_t old_line) {
    if (is_full(stack)) {
        fprintf(stderr, "Include depth exceeded!\n");
        return 0;
    }
	
    //save CALLER filename and line, for their restoration, when CALLEE is finished
    CallFrame frame = {
        .fname = old_fname,
	.line = old_line
    };

    if (!push(stack, &frame)) {
        fprintf(stderr, "Pushing call frame failed!\n");
        return 0;
    }

    return 1;
}

/* Pops frame, closes finished file, and restores Lexer buffer */
int pop_call_frame(Stack *stack, char** prev_fname, size_t* prev_line) {
    CallFrame frame;

    //Mutate CallFrame
    if (!pop(stack, &frame)) {
	fprintf(stderr, "Popping call frame failed!\n Perhaps at root main file?\n");
        return 0; // Root file reached
    }

    // Restore previous state
    *prev_fname = frame.fname;
    *prev_line = frame.line;
    return 1;
}

/* Handles GOTO / REPEAT label tracking */
int push_jmp_frame(Stack *stack, const char *label, size_t line, long byte_offset) {
    JmpFrame frame;
    //Fill frame with params
    strncpy(frame.label, label, STR_LEN);
    frame.label[STR_LEN] = '\0';
    frame.line = line;
    frame.byte_offset = byte_offset;

    //Push frame
    return push(stack, &frame);
}
