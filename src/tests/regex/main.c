#include "find_labels.h"

int
main(int argc, char *argv[])
{
  //h = init_hashmap();
  FILE* mpf = fopen("./test.mpf", "rb");
  //--------Preprocessor for Labels
  find_labels(mpf);

  //--------Bison Interpreter
  return 0;
}
