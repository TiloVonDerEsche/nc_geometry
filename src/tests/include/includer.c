#include <stdio.h>

extern int yylex(void);
extern FILE *yyin;

void includer(FILE *f) {
    yyin = f;
    yylex();
}

void main() {
    FILE* ncf = fopen("main.nc", "rb");
    includer(ncf);
}
