#include <stdio.h>
#include "parser.tab.h"

Color map_color(const char* expr_str);

int main(void) {
    Color c = (Color){0,0,0};

    const char *expr1 = "0==00 -> {0,255,0};";
    const char *expr2 = "1<2 -> {0,0,255};";
    const char *expr3 = "(1 > 50 || 1<50) -> {60,40,0};";

    c = map_color(expr1);
        printf("'%s' mapped to {%u,%u,%u}\n", expr1, c.r, c.g, c.b);
    c = map_color(expr2);
        printf("'%s' mapped to {%u,%u,%u}\n", expr2, c.r, c.g, c.b);
    c = map_color(expr3);
        printf("'%s' mapped to {%u,%u,%u}\n", expr3, c.r, c.g, c.b);

    return 0;
}
