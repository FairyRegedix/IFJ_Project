//
// Created by Frantisek on 11.11.2020.
//

#include <stdio.h>
#include <stdlib.h>
#include "symtable.h"
#include "str.h"

//#define NAMES_NO 5

void st_print(symbol_table* st){
    st_item* item;
    int i = 0;
    int null_count = 0;
    for(; i < ST_SIZE; i++){
        item = (*st)[i];
        if(item != NULL){
            printf("item[%i]: %s\t",i,item->key.str);
            while(item->next!=NULL){
                item = item->next;
                printf("%s\t",item->key.str);
            }
            printf("\n");
        }
        else
            ++null_count;
    }
    printf("NULL appeared %i times.\n",null_count);
}

int main(){
    char names[7][10] = {{"xxx"},
                         {"x21"},
                         {"xac"},
                         {"x4"},
                         {"peter"},
                         {"xxx"},
                         {"rand"}};

    symbol_table st;
    st_init(&st);
    int error_code = 1;
    for(int i = 0; i < 7; i++){

        st_item* item = NULL;
        string s = c_str_to_str(names[i]);
        printf("Created \"%s\"\n",s.str);
        item = st_insert(&st, &s,type_function, &error_code);
        if(error_code != 0){
            str_free(&s);
            st_dispose(&st);
            return error_code;
        }
        if(item->data.defined)
            printf("Redefinition of \"%s\"\n",names[i]);
        else
            item->data.defined = true;

        str_free(&s);
    }
    string s1 = c_str_to_str("xxx");
    string s2 = c_str_to_str("peter");
    st_del_item(&st, &s1);
    st_del_item(&st, &s2);
    st_print(&st);
    st_dispose(&st);
    str_free(&s1);
    str_free(&s2);
    return 0;
}
