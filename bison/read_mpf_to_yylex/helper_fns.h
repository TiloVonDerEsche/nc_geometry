#ifndef HELPER_FNS_H
#define HELPER_FNS_H

#include <ctype.h>
#include <string.h>

void split_cmd_w_fnum(const char* str, char* cmd_buf, char* fnum_buf) {
    const char *p = str;
    char *q = cmd_buf;

    // Copy characters until a non-alphabetic character is found
    while (*p && isalpha((unsigned char)*p)) {
        *q++ = *p++;
    }
    *q = '\0';

    // Copy the remaining part (the float number) to fnum_buf
    strcpy(fnum_buf, p);
}


#endif
