#include "./../src/helper_funcs.h"



int main() {
  char str[] = "";

  printf("str=%s\n",str);
  flip_str(str,0);puts("Flipping...");
  printf("str=%s\n",str);

  return 0;
}
