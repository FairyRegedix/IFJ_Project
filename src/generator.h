/**
 * IFJ Projekt 2020
 *
 * Implementation of code generator
 *
 * @author <xhrmor00> Richard Hrmo
 * @author <xsabol03> František Sabol
 */

#ifndef IFJ_GENERATOR_H
#define IFJ_GENERATOR_H


#include "symtable.h"
#include "instruction_list.h"

/**
 * Generates instructions for assigning values to retvals
 * 
 * @param NumberOfReturns number of retvals
 * @param in_for True if in for, false otherwise
 */
void gen_set_retvals(int NumberOfReturns, bool in_for);

/**
 * Generates instructions for moving values from retvals into local variables
 * 
 * @param NumberOfVariables number of variables into which return values are assigned into
 */
void gen_assign_return(int NumberOfVariables);

/**
 * Generates instructions for creating Temporary Frame variables and assigning values to them before instruction for
 * 
 * @param NumberOfVariables number of variables to create and assign values into
 */
void gen_for_assign(int NumberOfVariables);

/**
 * Generates name of variable and adds it to the first position of string list Vars
 * 
 * @param var_name name of variable
 * @param scope number of scope
 */
void gen_add_to_vars(char *var_name, int scope);

/**
 * Adds string of instructions into string list Exps
 * 
 * @param exp String of instructions
 * @param in_for True if in for, false otherwise
 */
void gen_add_to_exp(char *exp, bool in_for);

/**
 * Generates instructions at the end of program & terminantes all string lists
 */
void close_generator();

/**
 * Pushes int to top of IntStack and increments ID
 */
void push_int();

/**
 * Pops int from top of stack
 */
void pop_int();

/**
 * Initialisate string lists and generates header of code and built in functions
 */
void generate_header();

/**
 * Generates variable
 * 
 * @param id id of variable
 * @param scope number of scope
 * @param in_for True if in for, false otherwise
 */
void gen_defvar(char* id,int scope, bool in_for);

/**
 * Generates retval
 * 
 * @param NumberOfReturns number of retvals to generate
 */
void gen_retvals(int number_of_return_values);

/**
 * Generates retvals
 * 
 * @param params pointer to string of parameters
 */
void gen_params(string* params);

/**
 * Generates parameters and initializes them
 * 
 * @param last pointer to struct token
 * @param local_st pointer to struct stack
 */
void gen_call_params(token_t *last, st_stack_t *local_st);

/**
 * Generates instructions to push value to the stack
 * 
 * @param type type of token
 * @param value pointer to string
 * @param local_st pointer to struct stack
 */
void gen_pushs_param(token_type type, string *value, st_stack_t *local_st);

/**
 * Initializes variables
 * 
 * @param NumberOfVariables number of variables to assign values into
 * @param in_for True if in for, false otherwise
 */
void gen_assign(int NumberOfVariables, bool in_for);

/**
 * Generates beginning of funcion for
 * 
 * @param expression string of expressions
 */
void gen_for_start(char *expression);

/**
 * Generates condition check in for function
 */
void gen_for_jump();

/**
 * Generates end of function for
 */
void gen_for_end();

/**
 * Generates beginning of call function
 * 
 * @param function name of function
 * @param count_of_vars number of input parameters 
 */
void gen_call_start(char* function, int count_of_vars);

/**
 * Generates function call
 * 
 * @param function name of function
 */
void gen_call(char* function);

/**
 * Generates beginning of label
 * 
 * @param label 
 */
void gen_LABEL_start(char* label);

/**
 * Generates end of Label
 */
void gen_LABEL_end();

/**
 * Generates beginning of function
 * 
 * @param function name of function
 */
void gen_start_of_function(char* function);

/**
 * Generates end of function
 */
void gen_end_of_function();

/**
 * Generates beginning of IF function
 * 
 * @param expression string of expressions
 */
void gen_if_start(char* expression);

/**
 * Generates else part of IF function
 */
void gen_else();

/**
 * Generates end of IF function
 */
void gen_if_end();

/**
 * Generates built in function inputs
 */
void gen_func_inputs();

/**
 * Generates built in function inputi
 */
void gen_func_inputi();

/**
 * Generates built in function inputf
 */
void gen_func_inputf();

/**
 * Generates built in function int2float
 */
void gen_func_int2float();

/**
 * Generates built in function float2int
 */
void gen_func_float2int();

/**
 * Generates built in function len
 */
void gen_func_len();

/**
 * Generates built in function substr
 */
void gen_func_substr();

/**
 * Generates built in function ord
 */
void gen_func_ord();

/**
 * Generates built in function chr
 */
void gen_func_chr();

/**
 * Generates built in function print
 */
void gen_func_print();


#endif //IFJ_GENERATOR_H