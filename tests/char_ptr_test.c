#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


int main() {
  char str[] = "ABCDEFG12345 ";
  char* char_ptr = str; //start at index 0 of str arr

  while (!isdigit(*char_ptr)) {printf("%c",*char_ptr++);}




  return 0;
}
