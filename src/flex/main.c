#include "lex.yy.c"
#include "grammar.tab.c"



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

  h = init_hashmap();
  FILE* mpf = fopen(config.mpf_file, "rb");
  //--------Preprocessor for Labels
  find_labels(h,mpf);

  //--------Bison Interpreter
  yyin = mpf;

  if(config.hmhis_to_file) {
    hmhis = init_file(config.hmhis_json,"[");
    setvbuf(hmhis, NULL, _IONBF, 0);  // disable buffering
  }
  tl = init_file(config.track_list_csv,"Track_id,A.x,A.y,A.z,B.x,B.y,B.z,\
  laser_power,machine_speed,coll_vec.x,coll_vec.y,coll_vec.z,hradius,vradius");
  //setvbuf(tl, NULL, _IONBF, 0); takes longer, errors occur more often.



  yyparse ();

  printf("%lu tracks written to %s!\n",tid,config.track_list_csv);
  if(config.hmhis_to_file) {close_hmhis();}
  fclose(tl);
  return 0;
}
