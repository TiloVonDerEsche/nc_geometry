#include <stdio.h>
#include "parser.tab.h"

Color map_color(const char* expr_str);

void test_color_config(char* str) {
  Color c = (Color){0,0,0};
  c = map_color(str);
  printf("'%s' mapped to {%u,%u,%u}\n", str, c.r, c.g, c.b);
}

/*
Can't handle:
(1 > 50 || 1<50)
*/

int main(void) {
    test_color_config("1<2 -> {0,0,255};");
    test_color_config("((1 > 50) || (1<50)) -> {60,40,0};");
    test_color_config("0==00 -> {0,255,0};");
    test_color_config("0 || 1 -> {0,1,0};");
    test_color_config("1 && 1 -> {0,1,1};");
    //test_color_config("0 |& 1 -> {0,1,0};");

    test_color_config("1>2 -> {0,0,255};");
    test_color_config("((1 > 50) || (51<50)) -> {60,40,0};");
    test_color_config("0==01 -> {0,255,0};");
    test_color_config("0 && 1 -> {0,1,0};");
    test_color_config("0 || 0 -> {0,1,1};");
    return 0;
}
