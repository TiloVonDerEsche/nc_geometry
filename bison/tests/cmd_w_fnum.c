#include <ctype.h>
#include <string.h>
#include <stdio.h>

#define INPUT_BUF_LEN 40
#define CMD_BUF_LEN 20
#define FNUM_BUF_LEN 20


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


int main() {


  char input_buf[INPUT_BUF_LEN] = {};
  char cmd_buf[CMD_BUF_LEN] = {};
  char fnum_buf[FNUM_BUF_LEN] = {};


  while(1) {
  printf(">>> Enter a cmd_w_fnum to split: ");

    if (fgets(input_buf, INPUT_BUF_LEN, stdin) == NULL) {
      printf("\n\nEOF received. Exiting program. Goodbye!\n");
      return 0;
    }


    split_cmd_w_fnum(input_buf, cmd_buf, fnum_buf);

    printf("cmd=%s, fnum=%s\n",cmd_buf,fnum_buf);
  }

  return 0;
}
