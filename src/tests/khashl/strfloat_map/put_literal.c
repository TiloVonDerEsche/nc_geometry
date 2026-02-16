// To run this program: `./this_prog abc bc abc a bc`
#include <stdio.h>
#include <string.h>
#include "../../../src/khashl.h"
//KHASHL_SET_INIT(KH_LOCAL, strmap_t, strmap, const char*, kh_hash_str, kh_eq_str)
KHASHL_MAP_INIT(KH_LOCAL, strfloat_t, strfloat, const char*, float, kh_hash_str, kh_eq_str)

int main(int argc, char *argv[])
{
    strfloat_t *h;
    int i, absent;
    khint_t k;
    h = strfloat_init();

    strfloat_put(h, "literal", &absent);
    strfloat_put(h, "literal", &absent);
    strfloat_put(h, "literal", &absent);
    strfloat_put(h, "literal1", &absent);
    strfloat_put(h, "literal2", &absent);


    //print_hashmap
    kh_foreach(h, k) {
      printf("%s: %f\n", kh_key(h, k), kh_val(h, k));
    }


    printf("# of distinct words: %d\n", kh_size(h));
    strfloat_destroy(h);
    return 0;
}
