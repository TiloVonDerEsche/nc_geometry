void print_hashmap(strfloat_t* h) {
  //Print all variables from hashmap
  khint_t k;
  puts("-----");
  kh_foreach(h, k) {
      if (k < kh_end(h)) {  // Check if found (k != end iterator)
          printf("var %s=%f\n", kh_key(h, k), kh_val(h, k));
      }
  }
  puts("-----");
}

//write keys at the end of nc program to first line of hmhis.csv
void print_key_to_hmhis(strfloat_t* h) {
  printf("Opening: hmhis.csv in write mode...\n");
  FILE* hmhis = fopen("./data/hmhis.csv", "w");
  if (hmhis == NULL) {
      fprintf(stderr, "Error: Could not open hmhis.csv (in write mode)!\n");
      return;}


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
