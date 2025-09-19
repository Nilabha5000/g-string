#ifndef D_STRING_H
#define D_STRING_H
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
struct d_string{
    size_t cap; //for capacity
    union 
    {
        char *data;
        char buffer[16];
    };
    bool is_short;
    size_t length;
};

typedef struct d_string d_string;


d_string *create_dstring(const char*);
d_string *d_str_clone(d_string *);
void destroy_dstring(d_string *);
size_t length(d_string *);
size_t capacity(d_string *);
void add_char(d_string *, const char);
void to_upper(d_string *);
void to_lower(d_string *);
bool d_str_compare_str(d_string *, const char *);
bool d_str_compare_d_str(d_string * , d_string*);
void add_str(d_string * , const char *);
d_string* add_dstr(d_string * , d_string *);
char *d_str_to_string(d_string*);

#endif