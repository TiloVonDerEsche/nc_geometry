
#include "./../src/helper_funcs.h"

//split string across the switch from aplhas to a float num
// void parse(size_t str_len, char (*A)[20], size_t* A_len,
//                         char **B, size_t* B_len) {
//   puts("Inside of parsing fn!");
//   size_t m = 0;
//   char* char_ptr = *A; //set to start of str
//
//   printf("*char_ptr=%lu\n",char_ptr);
//   puts("Finding char float num switch...");
//   printf("*char_ptr=%c\n",*char_ptr);
//
//   while (isalpha(*char_ptr) && m < str_len) {
//     printf("*char_ptr=%c\n",*char_ptr);
//     char_ptr++;m++;
//
//   }
//
//   (*A)[m] = '\0';
//   *A_len = m;
//
//   (*B) += m;
//   *B_len = str_len - m;
//
//   //printf("str_len=%lu\nA=%s,%lu\nB=%s,%lu,\n",str_len,A,*A_len,B,*B_len);
// }

void parse_cmd_w_num(char* c, size_t str_len,
                                    size_t* A_len, char (*A)[20],
                                    size_t* F_len, char **F) {
  size_t m = 0;
  char* char_ptr = *A; //set to start of str

  //find where the switch betw the alphas & the float num happens
  while (isalpha(*char_ptr) && m < str_len) {
    char_ptr++;m++;}

  (*A)[m] = '\0';
  *A_len = m;

  (*F) += m;
  *F_len = str_len - m;
}


int main() {

  char str[] = "AB123"; size_t str_len = 5;

  char A[20]; size_t A_len;
  char* B = str; size_t B_len;

  puts("Copying str into A and B...");
  strcpy(A, str);

  puts("Parsing...");
  parse(str_len,&A,&A_len,&B,&B_len);


  puts("Printing results...");
  printf("str_len=%lu\nA=%s,%lu\nB=%s,%lu,\n",str_len,A,A_len,B,B_len);

  return 0;
}
