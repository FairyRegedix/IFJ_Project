//generator



#include "generator.h"

void generate_header(){
   
    printf(".IFJcode20\n");

    printf("JUMP $$main\n");

    printf("LABEL $$main # main body\n");
    printf("PUSHFRAME\n");

}

void generate_end_of_main(){
    printf("POPFRAME\n");
    printf("CLEARS\n");
}

void gen_move(char* s1, char* s2){
    printf("MOVE LF@%s %s", s1, s2);
}

void gen_createframe(){
    printf("CREATEFRAME");
}

void gen_pushframe(){
    printf("PUSHFRAME");
}

void gen_popframe(){
    printf("POPFRAME");
}

void gen_defvar(char* id_of_variable){
    printf("DEFVAR LF@%s\n", id_of_variable);
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

void gen_pushs(char* s1){
    printf("PUSHS %s", s1);
}

void gen_pops(char* s1){
    printf("POPS %s", s1);
}

void gen_clears(){
    printf("CLEARS");
}

void gen_add(char* s1, char* s2, char* s3){
    printf("ADD %s %s %s", s1, s2, s3);
}

void gen_sub(char* s1, char* s2, char* s3){
    printf("SUB %s %s %s", s1, s2, s3);
}

void gen_mul(char* s1, char* s2, char* s3){
    printf("MUL %s %s %s", s1, s2, s3);
}

void gen_div(char* s1, char* s2, char* s3){
    printf("DIV %s %s %s", s1, s2, s3);
}

void gen_idiv(char* s1, char* s2, char* s3){
    printf("IDIV %s %s %s", s1, s2, s3);
}

// miesto pre zasobnikove verzie ADDS/SUBS/MULS/DIVS/IDIVS

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

void gen_WRITE(char* s1){
    printf("WRITE %s", s1);
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

void gen_LABEL(char* s1){
    printf("LABEL $%s", s1);
}

void gen_JUMP(char* s1){
    printf("JUMP %s", s1);
}

void gen_JUMPIFEQ(char* s1, char* s2, char* s3){
    printf("JUMPIFEQ %s %s %s", s1, s2, s3);
}

void gen_JUMPIFNEQ(char* s1, char* s2, char* s3){
    printf("JUMPIFNEQ %s %s %s", s1, s2, s3);
}

//miesto pre zasobnikove verzie JUMPIFEQS/JUMPIFNEQS

void gen_EXIT(char* s1){
    printf("EXIT %s", s1);
}

void gen_BREAK(){
    printf("BREAK");
}

void gen_DPRINT(char* s1){
    printf("DPRINT %s", s1);
}


//pomocne funkcie
void gen_EOL(){
    printf("\n");
}

void gen_start_of_function(char* function){
    printf("#Start of function\n");
    gen_LABEL(function);
    printf("PUSHFRAME");
}

void gen_end_of_function(char *function){
    printf("#End of function\n");
    printf("\n");
    printf("POPFRAME");
    printf("RETURN");
}





void codeGenerator(token_type type, token_t *token, st_item item){
    
    switch (type)
    {
    case TOKEN_ID:

        break;
    case TOKEN_ASSIGN: 

        break;
    case TOKEN_DEFINITION:

        break;
    case TOKEN_PRINT:

        break;
    case TOKEN_STRING:

        break;
    case TOKEN_LEN:

        break;
    case TOKEN_ORD:

        break;
    case TOKEN_CHR:

        break;
    case TOKEN_PACKAGE:

        break;
    case TOKEN_UNDERSCORE:

        break;
    case TOKEN_STR:

        break;
    case TOKEN_INTEGER:

        break;
    case TOKEN_INT:

        break;
    case TOKEN_FLOAT:

        break;
    case TOKEN_FLOAT64:

        break;
    case TOKEN_EOL:
        gen_EOL();
        break;
    case TOKEN_EOF:
    
        break;
    case TOKEN_COMMA:

        break;
    case TOKEN_SEMICOLON:

        break;
    case TOKEN_LBRACKET:

        break;
    case TOKEN_RBRACKET:

        break;
    case TOKEN_LCURLY:

        break;
    case TOKEN_RCURLY:

        break;
    
    case TOKEN_ADD:
    case TOKEN_SUB:
    case TOKEN_MUL:
    case TOKEN_DIV:


        break;
    
    case TOKEN_EQL:
    case TOKEN_NEQ:
    case TOKEN_LTE:
    case TOKEN_GTE:
    case TOKEN_LT:
    case TOKEN_GT:

        break;
    
    case TOKEN_NOT:
    case TOKEN_AND:
    case TOKEN_OR:


        break;

    case TOKEN_IF:

        break;
    case TOKEN_ELSE:

        break;    
    case TOKEN_FOR:
        break;
    
    case TOKEN_FUNC:
        break;
        
    case TOKEN_INPUTB:

        break;
    case TOKEN_INPUTI:

        break;
    case TOKEN_INPUTS:

        break;
    case TOKEN_INPUTF:

        break;
    case TOKEN_SUBSTR:
        
        break;
    case TOKEN_RETURN:

        break;
    default:
        break;
    }

}
