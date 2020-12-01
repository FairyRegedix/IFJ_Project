//generator



#include "generator.h"

void generate_code_end(){
    printf("LABEL $$final_end\n");
    gen_EXIT("int@0\n");
}
void generate_header(){
   
    printf(".IFJcode20\n");
    printf("DEFVAR GF@EXPRESULT\n");
    printf("JUMP $main\n");
    printf("JUMP $$final_end\n");
}

void generate_start_of_main(){//nepotrebne
    printf("LABEL $$main # main body\n");
    printf("PUSHFRAME\n");
}

void generate_end_of_main(){//nepotrebne
    printf("POPFRAME\n");
    printf("CLEARS\n");
}

void gen_defvar(char* id_of_variable){
    printf("DEFVAR LF@%s\n", id_of_variable);
}

void gen_retval(data_type type){
    char* value = gen_var_value(type);
    printf("DEFVAR LF@retval\n");
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
        return_value = "float@0x0p+0";
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
    printf("RETURN\n");
}

void gen_WRITE(char* s1){
    printf("WRITE LF%s\n", s1);
}

void gen_LABEL_start(char* label){
    printf("LABEL $%s\n", label);
    printf("PUSHFRAME\n");
}

void gen_LABEL_end(){
    printf("POPFRAME\n");
    printf("RETURN\n");
}

void gen_start_of_function(char* function){
    printf("#Start of function\n");
    gen_LABEL_start(function);
}

void gen_end_of_function(){
    printf("#End of function\n");
    gen_LABEL_end();
}

void gen_if_start(char *label, int id) {
    printf("#IF $%s$if$%i\n",label, id);
    printf("JUMPIFEQ $%s$if$%i$else GF@EXPRESULT bool@false\n", label, id); //GF@EXPRESULT
}

void gen_if_else(char* label, int id){
    printf("JUMP $%s$if$%i$end\n", label, id);
    printf("LABEL $%s$if$%i$else\n", label, id);
}

void gen_if_end(char* label, int id){
    printf("LABEL $%s$if$%i$end\n",label, id);
}

void gen_while_start(char* label, int id){
    printf("#WHILE $%s$while$%i\n",label, id);
    printf("LABEL $%s$while$%i$start\n", label, id);
}

void gen_while_end(char* label, int id, char* truefalse){
    printf("JUMPIFEQ $%s$while$%i$end bool@true %s\n", label, id, truefalse);
    printf("JUMP $%s$while$%i$start\n", label, id);
    printf("LABEL $%s$while$%i$end\n", label, id);
}

void gen_JUMP(char* destination){
    printf("JUMP $%s\n", destination);
}

void gen_JUMPIFEQ(char* destination, char* s1, char *s2){
    printf("JUMPIFEQ $%s %s %s\n", destination, s1, s2);
}

void gen_JUMPIFNEQ(char* destination, char* s1, char *s2){
    printf("JUMPIFEQ $%s %s %s\n", destination, s1, s2);
}

void gen_stack_GTE(){
    printf("DEFVAR LF@$GTE$param1\n");
    printf("DEFVAR LF@$GTE$param2\n");
    printf("POPS LF@$GTE$param1\n");
    printf("POPS LF@$GTE$param2\n");
    printf("JUMPIFNEQ $EQS LF@$GTE$param1 LF@$GTE$param2\n");
    printf("PUSHS bool@true\n");
    printf("JUMP $LTEGTEEND\n");
    printf("LABEL $EQS\n");
    printf("PUSHS LF@$GTE$param2\n");
    printf("PUSHS LF@$GTE$param1\n");
    printf("GTS\n");
    printf("LABEL $GTEEND\n");
}

void gen_stack_LTE(){
    printf("DEFVAR LF@$LTE$param1\n");
    printf("DEFVAR LF@$LTE$param2\n");
    printf("POPS LF@$LTE$param1\n");
    printf("POPS LF@$LTE$param2\n");
    printf("JUMPIFNEQ $EQS LF@$LTE$param1 LF@$LTE$param2\n");
    printf("PUSHS bool@true\n");
    printf("JUMP $LTEGTEEND\n");
    printf("LABEL $EQS\n");
    printf("PUSHS LF@$LTE$param2\n");
    printf("PUSHS LF@$LTE$param1\n");
    printf("LTS\n");
    printf("LABEL $LTEEND\n");
}

void gen_stack_instructions(stack_instruction instruction){
    switch(instruction){
        case ADDS:
            printf("ADDS\n");
            break;
        case SUBS:
            printf("SUBS\n");
            break;
        case MULS:
            printf("MULS\n");
            break;
        case DIVS:
            printf("DIVS\n");
            break;
        case IDIVS:
            printf("IDIVS\n");
            break;
        case LTS:
            printf("LTS\n");
            break;
        case GTS:
            printf("GTS\n");
            break;
        case EQS:
            printf("EQS\n");
            break;
        case ANDS:
            printf("ANDS\n");
            break;
        case ORS:
            printf("ORS\n");
            break;
        case NOTS:
            printf("NOTS\n");
        default:
            break;
    }
}

