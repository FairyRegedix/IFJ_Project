#ifndef GENERATOR_H
#define GENERATOR_H



#include "libraries.h"
#include "error.h"
#include "symtable.h"
#include "scanner.h"
#include "str.h"

void gen_move(string s1, string s2);
void gen_createframe();
void gen_pushframe();
void gen_popframe();
void gen_defvar(string s1);
void gen_call(string s1);
void gen_return();
void gen_pushs(string s1);
void gen_pops(string s1);
void gen_clears();
void gen_add(string s1, string s2, string s3);
void gen_sub(string s1, string s2, string s3);
void gen_mul(string s1, string s2, string s3);
void gen_div(string s1, string s2, string s3);
void gen_idiv(string s1, string s2, string s3);
void gen_LT(string s1, string s2, string s3);
void gen_GT(string s1, string s2, string s3);
void gen_EQ(string s1, string s2, string s3);
void gen_AND(string s1, string s2, string s3);
void gen_OR(string s1, string s2, string s3);
void gen_NOT(string s1, string s2, string s3);
void gen_INT2FLOAT(string s1, string s2);
void gen_FLOAT2INT(string s1, string s2);
void gen_INT2CHAR(string s1, string s2);
void gen_STRI2INT(string s1, string s2);
void gen_READ(string s1, string s2);
void gen_WRITE(string s1);
void gen_CONCAT(string s1, string s2, string s3);
void gen_STRLEN(string s1, string s2);
void gen_GETCHAR(string s1, string s2, string s3);
void gen_SETCHAR(string s1, string s2, string s3);
void gen_TYPE(string s1, string s2);
void gen_LABEL(string s1);
void gen_JUMP(string s1);
void gen_JUMPIFEQ(string s1, string s2, string s3);
void gen_JUMPIFNEQ(string s1, string s2, string s3);
void gen_EXIT(string s1);
void gen_BREAK();
void gen_DPRINT(string s1);

void gen_EOL();

void codeGenerator();
void generate_header();
void generate_end_of_main();

#endif //GENERATOR_H