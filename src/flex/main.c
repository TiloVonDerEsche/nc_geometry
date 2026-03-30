#include <stdio.h>
#include "helper.h"

extern Config config;
extern int debug;
extern int print_strs;
extern strfloat_t* h;

extern FILE* hmhis;
extern FILE* tl;

extern void label_finder(FILE* fp);

extern FILE* yyin;
extern int yyparse();

int
main(int argc, char *argv[])
{
  // Read the config file
  if (read_config("config.txt", &config) != 0) {
    return 1;
  }
  puts("\nRead values from config.txt:");
  // Print the config.txt values to verify
  printf("mpf_file=%s\n", config.mpf_file);
  printf("track_list_csv=%s\n", config.track_list_csv);

  debug = config.debug_prints;
  print_strs = config.print_strs;

  h = init_hashmap();
  FILE* mpf = fopen(config.mpf_file, "rb");
  if (mpf == NULL) {
    fprintf(stderr, "Error: Could not open %s (in read mode)!\
    \n\rDoes that file exist?\n",config.mpf_file);
    return -1;
  }

  //--------Preprocessor for Labels
  label_finder(mpf);
  rewind(mpf);
  print_hashmap(h,stdout);
  //--------Bison Interpreter
  yyin = mpf;

  if(config.hmhis_to_file) {
    hmhis = init_file(config.hmhis_json,"[");
    setvbuf(hmhis, NULL, _IONBF, 0);  // disable buffering
  }
  tl = init_file(config.track_list_csv,"Track_id,A.x,A.y,A.z,B.x,B.y,B.z,\
  laser_power,machine_speed,coll_vec.x,coll_vec.y,coll_vec.z,hradius,vradius");

  yyparse();

  //printf("%lu tracks written to %s!\n",tid,config.track_list_csv);
  if(config.hmhis_to_file) {close_hmhis();}
  fclose(tl);
  return 0;
}
