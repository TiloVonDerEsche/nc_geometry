#include <stdio.h>
#include "parser.tab.h"

//mutate color attr of t
//access other attrs for bool eval
Color map_color(const char* expr_str, Track* t);
void apply_color_config(FILE* fp, Track* t_ptr);
/*
TODO
void map_color(FILE* fp, Track* t);
Read color_config.txt line by line
Call yyterminate after the first line mapped succesfully to a color

One could alternativly mix the colors of the matching lines
x < 150 -> {255,0,0};
y < 150 -> {0,255,0};
z < 150 -> {0,0,255};
So 3D Point: (100,100,300) gets color {255,255,0}

One could even interpolate the colors
*/

void test_color_config(char* str, Track* t) {
  printf("T_color before: {%u,%u,%u}\n", t->color.r, t->color.g, t->color.b);
  map_color(str, t);
  printf("'%s' mapped to {%u,%u,%u}\n", str, t->color.r, t->color.g, t->color.b);
}

int main(void) {
  Track t = (Track){12,
             2.3, 11.2, -7.9,
             5.12, 17.10, -0.9,
             200, 1750,
             1,
             (Color){0,0,0},
             0.5,0.5
            };

  const char* filename="color_config.txt";
  printf("Opening %s...\n", filename);
  FILE* file = fopen(filename, "r");
  if (file == NULL) {
      fprintf(stderr, "Error: Could not open file %s\n", filename);
      return FAILURE;
  }

  apply_color_config(file,&t);

  return SUCCESS;
}
