// To run this program: `./this_prog abc bc abc a bc`
#include <stdio.h>
#include <string.h>
#include "../../src/khashl.h"
KHASHL_SET_INIT(KH_LOCAL, strmap_t, strmap, const char*, kh_hash_str, kh_eq_str)

int main(int argc, char *argv[])
{
    strmap_t *h;
    int i, absent;
    h = strmap_init();
    for (i = 1; i < argc; ++i)
        strmap_put(h, argv[i], &absent);
    printf("# of distinct words: %d\n", kh_size(h));
    strmap_destroy(h);
    return 0;
}
