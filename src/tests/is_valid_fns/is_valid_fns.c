#include "./../src/helper_funcs.h"

typedef uint8_t (*test_fn)(const char*);

void is_valid_test(test_fn fn, size_t arr_len, char* arr[], uint8_t test_results[]) {
  for (size_t i = 0; i < arr_len; i++) {
    printf("Test %lu:'%s', %s\n",(i+1) , arr[i],
    fn(arr[i]) ? "was a success!" : "failed!");
  }
}


int main () {
  size_t lit_arr_len = 12;
  char* lit_arr[] = {"1750", "Nope", "R8", "G00", "00",
                  "3.76", "0.12", ".2", "..", "...", "2.7e13", "10^5"};
  uint8_t test_results[lit_arr_len];

  size_t varn_arr_len = 10;
  char* varn_arr[] = {"R8", "G00", "VARIABLE", "14", "__",
                      "S 10", "GU\0lp", "New\n", "_hidden1", "____o____"};
  //uint8_t test_results[varn_arr_len];

  is_valid_test(is_valid_literal, lit_arr_len, lit_arr, test_results);
  puts("------------");
  is_valid_test(is_valid_varname, varn_arr_len, varn_arr, test_results);

  return 0;
}
