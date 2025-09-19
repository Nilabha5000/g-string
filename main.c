#include "d_string.h"
#include <stdio.h>
int main(){
    d_string *ds = create_dstring("Hello,world this is a great day");
    d_string *ds1 = d_str_clone(ds);
    printf("this is ds string :%s\n",d_str_to_string(ds));
    printf("this is ds1 string :%s\n",d_str_to_string(ds1));
    if(d_str_compare_str(ds1,"Hello,world this is a kreat day"))
        printf("!strings matched\n");
    else
      printf("!strings not matched\n");
    printf("%s\n",d_str_to_string(ds));
    printf("\nfinal ds string length %ld\n", length(ds));
    printf("final ds string capacity %ld\n", capacity(ds));
    printf("\nfinal ds1 string length %ld\n", length(ds1));
    printf("final ds1 string capacity %ld\n", capacity(ds1));
    destroy_dstring(ds);
    destroy_dstring(ds1);
    return 0;
}
