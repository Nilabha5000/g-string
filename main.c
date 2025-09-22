#include "d_string.h"
#include <stdio.h>

int main() {
    d_string *ds = create_dstring("");
    d_string *ds1 = d_str_clone(ds);

    printf("ds string : \"%s\"\n", d_str_to_string(ds));
    printf("ds1 string: \"%s\"\n", d_str_to_string(ds1));

    d_str_add_str(ds, "Hello, world this is a great day!");
    d_str_to_upper(ds);
    if (d_str_compare_str(ds, "HELLO, WORLD THIS IS A GREAT DAY!"))
        printf("strings matched!\n");
    else
        printf("strings not matched.\n");
    
    int x = d_str_find(ds,"GREAT");
    (x != -1) ?  printf("GREAT is found in ds string at pos %d\n",x) : printf("GREAT is not found in ds \n");
    d_str_add_str(ds, " Let's append more text.");
    d_str_add_str(ds, "Another string here."); 
    d_str_to_upper(ds);
    printf("ds after append: %s\n", d_str_to_string(ds));
    
    d_string *substr = d_str_sub_str(ds, 7, 11); // "world"
    if (substr)
        printf("substr : %s\n", d_str_to_string(substr));
    else
        printf("substr : NULL (out of range)\n");
    printf("ds at 7 = %c\n", d_str_at(ds, 7));
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
