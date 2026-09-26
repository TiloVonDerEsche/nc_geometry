#include <stdio.h>
#include "parser.tab.h"

//mutate color attr of t
//access other attrs for bool eval
void map_color(const char* expr_str, Track* t);

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


    puts("All True:");
    test_color_config("11<t_id -> {0,0,255};",&t);
    test_color_config("((1 > 50) || (1<50)) -> {60,40,0};",&t);
    test_color_config("0==00 -> {0,255,0};",&t);
    test_color_config("0 || 1 -> {0,1,0};",&t);
    test_color_config("1 && 1 -> {0,1,1};",&t);
    //test_color_config("0 |& 1 -> {0,1,0};",&t);

    puts("All False:");
    test_color_config("1>2 -> {0,0,255};",&t);
    test_color_config("((1 > 50) || (51<50)) -> {60,40,0};",&t);
    test_color_config("0==01 -> {0,255,0};",&t);
    test_color_config("0 && 1 -> {0,1,0};",&t);
    test_color_config("0 || 0 -> {0,1,1};",&t);

    puts("Let's see:");
    test_color_config("-4<-3 -> {0,0,255};",&t);
    test_color_config("2<1<3 -> {60,40,0};",&t);
    test_color_config("3<2<1 -> {0,255,0};",&t);
    test_color_config("1+2   -> {0,1,0};",&t);
    test_color_config("1-1   -> {0,1,1};",&t);
    return 0;
}
