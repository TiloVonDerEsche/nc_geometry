#include "./../../src/expr.h"
#include <stdio.h>
#include <string.h>

#define MAX_INPUT_SIZE 100

// Custom function that returns the sum of its two arguments
// static float add(struct expr_func *f, vec_expr_t *args, void *c) {
//   float a = expr_eval(&vec_nth(args, 0));
//   float b = expr_eval(&vec_nth(args, 1));
//   return a + b;
// }
//
static struct expr_func user_funcs[] = {
    {NULL, NULL, NULL, 0},
};

int main() {
  char input_buf[MAX_INPUT_SIZE];
  char* test_str = "5+5";

  struct expr *e;
  struct expr_var_list vars = {0};

  size_t len = 0;

  printf("To quit, press Ctrl+D (or Ctrl+Z on Windows) and Enter.\n\n");
  while(1) {
    printf(">>> Enter an expression to eval: ");
    //const char *s = "x = 5, add((2+3)*10, x)";

    if (fgets(input_buf, MAX_INPUT_SIZE, stdin) == NULL) {
      printf("\n\nEOF received. Exiting program. Goodbye!\n");
      break;
    }

    len = strlen(input_buf);
    if (0 < len && input_buf[len - 1] == '\n') { //remove '\n'
      input_buf[len - 1] = '\0';
      len--;
    }
    printf("input_buf=%s\n",input_buf);

    e = expr_create(input_buf, len, &vars, user_funcs);
    if (e == NULL) {
      printf("Syntax error");
      break;
    }

    float result = expr_eval(e);
    printf("result: %f\n", result);

  }

  expr_destroy(e, &vars);
  return 0;
}
