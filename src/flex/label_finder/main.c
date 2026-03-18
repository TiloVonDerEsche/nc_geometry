#include "lex.yy.c"
#include "./../grammar.tab.c"


int main(int argc, char* argv[]) {
  if (read_config("config.txt", &config) != 0) {
    return 1;
  }
  puts("\nRead values from config.txt:");
  // Print the config.txt values to verify
  printf("mpf_file=%s\n", config.mpf_file);

  h = init_hashmap();
  yyin = fopen(config.mpf_file, "rb");

  yyparse();

  print_hashmap(h,stdout);
  return 0;
}