void gen_func_inputs(){
    printf("LABEL $inputs\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@$retval\n");
    printf("MOVE LF@$retval nil@nil\n");
    printf("DEFVAR LF@param1\n");
    printf("MOVE LF@param1 LF@$1\n");
    printf("DEFVAR LF@errorCheck\n");
    printf("TYPE LF@errorCheck LF@param1\n");
    printf("JUMPIFNEQ $ERROR string@string LF@errorCheck\n");
    printf("DEFVAR LF@strlen\n");
    printf("STRLEN LF@strlen LF@param1\n");
    printf("SUB LF@strlen LF@strlen int@1\n");
    printf("DEFVAR LF@getchar\n");
    printf("GETCHAR LF@getchar LF@param1 LF@strlen\n");
    printf("JUMPIFNEQ $ENDOFINPUTS LF@getchar string@\\010\n");
    printf("SETCHAR LF@param1 LF@strlen string@\\000\n");
    printf("LABEL $ENDOFINPUTS\n");
    printf("MOVE LF@$retval LF@param1\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
    printf("LABEL $ERROR\n");
    printf("EXIT int@1\n");
}

void gen_func_inputi(){

}

void gen_func_inputf(){

}

void gen_func_print(){

}

void gen_func_int2float(){

}

void gen_func_float2int(){

}

void gen_func_len(){

}

void gen_func_substr(){

}

void gen_func_ord(){

}

void gen_func_chr(){
    
}

void gen_pushs(char* s1){
    printf("PUSHS %s\n", s1);
}

void gen_pops(char* s1){
    printf("POPS %s\n", s1);
}

void gen_clears(){
    printf("CLEARS\n");
}

void gen_add(char* s1, char* s2, char* s3){
    printf("ADD TF@%s @%s %s\n", s1, s2, s3);
}

void gen_sub(char* s1, char* s2, char* s3){
    printf("SUB TF@%s @%s %s\n", s1, s2, s3);
}

void gen_mul(char* s1, char* s2, char* s3){
    printf("MUL TF@%s @%s %s\n", s1, s2, s3);
}

void gen_div(char* s1, char* s2, char* s3){
    printf("DIV TF@%s @%s %s\n", s1, s2, s3);
}

void gen_idiv(char* s1, char* s2, char* s3){
    printf("IDIV TF@%s @%s %s\n", s1, s2, s3);
}

void gen_LT(char* s1, char* s2, char* s3){
    printf("LT %s %s %s\n", s1, s2, s3);
}

void gen_GT(char* s1, char* s2, char* s3){
    printf("GT %s %s %s\n", s1, s2, s3);
}

void gen_EQ(char* s1, char* s2, char* s3){
    printf("EQ %s %s %s\n", s1, s2, s3);
}

void gen_AND(char* s1, char* s2, char* s3){
    printf("AND %s %s %s\n", s1, s2, s3);
}

void gen_OR(char* s1, char* s2, char* s3){
    printf("OR %s %s %s\n", s1, s2, s3);
}

void gen_NOT(char* s1, char* s2, char* s3){
    printf("NOT %s %s %s\n", s1, s2, s3);
}

void gen_INT2FLOAT(char* s1, char* s2){
    printf("INT2FLOAT %s %s\n", s1, s2);
}

void gen_FLOAT2INT(char* s1, char* s2){
    printf("FLOAT2INT %s %s\n", s1, s2);
}

void gen_INT2CHAR(char* s1, char* s2){
    printf("INT2CHAR %s %s\n", s1, s2);
}

void gen_STRI2INT(char* s1, char* s2){
    printf("STRI2INT %s %s\n", s1, s2);
}

void gen_READ(char* s1, char* s2){
    printf("READ %s %s\n", s1, s2);
}

void gen_CONCAT(char* s1, char* s2, char* s3){
    printf("CONCAT %s %s %s\n", s1, s2, s3);
}

void gen_STRLEN(char* s1, char* s2){
    printf("STRLEN %s %s\n", s1, s2);
}

void gen_GETCHAR(char* s1, char* s2, char* s3){
    printf("GETCHAR %s %s %s\n", s1, s2, s3);
}

void gen_SETCHAR(char* s1, char* s2, char* s3){
    printf("SETCHAR %s %s %s\n", s1, s2, s3);
}

void gen_TYPE(char* s1, char* s2){
    printf("TYPE %s %s\n", s1, s2);
}

void gen_EXIT(char* s1){
    printf("EXIT %s\n", s1);
}

void gen_BREAK(){
    printf("BREAK\n");
}

void gen_DPRINT(char* s1){
    printf("DPRINT %s\n", s1);
}

void gen_EOL(){
    printf("\n");
}
