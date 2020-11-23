#ifndef GENERATOR_H
#define GENERATOR_H



#include "libraries.h"
#include "error.h"
#include "symtable.h"
#include "scanner.h"
#include "str.h"

void gen_defvar(char* id_of_variable);
void gen_retval(data_type type);
void gen_move_to_defvar(char* id_of_variable, data_type type);
char* gen_var_value(data_type type);


void gen_call(char* s1);
void gen_return();
void gen_pushs(char* s1);
void gen_pops(char* s1);
void gen_clears();
void gen_add(char* s1, char* s2, char* s3);
void gen_sub(char* s1, char* s2, char* s3);
void gen_mul(char* s1, char* s2, char* s3);
void gen_div(char* s1, char* s2, char* s3);
void gen_idiv(char* s1, char* s2, char* s3);
void gen_LT(char* s1, char* s2, char* s3);
void gen_GT(char* s1, char* s2, char* s3);
void gen_EQ(char* s1, char* s2, char* s3);
void gen_AND(char* s1, char* s2, char* s3);
void gen_OR(char* s1, char* s2, char* s3);
void gen_NOT(char* s1, char* s2, char* s3);
void gen_INT2FLOAT(char* s1, char* s2);
void gen_FLOAT2INT(char* s1, char* s2);
void gen_INT2CHAR(char* s1, char* s2);
void gen_STRI2INT(char* s1, char* s2);
void gen_READ(char* s1, char* s2);
void gen_WRITE(char* s1);
void gen_CONCAT(char* s1, char* s2, char* s3);
void gen_STRLEN(char* s1, char* s2);
void gen_GETCHAR(char* s1, char* s2, char* s3);
void gen_SETCHAR(char* s1, char* s2, char* s3);
void gen_TYPE(char* s1, char* s2);

void gen_LABEL_start(char* s1);
void gen_LABEL_end();



void gen_JUMP(char* s1);
void gen_JUMPIFEQ(char* s1, char* s2, char* s3);
void gen_JUMPIFNEQ(char* s1, char* s2, char* s3);
void gen_EXIT(char* s1);
void gen_BREAK();
void gen_DPRINT(char* s1);


void gen_start_of_function(char* function);
void gen_end_of_function();

void gen_EOL();

void codeGenerator();
void generate_header();
void generate_start_of_main();
void generate_end_of_main();

#endif //GENERATOR_H