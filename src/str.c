
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "str.h"


int str_init(string *s){
    if ((s->str = (char*) malloc(BLOCK_SIZE*sizeof(char))) == NULL)
        return ERROR_TRANS;
    s->str[0] = '\0';
    s->len = 0;
    s->size = BLOCK_SIZE;
    return SUCCESS;
}

void str_free(string *s){
    free(s->str);
    s->str = NULL; //not initialized state
    s->len = -1; //not initialized state
    s->size= 0; //not initialized state
}

void str_reinit(string *s){
    s->str[0] = '\0';
    s->len = 0;
}

int str_add_char(string *s1, char c){
    if (s1->len + 1 >= s1->size){
        if ((s1->str = (char*) realloc(s1->str, (s1->len + BLOCK_SIZE)*sizeof(char))) == NULL)
            return ERROR_TRANS;
        s1->size = s1->len + BLOCK_SIZE;
    }
    s1->str[s1->len] = c;
    s1->len++;
    s1->str[s1->len] = '\0';
    return SUCCESS;
}

string c_str_to_str(const char* s){
    char c;
    int i = 0;
    string str;
    str_init(&str);

    if(s == NULL)
        return str;

    while((c = s[i++]) != '\0'){
        if(str_add_char(&str,c) != SUCCESS){
            str_free(&str);
            return str;
        }
    }
    return str;
}

int str_copy(string *s1, const string *s2){
    if (s2->len >= s1->size){
        s1->str = (char*)realloc(s1->str, (s2->len + 1)*sizeof(char));
        if (s1->str == NULL)
            return ERROR_TRANS;
        s1->size = s2->len + 1;
    }
    strcpy(s1->str, s2->str);
    s1->len = s2->len;
    return SUCCESS;
}

int str_cmp(const string *s1, const string *s2){
    return strcmp(s1->str, s2->str);
}

int str_cmp_c_str(const string *s1, const char* s2){
    return strcmp(s1->str, s2);
}

char* str_to_c_str(const string *s){
    return s->str;
}

size_t str_len(const string *s){
    return s->len;
}

int str_concat(string* s1, const char* s2, int len){
    if (s1->len + len >= s1->size){
        if ((s1->str = (char*) realloc(s1->str, (s1->len + len + BLOCK_SIZE)*sizeof(char))) == NULL)
            return ERROR_TRANS;
        s1->size = s1->len + len + BLOCK_SIZE;
    }
    for(int i = 0; i < (int)len; i++){
        s1->str[s1->len] = s2[i];
        s1->len++;
    }
    s1->str[s1->len] = '\0';

    return SUCCESS;

}