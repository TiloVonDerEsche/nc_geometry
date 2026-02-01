#include "lex.yy.c"
#include "grammar.tab.c"



int
main(int argc, char *argv[])
{
  Config config = {0};

  // Read the config file
  if (read_config("config.txt", &config) != 0) {
    return 1;
  }
  puts("\nRead values from config.txt:");
  // Print the config.txt values to verify
  printf("mpf_file=%s\n", config.mpf_file);
  printf("track_list_csv=%s\n", config.track_list_csv);

  yyin = fopen(config.mpf_file, "rb");

  if(hmhis_json) {hmhis = init_file("./data/hmhis.json","[");}
  tl = init_file("./data/track_list.csv","track_index,Ax,Ay,Az,Bx,By,Bz");

  h = init_hashmap();

  yyparse ();

  if(hmhis_json) {close_hmhis();}
  fclose(tl);
  return 0;
}
