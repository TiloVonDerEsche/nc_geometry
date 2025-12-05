#include "khashl.h"
KHASHL_MAP_INIT(KH_LOCAL,
  strfloat_t, strfloat,
  const char*, float,
  kh_hash_str, kh_eq_str)


//print in JSON object syntax
void print_hashmap(strfloat_t* h, FILE* destination) {
  //Print all variables from hashmap
  khint_t k;
  fprintf(destination,"{");

  kh_foreach(h, k) {
      if (k < kh_end(h)) {  // Check if found (k != end iterator)
          fprintf(destination,"\"%s\":\"%f\",", kh_key(h, k), kh_val(h, k));
      }
  }

  //only works if destination is seekable, a pipe f.e. is not
  fseek(destination, -1, SEEK_CUR); //move to pos of last redundant comma
  fprintf(destination,"},\n"); //replace last comma
}

void init_hashmap(strfloat_t* h) {
  khint_t k;
  int absent;

  h = strfloat_init();

  k = strfloat_put(h, "line", &absent);
  kh_key(h, k) = strdup("line");
  kh_val(h, k) = 123;

  k = strfloat_put(h, "laser", &absent);
  kh_key(h, k) = strdup("laser");

  kh_val(h, k) = -1.2;
  k = strfloat_put(h, "laser_power", &absent);
  kh_key(h, k) = strdup("laser_power");
  kh_val(h, k) = 3500;

  k = strfloat_put(h, "R1", &absent);
  kh_key(h, k) = strdup("R1");
  kh_val(h, k) = 456;

  print_hashmap(h, stdout);
}













































//csv nonsense

//print one line of hmhis, which is fnums seperated by ','
// void print_values_to_hmhis(strfloat_t* h, FILE* hmhis) {
//   //Print all variables from hashmap
//   khint_t k;
//   kh_foreach(h, k) {
//       if (k < kh_end(h)) {  // Check if found (k != end iterator)
//           printf("Printing k=%d: value=%f\n", k, kh_val(h, k));
//           fprintf(hmhis,"%f,", kh_val(h, k));
//       }
//   }
//
//   fprintf(hmhis,"\n");
// }
