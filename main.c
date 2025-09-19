#include "d_string.h"
#include <stdio.h>

int main() {
    d_string *ds = create_dstring(NULL);
    d_string *ds1 = d_str_clone(ds);

    printf("ds string : \"%s\"\n", d_str_to_string(ds));
    printf("ds1 string: \"%s\"\n", d_str_to_string(ds1));

    d_str_add_str(ds, "Hello, world this is a great day!");
    if (d_str_compare_str(ds1, "Hello, world this is a great day!"))
        printf("strings matched!\n");
    else
        printf("strings not matched.\n");

    printf("ds after append: %s\n", d_str_to_string(ds));

    d_string *substr = d_str_sub_str(ds, 7, 11); // "world"
    if (substr)
        printf("substr : %s\n", d_str_to_string(substr));
    else
        printf("substr : NULL (out of range)\n");

    printf("\nfinal ds length = %zu\n", length(ds));
    printf("final ds capacity = %zu\n", capacity(ds));
    printf("final ds1 length = %zu\n", length(ds1));
    printf("final ds1 capacity = %zu\n", capacity(ds1));

    // cleanup
    destroy_dstring(substr);
    destroy_dstring(ds);
    destroy_dstring(ds1);

    return 0;
}
