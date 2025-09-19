#include "./../src/helper_funcs.h"

void is_valid_literal_test(size_t lit_arr_len, char* lit_arr[], uint8_t test_results[]) {
  for (size_t i = 0; i < lit_arr_len; i++) {
    printf("Test %lu:'%s', %s\n",(i+1) , lit_arr[i],
    is_valid_literal(lit_arr[i]) ? "was a success!" : "failed!");
  }
}


int main () {
  size_t lit_arr_len = 12;
  char* lit_arr[] = {"1750", "Nope", "R8", "G00", "00",
                  "3.76", "0.12", ".2", "..", "...", "2.7e13", "10^5"};
  uint8_t test_results[lit_arr_len];

  is_valid_literal_test(lit_arr_len, lit_arr, test_results);


  return 0;
}
