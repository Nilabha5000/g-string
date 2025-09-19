#include "d_string.h"
#include <string.h>
#include <errno.h>
#include <stdio.h>

//these function are private
void d_str_extend(d_string *);
char *d_str_to_string_private(d_string*);

d_string *create_dstring(const char *str){
    d_string *res = (d_string*)malloc(sizeof(d_string));
    if(str == NULL){
      res->length = 0;
      res->cap = 16;
      res->is_short = true;
      res->buffer[0] = '\0';
      res->data = NULL;
      return res;
    }
    res->length = strlen(str);
    
    if(res->length < 16){
        res->cap = 16;
        strncpy(res->buffer,str,16);
        res->buffer[res->length] = '\0';
        res->is_short = true;
    }
    else{
        res->cap = res->length+16;
       res->data = (char*)malloc(res->cap);
       strcpy(res->data , str);
       res->is_short = false;
    }

    return res;
}
void d_str_extend(d_string *dstr){
      if(dstr == NULL) return;
      dstr->cap *= 2;
      char *newBlock = (char*)malloc(dstr->cap);
      if(dstr->is_short){
          memcpy(newBlock,dstr->buffer,dstr->length+1);
          dstr->is_short = false;
          dstr->data = newBlock;
      }
      else{
         memcpy(newBlock,dstr->data,dstr->length+1);
         free(dstr->data);
         dstr->data = newBlock;
      }
}
//this function returens a clone of dstr
d_string *d_str_clone(d_string * dstr){
      if(dstr == NULL) return NULL;
    d_string *clone = create_dstring(d_str_to_string(dstr));

    return clone;
}
//it destroys the dstring
void destroy_dstring(d_string *dstr){
    if(dstr == NULL) return;
    //if the dstring is not short then deallocate the data string
    if(!dstr->is_short){
        free(dstr->data);
    }
    //freeing the dstr
    free(dstr);
}

size_t length(d_string *dstr){
    if(dstr == NULL) return (size_t)-1;
    return dstr->length;
}
size_t capacity(d_string *dstr){
    if(dstr == NULL) return (size_t)-1;
    return dstr->cap;
}
//it converts dstring to simple string
const char *d_str_to_string(d_string*dstr){
    if(dstr == NULL) return NULL;
    //check if the string is short then return buffer
    if(dstr->is_short)
       return (const char*)dstr->buffer;
    
    return (const char *)dstr->data;
}
char *d_str_to_string_private(d_string*dstr){
    if(dstr == NULL) return NULL;
    //check if the string is short then return buffer
    if(dstr->is_short)
       return dstr->buffer;
    
    return dstr->data;
}
d_string *d_str_sub_str(d_string *dstr , int s , int e){
    if(dstr == NULL) return NULL;
    if(s < 0 || s > e) return NULL;

    if(s >= 0 && e <= dstr->length-1){
          char *actual_str = d_str_to_string_private(dstr);
          d_string *res = create_dstring(NULL); // empty d_string
          for(int i = s; i <= e; ++i)
             d_str_add_char(res,actual_str[i]);
         return res;
    }
    
    return NULL;
}
//it appends a single charecter 
void d_str_add_char(d_string *dstr, const char c){
       
    if(dstr == NULL) return;
     
    char *actual_str = NULL;
    if(dstr->length >= dstr->cap-1)
        d_str_extend(dstr);

    // Get the actual string buffer
    // This is safe now because we ensured enough capacity 
    // 
    actual_str = d_str_to_string_private(dstr);
    actual_str[dstr->length] = c;
    dstr->length++;
    actual_str[dstr->length] = '\0';

}
// it concatenate dstring with simple string
void d_str_add_str(d_string *dstr , const char * str){
    if(dstr == NULL)return;
    if(str == NULL) return;
    int n = strlen(str);

    for(int i = 0; i < n; ++i){
        d_str_add_char(dstr, str[i]);
    }
}
//this function converts upper case to lower case letters
void d_str_to_lower(d_string *dstr){
    if(dstr == NULL) return;
      
    char *s = d_str_to_string_private(dstr); 
    for (size_t i = 0; i < dstr->length; i++)
    {
        s[i] = (unsigned char)tolower(s[i]);
    }
    
}
//this function converts lower case to upper case
void d_str_to_upper(d_string *dstr){
     if(dstr == NULL) return;
      
    char *s = d_str_to_string_private(dstr); 
    for (size_t i = 0; i < dstr->length; i++)
    {
           s[i] = (unsigned char)toupper(s[i]);
    }
}

bool d_str_compare_str(d_string *dstr , const char *str){
    if(dstr == NULL)
        return false;
    if(str == NULL)
        return false;

    size_t n = strlen(str);

    if(length(dstr) != n) return false;
    if(strcmp(d_str_to_string(dstr) , str) == 0) return true;

    return false;
}
bool d_str_compare_d_str(d_string *dstr1 , d_string *dstr2){
    if(dstr1 == NULL) return false;
    if(dstr2 == NULL) return false;

    if(length(dstr1) != length(dstr2)) return false;

    return strcmp(d_str_to_string(dstr1),d_str_to_string(dstr2)) == 0;
}