#include "./../src/helper_funcs.h"
#include "./../src/file_funcs.h"






int main() {
  FILE* tf = fopen("./test.txt", "r");
  if (tf == NULL) {
      fprintf(stderr, "Error: Could not open ./test.txt (in read mode)!\n");
      return 0;
  }

  size_t line_count = count_lines(tf);

  printf("%s contains %lu lines!","./test.txt",line_count);

  return 0;
}
