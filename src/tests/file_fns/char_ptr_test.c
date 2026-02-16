#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


int main() {

  char test2[] = "yxcvbnm";
  char test3[] = "QWERTZUIOPÜ";
  char str[] = "ABCDEFG12345 ";
  char lit[] = "123";
  char test1[] = "DFGHJKLÖÄ";

  // char* char_ptr = str; //start at index 0 of str arr
  // while (!isdigit(*char_ptr)) {printf("%c,%lu\n",*char_ptr++,char_ptr-str);}

  char* char_ptr = lit;
  while (!isalpha((char)*char_ptr)) {
    printf("%c",*(char_ptr+2));
    *char_ptr++;
  }





  return 0;
}
