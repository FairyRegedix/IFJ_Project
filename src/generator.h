#ifndef GENERATOR_H
#define GENERATOR_H


#include "libraries.h"
#include "symtable.h"
#include "scanner.h"
#include "str.h"
#include "instruction_list.h"

typedef enum{
    ADDS = 0,
    SUBS = 1,
    MULS = 2,
    DIVS = 3,
    IDIVS = 4,
    LTS = 5,
    GTS = 6,
    EQS = 7,
    ANDS = 8,
    ORS = 9,
    NOTS = 10,
}stack_instruction;



void gen_set_retvals(int NumberOfReturns, bool in_for);
void gen_assign_return(int NumberOfVariables);
void gen_for_assign(int NumberOfVariables);

void gen_add_to_vars(char *var_name, int scope);
void gen_add_to_exp(char *exp, bool in_for);

void close_generator();
void push_int();
void pop_int();
void generate_header();
void gen_defvar(char* id,int scope, bool in_for);
void gen_retvals(int number_of_return_values);
void gen_params(string* params);
void gen_call_params(token_t *last, st_stack_t *local_st);
void gen_pushs_param(token_type type, string *value, st_stack_t *local_st);
void gen_assign(int NumberOfVariables, bool in_for);
void gen_for_start(char *expression);
void gen_for_jump();
void gen_for_end();
void gen_call_start(char* function, int count_of_vars);
void gen_call(char* function);
void gen_LABEL_start(char* label);
void gen_LABEL_end();
void gen_start_of_function(char* function);
void gen_end_of_function();
void gen_if_start(char* expression);
void gen_else();
void gen_if_end();

void gen_func_inputs();
void gen_func_inputi();
void gen_func_inputf();

void gen_func_int2float();
void gen_func_float2int();

void gen_func_len();
void gen_func_substr();
void gen_func_ord();
void gen_func_chr();
void gen_func_print();


#endif //GENERATOR_H