#include "./../src/helper_funcs.h"


int main() {

  char str[] = "AB123"; size_t str_len = 5;
  char* A; size_t A_len;
  char* B; size_t B_len;


  puts("Parsing...");
  parse_cmd_w_num(str, str_len,
                  &A, &A_len,
                  &B, &B_len);

  puts("Printing results...");
  printf("str=%s,%lu\nA=%s,%lu\nB=%s,%lu\n",str,str_len,A,A_len,B,B_len);

  return 0;
}
