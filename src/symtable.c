//
// Created by Frantisek on 04.11.2020.
//

#include <stdlib.h>
#include "str.h"
#include "symtable.h"
#include "error.h"



//djb2 algorithm
unsigned long hash(char* str){

    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}


void st_init(symbol_table* st_ptr){
    if(st_ptr != NULL){
        for(int i = 0; i < ST_SIZE; i++){
            (*st_ptr)[i] = NULL;
        }
    }
    else
        return;
}

st_item* st_item_alloc(int* error_code,const string* key, const item_type type){
    st_item* item = malloc(sizeof(st_item));
    if(item == NULL){
        *error_code = ERROR_TRANS;
        return NULL;
    }
    else{
        if((*error_code = st_item_init(item,key,type)) != SUCCESS){
            free(item)
            return NULL;
        }
        else
            return item;
    }
}
int st_item_init(st_item* item, const string* key, const item_type type){
    int error_code;

    if((error_code = str_init(&item->key)) != SUCCESS){
            free(item);
            return error_code;
    }
    
    if((error_code = str_copy(&item->key,key)) != SUCCESS){
        str_free(&item->key);
        free(item);
        return error_code;
    }
    item->next = NULL;
    item->data.type = type;
    item->data.defined = false;
    item->data.built_in = false;
    
    if(type == type_function){//function
        if((error_code = str_init(&item->data.as.function.params)) != SUCCESS){
                str_free(&item->key);
                free(item);
                return error_code;
        }
        if((error_code = str_init(&item->data.as.function.ret_types)) != SUCCESS){
            str_free(&item->data.as.function.params);
            str_free(&item->key);
            free(item);
            return error_code;
        }
    }
    else{//variable
        item->data.as.variable.value_type = -1;
        item->data.as.variable.value.int_value = 0;
    }
    
    return error_code;
}

void st_item_free(st_item* item){
    if(item == NULL) return;
    if(item->data.type == type_function){
        str_free(&item->data.as.function.params);
        str_free(&item->data.as.function.ret_types);
    }
    else if(item->data.type == type_variable && item->data.as.variable.value_type == type_str){
        str_free(&item->data.as.variable.value.string_value);
    }
    else{
        ;
    }
    str_free(&item->key);
    free(item);
}

st_item* st_get_item(const symbol_table* st, const string* key){
    unsigned long hash_code = hash(key->str) % ST_SIZE;
    st_item* item = (*st)[hash_code];
    while(item != NULL && str_cmp(&item->key,key)){
        item=item->next;
    }
    return item; //NULL or desired item
}


bool st_search(const symbol_table *st, const string* key) {
    st_item* item = st_get_item(st,key);
    if(item != NULL)
        return true;
    else
        return false;
}


bool st_insert(symbol_table* st, st_item* item){
        unsigned long hash_code = hash(item->key.str) % ST_SIZE;
        bool ret = st_del_item(st,&item->key); //delete item (doesn't matter if item exists or not)
        st_item* old_item = (*st)[hash_code];
        //insert to the front
        item->next = old_item;
        (*st)[hash_code] = item;
        return ret;
}

bool st_del_item(symbol_table* st, const string *key){
    unsigned long hash_code = hash(key->str) % ST_SIZE;
    st_item* del_item;
    del_item = (*st)[hash_code];
    if(del_item == NULL){//item doesn't exist
        return false;
    }
    else{//collision or item found
        st_item* prev_item = NULL;
        while(del_item != NULL && str_cmp(key,&del_item->key)){
            prev_item = del_item;
            del_item = del_item->next;
        }
        if(del_item != NULL){//item found
            if(prev_item == NULL)//item found at the front
                (*st)[hash_code] = del_item->next;
            else                //item found somewhere in the chain
                prev_item->next = del_item->next;

            st_item_free(del_item);
            return true;
        }
        else{//item doesn't exist
            return false;
        }
    }
}

void st_dispose(symbol_table* st){
    st_item* item;
    st_item* del_item;
    for(int i = 0; i < ST_SIZE; i++){
        item = (*st)[i];
        while(item != NULL){
            del_item = item;
            item = item->next;
            st_item_free(del_item);
        }
    }
    //free(st); not on heap currently
}
