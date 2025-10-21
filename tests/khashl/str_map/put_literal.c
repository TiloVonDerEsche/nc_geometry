// To run this program: `./this_prog abc bc abc a bc`
#include <stdio.h>
#include <string.h>
#include "../../../src/khashl.h"
KHASHL_SET_INIT(KH_LOCAL, strmap_t, strmap, const char*, kh_hash_str, kh_eq_str)

int main(int argc, char *argv[])
{
    strmap_t *h;
    int i, absent;
    khint_t k;
    h = strmap_init();

    strmap_put(h, "literal", &absent);
    strmap_put(h, "literal", &absent);
    strmap_put(h, "literal", &absent);
    strmap_put(h, "literal1", &absent);
    strmap_put(h, "literal2", &absent);


    //print_hashmap
    kh_foreach(h, k) {
      printf("%s\n", kh_key(h, k));
    }


    printf("# of distinct words: %d\n", kh_size(h));
    strmap_destroy(h);
    return 0;
}
