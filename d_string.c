#include "d_string.h"
#include <string.h>
#include <errno.h>
#include <stdio.h>

#define EXTEND_EXPONENTIAL 1
#define EXTEND_BY_SIZE 2
//these function are private
bool d_str_extend(d_string *,int , int);
char *d_str_to_string_private(d_string*);
int *compute_LPS(const char *);
//it creates a dstring from simple string
//if the input string is NULL then it creates an empty dstring
//it returns the pointer to the created dstring
//it is the responsibility of the user to free the allocated memory
//using destroy_dstring function    
d_string *create_dstring(const char *str){
    d_string *res = (d_string*)malloc(sizeof(d_string));
    if(res == NULL){
         perror("failed to create dstring");
         return NULL;
    }
    if(str == NULL || strcmp(str,"") == 0){
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
       if(res->data == NULL){
           perror("failed to create dstring");
           free(res);
           return NULL;
       }
       strcpy(res->data , str);
       res->is_short = false;
    }

    return res;
}
//this function extends the dstring
//it returns true if the extension is successful otherwise false
//extend_type = EXTEND_EXPONENTIAL : it doubles the capacity
//extend_type = EXTEND_BY_SIZE : it extends the capacity by n
bool d_str_extend(d_string *dstr, int extend_type, int n){
      if(dstr == NULL) return false;

      size_t size;
      if(extend_type == EXTEND_EXPONENTIAL)
           size = dstr->cap*2;
      else if(extend_type == EXTEND_BY_SIZE)
           size = dstr->cap+n+1;

      char *newBlock = (char*)malloc(size);

      if(newBlock == NULL)
           return false;
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
      newBlock[dstr->length] = '\0';
      dstr->cap = size;
    return true;
}
//it returns the charecter at index
//if index is out of range it returns '\0'
//if dstr is NULL it returns '\0'   
char d_str_at(d_string *dstr , int index){
    if(dstr == NULL) return '\0';
    //check for index range
    if( index < 0 || index >= length(dstr)) return '\0';
    //get the actual string
    char *actual_str = d_str_to_string_private(dstr);
    //return the charecter at index
    return actual_str[index];
}

