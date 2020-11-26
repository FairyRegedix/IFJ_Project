
#ifndef IFJ_PARSER_H
#define IFJ_PARSER_H

#endif //IFJ_PARSER_H

#include "scanner.h"
#include "symtable.h"


typedef struct parser_info{
    int error_code;
    int internal_error;

    symbol_table st; //global symbol table  - only functions
    st_stack_t* local_st; //stack of symbol tables for currently proccessed function, current scope always first
    token_t* token; //active token
    token_list_t token_list; //list of tokens
    st_item* in_function; //pointer to the function which definition we are currently parsing
    int scope; //current scope number

    string left_side_vars_types; // v1,v2,...,v_n = ...
    string right_side_exp_types; // ... = exp1,exp2,...,exp_n | call params types
    st_item* function_called; // ... = f()

    //possibly more stuff coming

}parser_info;


int get_next_token(parser_info* p);
int first_pass(parser_info* p);
int parser();
