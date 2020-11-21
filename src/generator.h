#ifndef GENERATOR_H
#define GENERATOR_H



#include "libraries.h"
#include "error.h"
#include "symtable.h"
#include "scanner.h"
#include "str.h"

int codeGenerator();
int add_to_code();
int generate_header();
int generate_end_of_main();

#endif //GENERATOR_H