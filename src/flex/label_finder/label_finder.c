#include <stdio.h>
#include "./../helper.h"

extern int lflex(void);
extern FILE *lfin;

void label_finder(FILE *f) {
    lfin = f;  
    lflex();   
}
