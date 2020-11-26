//generator



#include "generator.h"

//finished functions
void generate_header(){
   
    printf(".IFJcode20\n");

    printf("JUMP $$main\n");
}

void generate_start_of_main(){
    printf("LABEL $$main # main body\n");
    printf("PUSHFRAME\n");
}

void generate_end_of_main(){
    printf("POPFRAME\n");
    printf("CLEARS\n");
}

void gen_defvar(char* id_of_variable){
    printf("DEFVAR LF@%s\n", id_of_variable);
}

void gen_retval(data_type type){
    char* value = gen_var_value(type);
    printf("DEFVAR LF@%retval\n");
    printf("MOVE LF@retval %s\n", value);
}

void gen_move_to_defvar(char* id_of_variable, data_type type){
    char* value = gen_var_value(type);
    printf("MOVE LF@%s %s\n", id_of_variable, value);
}

char* gen_var_value(data_type type){
    char *return_value;
    switch (type)
    {
    case type_int:
        return_value = "int@0";
        break;
    case type_float:
        return_value = "float@0.0";
        break;
    case type_bool:
        return_value = "bool@false";
        break;
    case type_str:
        return_value = "string@";
        break;
    default:
        break;
    }
    return return_value;

}

void gen_call(char* function){
    printf("CALL $%s\n", function);
}

void gen_return(){
    printf("RETURN");
}

void gen_WRITE(char* s1){
    printf("WRITE LF%s", s1);
}

void gen_LABEL_start(char* label){
    printf("LABEL $%s", label);
    printf("PUSHFRAME");
}

void gen_LABEL_end(){
    printf("POPFRAME");
    printf("RETURN");
}

void gen_start_of_function(char* function){
    printf("#Start of function\n");
    gen_LABEL_start(function);
}

void gen_end_of_function(){
    printf("#End of function\n");
    printf("\n");
    gen_LABEL_end();
}

void gen_if_start(char* label, int id){
    printf("JUMPIFNEQ $%s$if$%i$else\n", label, id);
}

void gen_if_else(char* label, int id){
    printf("JUMP $%s$if$%i$end\n", label, id);
    printf("LABEL $%s$if$%i$else\n", label, id);
}

void gen_if_end(char* label, int id){
    printf("LABEL $%s$if$%i$end\n",label, id);
}

void gen_while_start(char* label, int id){
    printf("LABEL $%s$while$%i$start\n", label, id);
}

void gen_while_end(char* label, int id){
    printf("JUMP $%s$while$%i$start\n", label, id);
    printf("LABEL $%s$while$%i$end\n", label, id);
}

void gen_JUMP(char* destination){
    printf("JUMP $%s", destination);
}

//end of finished functions

void gen_pushs(char* s1){
    printf("PUSHS %s\n", s1);
}

void gen_pops(char* s1){
    printf("POPS %s", s1);
}

void gen_clears(){
    printf("CLEARS");
}

void_gen_adds(){

}

void gen_add(char* s1, char* s2, char* s3){
    printf("ADD TF@%s @%s %s", s1, s2, s3);
}

void gen_sub(char* s1, char* s2, char* s3){
    printf("SUB TF@%s @%s %s", s1, s2, s3);
}

void gen_mul(char* s1, char* s2, char* s3){
    printf("MUL TF@%s @%s %s", s1, s2, s3);
}

void gen_div(char* s1, char* s2, char* s3){
    printf("DIV TF@%s @%s %s", s1, s2, s3);
}

void gen_idiv(char* s1, char* s2, char* s3){
    printf("IDIV TF@%s @%s %s", s1, s2, s3);
}


void gen_LT(char* s1, char* s2, char* s3){
    printf("LT %s %s %s", s1, s2, s3);
}

void gen_GT(char* s1, char* s2, char* s3){
    printf("GT %s %s %s", s1, s2, s3);
}

void gen_EQ(char* s1, char* s2, char* s3){
    printf("EQ %s %s %s", s1, s2, s3);
}

//miesto pre zasobnikove verzie LTS/GTS/EQS

void gen_AND(char* s1, char* s2, char* s3){
    printf("AND %s %s %s", s1, s2, s3);
}

void gen_OR(char* s1, char* s2, char* s3){
    printf("OR %s %s %s", s1, s2, s3);
}

void gen_NOT(char* s1, char* s2, char* s3){
    printf("NOT %s %s %s", s1, s2, s3);
}

void gen_INT2FLOAT(char* s1, char* s2){
    printf("INT2FLOAT %s %s", s1, s2);
}

void gen_FLOAT2INT(char* s1, char* s2){
    printf("FLOAT2INT %s %s", s1, s2);
}

void gen_INT2CHAR(char* s1, char* s2){
    printf("INT2CHAR %s %s", s1, s2);
}

void gen_STRI2INT(char* s1, char* s2){
    printf("STRI2INT %s %s", s1, s2);
}

//miesto pre zasobnikove verzie INT2FLOATS/FLOAT2INTS/INT2CHARS/STRI2INTS

void gen_READ(char* s1, char* s2){
    printf("READ %s %s", s1, s2);
}



void gen_CONCAT(char* s1, char* s2, char* s3){
    printf("CONCAT %s %s %s", s1, s2, s3);
}

void gen_STRLEN(char* s1, char* s2){
    printf("STRLEN %s %s", s1, s2);
}

void gen_GETCHAR(char* s1, char* s2, char* s3){
    printf("GETCHAR %s %s %s", s1, s2, s3);
}

void gen_SETCHAR(char* s1, char* s2, char* s3){
    printf("SETCHAR %s %s %s", s1, s2, s3);
}

void gen_TYPE(char* s1, char* s2){
    printf("TYPE %s %s", s1, s2);
}





void gen_EXIT(char* s1){
    printf("EXIT %s", s1);
}

void gen_BREAK(){
    printf("BREAK");
}

void gen_DPRINT(char* s1){
    printf("DPRINT %s", s1);
}


void gen_EOL(){
    printf("\n");
}
