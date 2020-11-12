
#ifndef IFJ_PROJECT_SYMTAB_H
#define IFJ_PROJECT_SYMTAB_H

#include <stdlib.h>
#include <stdbool.h>
#include "str.h"

#define ST_SIZE 2333

typedef enum data_type{
    type_int,
    type_float,
    type_str,
    type_bool,
} data_type;

typedef enum item_type{
    type_function,
    type_variable,
} item_type;

typedef struct function_t{
    string params;
    string ret_types;
} function_t; //function signature

typedef struct variable_t{
    data_type value_type; //data type of the variable
    union{
        int int_value; //also for bool value
        double float_value;
        string string_value;
    }value;
} variable_t; //variable signature

typedef struct item_data{
    item_type type; // function | var ?
    bool defined;
    bool built_in;
    union{
        function_t function;
        variable_t variable;
    }as;
}item_data;


typedef struct item{
    string key;  //identifier
    item_data data; //another information about symbol
    struct item* next;  //pointer to the next item in the list (chaining collisions)
}st_item;

typedef st_item* symbol_table[ST_SIZE];


/* djb2 hashing algorithm
 * @param   s   :string to be hashed
 * @return  returns hash(index to symbol table)*/
unsigned long hash(char* s);

/*
 * Function to initialize symbol table
 * @param   st  :pointer to symbol table to be initialized
 * */
void st_init(symbol_table* st);



/*
 * Helper function for st_item_init
 * For more info see st_item_init*/
st_item* st_item_alloc(int* error_code, const string* key, const item_type type);
/*
 * Function to initialize new symbol table item.
 * @pre     Item pointer has to be a valid pointer to memory.
 * @post    Memory space for some item attributes will be allocated.
 * @param   item :pointer to item
 * @param   key  :id of function/variable
 * @param   type :type_function or type_variable?
 * @return  SUCCESS or ERROR_TRANS(internal error)
 * */
int st_item_init(st_item* item, const string* key, const item_type type);

/*
 *@param    st  :a valid pointer to a symbol table
 *@param    key :a valid pointer to a key of an item to lookup
 *@return   returns pointer to an existing item or NULL otherwise*/
st_item* st_get_item(const symbol_table* st, const string* key);

/*
 *@param    st  :a valid pointer to a symbol table
 *@param    key :a valid pointer to a key of an item to lookup
 *@return   true - found, false - not found*/
bool st_search(const symbol_table* st, const string* key);


void st_item_free(st_item* item);
bool st_insert(symbol_table* st, st_item* item);
bool st_del_item(symbol_table* st, const string *key);

void st_dispose(symbol_table* st);




#endif //IFJ_PROJECT_SYMTAB_H