void d_str_set(d_string *dstr , int index , char c){
    if(dstr == NULL) return;
    if( index < 0 || index >= length(dstr)) return;

    char *actual_str = d_str_to_string_private(dstr);
    actual_str[index] = c;
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
//this function is safe to use
//only for read purpose
const char *d_str_to_string(d_string*dstr){
    if(dstr == NULL) return NULL;
    //check if the string is short then return buffer
    if(dstr->is_short)
       return (const char*)dstr->buffer;
    
    return (const char *)dstr->data;
}
//this function is private and unsafe to use
//it returns the actual string buffer
//it can be used to modify the string
char *d_str_to_string_private(d_string*dstr){
    if(dstr == NULL) return NULL;
    //check if the string is short then return buffer
    if(dstr->is_short)
       return dstr->buffer;
    
    return dstr->data;
}
d_string *d_str_sub_str(d_string *dstr , int s , int e){
    if(dstr == NULL) return NULL;
    if(s < 0 || s > e || e >= length(dstr)) return NULL;
     
    size_t len = e-s+1;
    char *actual_str = d_str_to_string_private(dstr);
    char *temp = (char*)malloc(len+1);
    if(temp == NULL)
        return NULL;
    memmove(temp, actual_str+s, len);
    temp[len] = '\0';
    d_string *res = create_dstring(temp);
    free(temp);
    return res;
}
//it appends a single charecter 
void d_str_add_char(d_string *dstr, const char c){
       
    if(dstr == NULL) return;
     
    char *actual_str = NULL;
    if(dstr->length >= dstr->cap-1){
        if(!d_str_extend(dstr,EXTEND_EXPONENTIAL,0))
        {
            perror("failed to extend memory !");
            return;
        }
    }

    // Get the actual string buffer
    // This is safe now because we ensured enough capacity above
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

    if(length(dstr)+n >= capacity(dstr)-1){
        if(!d_str_extend(dstr,EXTEND_BY_SIZE,n)){
            perror("failed to extend memory !");
            return;
        }
    }
    char *actual_str = d_str_to_string_private(dstr);
    memmove(actual_str+dstr->length , str , n);
    dstr->length += n;
    actual_str[dstr->length] = '\0';
}
void d_str_add_d_str(d_string *dstr1 , d_string *dstr2){
    if(dstr1 == NULL) return;
    if(dstr2 == NULL) return;
    if(length(dstr1)+length(dstr2) >= capacity(dstr1)-1){
        if(!d_str_extend(dstr1,EXTEND_BY_SIZE,length(dstr2))){
            perror("failed to extend memory !");
            return;
        }
    }
    char *str = d_str_to_string_private(dstr2);
    char *actual_str = d_str_to_string_private(dstr1);
    memmove(actual_str+dstr1->length,str,dstr2->length);
    dstr1->length += dstr2->length;
    actual_str[dstr1->length] = '\0';
}
//this function converts upper case to lower case letters
void d_str_to_lower(d_string *dstr){
    if(dstr == NULL) return;
    
    char *s = d_str_to_string_private(dstr); 
    for (size_t i = 0; i < dstr->length; i++)
    {
        s[i] = tolower((unsigned char)s[i]);
    }
    
}
//this function converts lower case to upper case
void d_str_to_upper(d_string *dstr){
     if(dstr == NULL) return;
      
    char *s = d_str_to_string_private(dstr); 
    for (size_t i = 0; i < dstr->length; i++)
    {
           s[i] = toupper((unsigned char)s[i]);
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

void d_str_rev(d_string *dstr){

    char temp = '\0';
    int i = 0 , j = length(dstr)-1;
    while(i < j){
        temp = d_str_at(dstr,i);
        d_str_set(dstr,i,d_str_at(dstr,j));
        d_str_set(dstr,j,temp);
        i++;
        j--;
    }
}
int *compute_LPS(const char *pattern){
    size_t n = strlen(pattern);
    int *lps = (int*)malloc(sizeof(int)*n);
    memset(lps,0,n);
    
    int len = 0;

    int i = 1;

    while(i < n){
        if(pattern[i] == pattern[len]){
            len++;
            lps[i] = len;
            i++;
        }
        else{
            if(len != 0){
                len = lps[len-1];
            }
            else{
                lps[i] = 0;
                i++;
            }
        }
    }
    return lps;
}

int d_str_find(d_string *dstr , const char *pattern){

    int i = 0;
    int j = 0;
    int *lps = compute_LPS(pattern);
    int pat_size = strlen(pattern);
    while(i < length(dstr)){
        if(d_str_at(dstr,i) == pattern[j]){
               i++;
               j++;
               if(j == pat_size){
                     free(lps);
                     return i-j;
               }
        }
        else{
            if(j != 0)
                j = lps[j-1];
            else
              i++;
        }
    }
    free(lps);
    return -1;
}

void d_str_insert(d_string *dstr , const char *str , int index){
    if(dstr == NULL) return;
    size_t n = strlen(str);
    //check for index range
    if(index < 0 || index > length(dstr)) return;
    //extend the dstring if required
    if(length(dstr) + n >= capacity(dstr))
        if(!d_str_extend(dstr ,EXTEND_BY_SIZE,n)) return;
    //get the actual string
    char *actual_str = d_str_to_string_private(dstr);
    // shifting the string to right
    int i = length(dstr)-1;
    // making space for inserting a string
    while(i >= index){
        actual_str[i+n] = actual_str[i];
        i--;
    }
    // inserting the string
    memmove(actual_str+index, str,n);
    //updating the length of dstring
    dstr->length += n;
    //null terminating the string
    actual_str[dstr->length] = '\0';
}