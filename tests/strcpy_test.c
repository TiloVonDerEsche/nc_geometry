#include "./../src/helper_funcs.h"

void fn(char str[], size_t str_len, char** A, size_t* A_len) {

  char dest[20];
  strcpy(dest, str);

  *A = dest;
  *A_len = str_len;
  (*A)[str_len] = '\0';

  printf("str=%s,%lu\nA=%s,%lu\n",str,str_len,*A,*A_len);
}

int main() {

  char str[] = "AB123"; size_t str_len = 5;

  char* A; size_t A_len;
  fn(str,str_len,&A,&A_len);


  puts("Printing results...");
  printf("str=%s,%lu\nA=%s,%lu\n",str,str_len,A,A_len);

  return 0;
}
