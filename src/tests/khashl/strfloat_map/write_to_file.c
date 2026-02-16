// To run this program: `./this_prog abc bc abc a bc`
#include <stdio.h>
#include <string.h>
#include "../../../src/khashl_helper.h"
#include "../../../src/khashl.h"

int main(int argc, char *argv[])
{
    FILE* hmhis = fopen("./hmhis_test.csv", "w");
    if (hmhis == NULL) {
        fprintf(stderr, "Error: Could not open hmhis_test.csv (in write mode)!\n");
        return -1;
    }
    fprintf(hmhis,"TEST!!!???\n");

    strfloat_t *h;
    int i, absent;
    khint_t k;
    h = strfloat_init();

    strfloat_put(h, "literal", &absent);
    print_hashmap(h,hmhis);

    strfloat_put(h, "literal", &absent);
    kh_val(h, k) = 2;
    print_hashmap(h,hmhis);


    k = strfloat_put(h, "literal", &absent);
    kh_val(h, k) = 3;
    print_hashmap(h,hmhis);

    kh_val(h, k) = 123;

    k = strfloat_put(h, "literal1", &absent);
    kh_val(h, k) = 222;

    k = strfloat_put(h, "literal2", &absent);
    kh_val(h, k) = 333;

    print_hashmap(h,hmhis);


    //print_hashmap
    kh_foreach(h, k) {
      printf("%s: %f\n", kh_key(h, k), kh_val(h, k));
    }


    printf("# of distinct words: %d\n", kh_size(h));
    fclose(hmhis);
    strfloat_destroy(h);
    return 0;
}
