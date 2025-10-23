#include "khashl.h"
KHASHL_MAP_INIT(KH_LOCAL,
  strfloat_t, strfloat,
  const char*, float,
  kh_hash_str, kh_eq_str)

void print_hashmap(strfloat_t* h, FILE* destination) {
  //Print all variables from hashmap
  khint_t k;
  fprintf(destination,"-----\n");
  kh_foreach(h, k) {
      if (k < kh_end(h)) {  // Check if found (k != end iterator)
          fprintf(destination,"%s=%f\n", kh_key(h, k), kh_val(h, k));
      }
  }
  fflush(destination);
}

//write keys at the end of nc program to first line of hmhis.csv
void print_key_to_hmhis(strfloat_t* h) {

}

//print one line of hmhis, which is fnums seperated by ','
void print_values_to_hmhis(strfloat_t* h, FILE* hmhis) {
  //Print all variables from hashmap
  khint_t k;
  kh_foreach(h, k) {
      if (k < kh_end(h)) {  // Check if found (k != end iterator)
          printf("Printing k=%d: value=%f\n", k, kh_val(h, k));
          fprintf(hmhis,"%f,", kh_val(h, k));
      }
  }

  fprintf(hmhis,"\n");
}
