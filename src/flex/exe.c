#define NOMINAL 0
#define NOT_FOUND -1

extern void label_finder(FILE* fp);
extern FILE* yyin;
extern int yyparse();

int exec(char* fpath) {
  FILE* ncf = fopen(fpath, "rb");
  if (ncf == NULL) {
    fprintf(stderr, "Error: Could not open %s (in read mode)!\
    \n\rDoes that file exist?\n",fpath);
    return NOT_FOUND;
  }

  //--------Preprocessor for Labels
  label_finder(ncf);
  rewind(ncf);
  printf("Labels of '%s':\n",fpath);print_hashmap(h,stdout);
  //--------Bison Interpreter
  yyin = ncf;
  yyparse();
  printf("Hashmap after executing '%s':\n",fpath);print_hashmap(h,stdout);

  return NOMINAL;
}
