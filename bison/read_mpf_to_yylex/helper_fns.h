#ifndef HELPER_FNS_H
#define HELPER_FNS_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>

int is_valid_float(const char* str) {
    if (str == NULL || *str == '\0') {
        return 0;  // Empty or null string is invalid
    }

    char *endptr;
    errno = 0;  // Reset errno before conversion
    float val = strtof(str, &endptr);

    // Check for conversion errors
    if (endptr == str) {
        return 0;  // No digits were found
    }
    if (*endptr != '\0') {
        return 0;  // Trailing characters after number
    }
    if (errno == ERANGE) {
        return 0;  // Overflow or underflow
    }

    // Optional: Ignore unused 'val' to suppress warnings if not needed
    (void)val;

    return 1;  // Valid float
}

void split_cmd_w_fnum(const char* str, char* cmd_buf, char* fnum_buf) {
    const char* p = str;
    char* q = cmd_buf;

    // Copy characters until a non-alphabetic character is found
    while (*p && isalpha((unsigned char)*p)) {
        *q++ = *p++;
    }
    *q = '\0';

    if(is_valid_float(p)) {
      strcpy(fnum_buf, p);}
    else {strcpy(fnum_buf,"NaN");}
}






#endif
