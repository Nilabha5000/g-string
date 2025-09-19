#include "d_string.h"
#include <string.h>
#include <errno.h>
#include <stdio.h>
d_string *create_dstring(const char *str){
    d_string *res = (d_string*)malloc(sizeof(d_string));
    if(str == NULL){
      res->length = 0;
      res->cap = 16;
      res->is_short = true;
      res->buffer[0] = '\0';
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
d_string *d_str_clone(d_string * dstr){
      if(dstr == NULL) return NULL;
    d_string *clone = create_dstring(d_str_to_string(dstr));

    return clone;
}
//it destroys the dstring
void destroy_dstring(d_string *dstr){
    if(dstr == NULL){
        perror("string is not allocated\nso cannot destroy string");
        return;
    }
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
char *d_str_to_string(d_string*dstr){
    if(dstr == NULL){
        perror("string is not allocated");
        return NULL;
    }
    //check if the string is short then return buffer
    if(dstr->is_short)
       return dstr->buffer;
    
    return dstr->data;
}
//it appends a single charecter 
void add_char(d_string *dstr, const char c){
       
    if(dstr == NULL){
        perror("string is not allocated");
        return;
    }

    if(dstr->is_short && dstr->length < 15){
           dstr->buffer[dstr->length] = c;
           dstr->length++;
           dstr->buffer[dstr->length] = '\0';
           
    }
    // check if it exceeds the capacity then extend it.
    else if(dstr->length >= dstr->cap-1){
        dstr->cap += 16;
        char *newStr = (char*)malloc(dstr->cap);
        if(dstr->is_short){
            dstr->is_short = false;
            strcpy(newStr,dstr->buffer);
            dstr->data = newStr;
            dstr->data[dstr->length] = c;
            dstr->length++;
            dstr->data[dstr->length] = '\0';
        }
        else{
          strcpy(newStr,dstr->data);
          free(dstr->data);
          dstr->data = newStr;
        
          dstr->data[dstr->length] = c;
          dstr->length++;
          dstr->data[dstr->length] = '\0';
        }
        
    }
    
    else{
        dstr->data[dstr->length] = c;
        dstr->length++;
        dstr->data[dstr->length] = '\0';
    }

}
// it concatenate dstring with simple string
void add_str(d_string *dstr , const char * str){
    if(dstr == NULL){
        perror("string is not allocated");
        return;
    }
    if(str == NULL) return;
    int n = strlen(str);

    for(int i = 0; i < n; ++i){
        add_char(dstr, str[i]);
    }
}
//this function converts upper case to lower case letters
void to_lower(d_string *dstr){
    if(dstr == NULL){
        perror("string is not allocated");
        return;
    }
      
    char *s = d_str_to_string(dstr); 
    for (size_t i = 0; i < dstr->length; i++)
    {
        //check for if the element is an alphabet and also not lowercase
        if(isalpha(s[i]) && !islower(s[i]))
           s[i] += 32;
    }
    
}
//this function converts lower case to upper case
void to_upper(d_string *dstr){
     if(dstr == NULL){
        perror("string is not allocated");
        return;
    }
      
    char *s = d_str_to_string(dstr); 
    for (size_t i = 0; i < dstr->length; i++)
    {
        //check for if the element is an alphabet and also not uppercase
        if(isalpha(s[i]) && !isupper(s[i]))
           s[i] -= 32;
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