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

  yyin = fopen(config.mpf_file, "rb");

  if(config.hmhis_to_file) {
    hmhis = init_file(config.hmhis_json,"[");
    setvbuf(hmhis, NULL, _IONBF, 0);  // disable buffering
  }
  tl = init_file(config.track_list_csv,"track_index,Ax,Ay,Az,Bx,By,Bz");

  h = init_hashmap();

  yyparse ();

  if(config.hmhis_to_file) {close_hmhis();}
  fclose(tl);
  return 0;
}
