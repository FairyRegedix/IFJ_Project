//generator



#include "generator.h"

unsigned int GTEcounter = 0;
unsigned int LTEcounter = 0;
unsigned int ID = 0;
StringList ListOfStrings;

int IntStack[1000];
int top = -1;

void push_int(){
    top = top + 1;
    IntStack[top] = ID;
    ID++;
}

void pop_int(){
    top = top - 1;
}

void close_generator(){
    DisposeListString(&ListOfStrings);
}

void generate_header(){
    InitListString(&ListOfStrings);
    printf(".IFJcode20\n");
    printf("DEFVAR GF@EXPRESULT\n");
    printf("DEFVAR GF@CONCATRESULT\n\n");
    printf("MOVE GF@BLOCK$COUNTER int@0\n");
    gen_func_inputs();
    gen_func_inputf();
    gen_func_inputi();
    gen_func_int2float();
    gen_func_float2int();
    printf("CALL $main\n");
    printf("JUMP $$final_end\n");
}

void gen_defvar(char* id,int scope, bool in_for){
    if(in_for){
        printf("MOVE LF@%s$%d nil@nil",id, scope);
    }else
    {
        printf("DEFVAR LF@%s$%d",id, scope);
    }
    
}

void gen_retvals(int number_of_return_values){
    for(int i = 1; i <= number_of_return_values; i++)
        printf("DEFVAR LF@retval$%d\n", i);
}

void gen_move_to_defvar(char* id_of_variable, char* value){
    printf("MOVE LF@%s %s\n", id_of_variable, value);
}

void gen_params(string* params){

    printf("DEFVAR LF@");
    for(int i = 0; i < params->len-1; i++){
        if(params->str[i] != '#'){
            printf("%c", params->str[i]);
        }else
        {
            printf("$0\nDEFVAR LF@");
        }
    }

    char tmpchar[params->len];
    int j = 0;
    for(int i = params->len-1 ; i >= 0; i--){
        if(params->str[i] != '#'){
            tmpchar[j] = params->str[i];
            j++;
        }else
        {
            tmpchar[j] = '\0';
            strrev(tmpchar);
            printf("POPS LF@%s$0\n", tmpchar);
            j = 0;
        }
    }
}

void gen_call_params(token_t* last){
    token_t* prev = last;
    while(prev->type != TOKEN_LBRACKET){
        gen_pushs_param(prev->type, &prev->actual_value, 0);
        prev = prev->prev;
    }

}
void gen_pushs_param(token_type type, string *value, int scope){
    printf("PUSHS ");
    switch (type)
    {
    case TOKEN_INT:
        printf("int@%s\n", value->str);
        break;

    case TOKEN_FLOAT64:
        printf("float@%a\n", Str_to_Float(value));
        break;

    case TOKEN_ID:
        printf("%s$%d\n", value->str, scope);
        break; 

    case TOKEN_STRING:
        printf("string@%s\n", value->str);
        break;

    default:
        break;
    }

}

void gen_assign(int NumberOfVariables, StringList *Expressions, StringList *Variables){
    for(int j = 0; j < NumberOfVariables; j++){
        printf("%s", Expressions->First->data);
        DeleteFirstString(Expressions);
        printf("POPS LF@$tmp$%d", ID);
        push_int();
    }

    for(int i = 0; i < NumberOfVariables; i++){
        printf("MOVE LF@%s LF@$tmp%d",Variables->First->data, IntStack[top]);
        DeleteFirstString(Variables);
        pop_int();
    }

}

void gen_for_start(char *expression){
    printf("LABEL CHECK$FOR$%d\n", ID);
    InsertFirstString(&ListOfStrings, expression);
}

void gen_for_jump(){
    printf("PUSHS bool@true\n");
    printf("JUMPIFNEQS END$FOR$%d\n", ID);
    push_int();
}

void gen_for_end(){
    printf("%s", ListOfStrings.First->data);
    DeleteFirstString(&ListOfStrings);
    printf("JUMP CHECK$FOR$%d", IntStack[top]);
    printf("LABEL END$FOR$%d", IntStack[top]);
    pop_int();
}

void gen_call_start(char* function, int count_of_vars){
    printf("CREATEFRAME\n");
    printf("CLEARS\n");
    if(strcmp(function, "print") == 0){
        printf("DEFVAR TF@$0\n");
        printf("MOVE TF@$0 int@%d\n", count_of_vars);
    }
}

void gen_call(char* function){
    printf("CALL func$%s\n", function);
}

void gen_return(){
    printf("RETURN\n");
}

void gen_WRITE(char* s1){
    printf("WRITE LF%s\n", s1);
}

void gen_LABEL_start(char* label){
    printf("LABEL func$%s\n", label);
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

void gen_if_start(char* truefalse){
    printf("#IF $if$%d\n",ID);
    printf("JUMPIFEQ $if$%d$else bool@true %s\n",ID, truefalse);
    push_int();
}

void gen_if_else(){
    printf("JUMP $if$%d$end\n", IntStack[top]);
    printf("LABEL $if$%d$else\n", IntStack[top]);
}

void gen_if_end(){
    printf("LABEL $if$%d$end\n", IntStack[top]);
    pop_int();
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
    printf("DEFVAR LF@$GTE$param1$%d\n", GTEcounter);
    printf("DEFVAR LF@$GTE$param2$%d\n", GTEcounter);
    printf("POPS LF@$GTE$param1$%d\n", GTEcounter);
    printf("POPS LF@$GTE$param2$%d\n", GTEcounter);
    printf("PUSHS LF@$GTE$param2$%d\n", GTEcounter);
    printf("PUSHS LF@$GTE$param1$%d\n", GTEcounter);
    printf("GTS\n");
    printf("PUSHS LF@$GTE$param2$%d\n", GTEcounter);
    printf("PUSHS LF@$GTE$param1$%d\n", GTEcounter);
    printf("EQS\n");
    printf("ORS\n");
    GTEcounter++;
}

void gen_stack_LTE(){
    printf("DEFVAR LF@$LTE$param1$%d\n", LTEcounter);
    printf("DEFVAR LF@$LTE$param2$%d\n", LTEcounter);
    printf("POPS LF@$LTE$param1$%d\n", LTEcounter);
    printf("POPS LF@$LTE$param2$%d\n", LTEcounter);
    printf("PUSHS LF@$LTE$param2$%d\n", LTEcounter);
    printf("PUSHS LF@$LTE$param1$%d\n", LTEcounter);
    printf("GTS\n");
    printf("PUSHS LF@$LTE$param2$%d\n", LTEcounter);
    printf("PUSHS LF@$LTE$param1$%d\n", LTEcounter);
    printf("EQS\n");
    printf("ORS\n");
    LTEcounter++;
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
    printf("#FUNCTION INPUTS\n\n");
    printf("LABEL $inputs\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@$retval\n");
    printf("MOVE LF@$retval nil@nil\n");
    printf("DEFVAR LF@param1\n");
    printf("READ LF@param1 string\n");
    printf("DEFVAR LF@errorCheck\n");
    printf("TYPE LF@errorCheck LF@param1\n");
    printf("JUMPIFNEQ $ERROR$INPUTS string@string LF@errorCheck\n");
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
    printf("LABEL $ERROR$INPUTS\n");
    printf("EXIT int@1\n\n");
}

void gen_func_inputi(){
    printf("#FUNCTION INPUTI\n\n");
    printf("LABEL $inputi\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@$retval\n");
    printf("MOVE LF@$retval nil@nil\n");
    printf("DEFVAR LF@param$1\n");
    printf("DEFVAR LF@error$check\n");
    printf("READ LF@param$1 int\n");
    printf("TYPE LF@error$check LF@param$1\n");
    printf("JUMPIFNEQ $ERROR$INPUTI string@int LF@error$check\n");
    printf("MOVE LF@$retval LF@param$1\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
    printf("LABEL $ERROR$INPUTI\n");
    printf("EXIT int@1\n\n");
}

void gen_func_inputf(){
    printf("#FUNCTION INPUTF\n\n");
    printf("LABEL $inputf\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@$retval\n");
    printf("MOVE LF@$retval nil@nil\n");
    printf("DEFVAR LF@param$1\n");
    printf("DEFVAR LF@error$check\n");
    printf("READ LF@param$1 float\n");
    printf("TYPE LF@error$check LF@param$1\n");
    printf("JUMPIFNEQ $ERROR$INPUTF string@float LF@error$check\n");
    printf("MOVE LF@$retval LF@param$1\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
    printf("LABEL $ERROR$INPUTF\n");
    printf("EXIT int@1\n\n");
}

void gen_func_print(){
    printf("#PRINT\n\n");
    printf("LABEL $print\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@print$var\n");
    printf("LABEL while$print\n");
    printf("POPS LF@print$var\n");
    printf("WRITE LF@print$var\n");
    printf("SUB LF@$0 LF@$0 int@1\n");
    printf("JUMPIFNEQ while$print LF@$0 int@0\n");
    printf("CLEARS\n");
    printf("RETURN\n");
}

void gen_func_int2float(){
    printf("#FUNCTION INT2FLOAT\n\n");
    printf("LABEL $int2float\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@$retval\n");
    printf("DEFVAR LF@error$check\n");
    printf("TYPE LF@error$check LF@$1\n");
    printf("JUMPIFNEQ $ERROR$INT2FLOAT string@int LF@error$check\n");
    printf("INT2FLOAT LF@$retval LF@$1\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
    printf("LABEL $ERROR$INT2FLOAT\n");
    printf("EXIT int@6\n\n");
}

void gen_func_float2int(){
    printf("#FUNCTION FLOAT2INT\n\n");
    printf("LABEL $float2int\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@$retval\n");
    printf("DEFVAR LF@error$check\n");
    printf("TYPE LF@error$check LF@$1\n");
    printf("JUMPIFNEQ $ERROR$FLOAT2INT string@float LF@error$check\n");
    printf("FLOAT2INT LF@$retval LF@$1\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
    printf("LABEL $ERROR$FLOAT2INT\n");
    printf("EXIT int@6\n\n");
}

void gen_func_len(){
    printf("#FUNCTION LEN\n\n");
    printf("LABEL $len\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@$retval1\n");
    printf("POPS LF@$retval1\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
}

void gen_func_substr(){
    printf("LABEL $substr");
    printf("PUSHFRAME");
    printf("DEFVAR LF@string");
    printf("DEFVAR LF@i");
    printf("DEFVAR LF@n");

    printf("POPS LF@string");
    printf("POPS LF@i");
    printf("POPS LF@n");

    printf("DEFVAR LF@$retval1");
    printf("DEFVAR LF@$retval2");

    printf("DEFVAR LF@$bool");
    printf("DEFVAR LF@$len");
    printf("DEFVAR LF@$tmpstring");

    printf("STRLEN LF@$len LF@string");
    printf("MOVE LF@$retval1 string@");
    printf("MOVE LF@$retval2 int@0");

    printf("CLEARS");
    printf("PUSHS LF@$len");
    printf("PUSHS LF@i");
    printf("GTS");
    printf("PUSHS LF@i");
    printf("PUSHS int@0");
    printf("LTS");
    printf("NOTS");
    printf("ANDS");

    printf("PUSHS LF@n");
    printf("PUSHS int@0");
    printf("LTS");
    printf("NOTS");
    printf("ANDS");

    printf("POPS LF@$bool");
    printf("JUMPIFEQ $substr$error LF@$bool bool@false");

    printf("PUSHS LF@$len");
    printf("PUSHS LF@i");
    printf("SUBS");
    printf("PUSHS LF@n");
    printf("LTS");

    printf("POPS LF@$bool");
    printf("ADD LF@n LF@n LF@i #defaultvalue");
    printf("JUMPIFEQ $substr$lts bool@false LF@$bool");
    printf("MOVE LF@n LF@$len");
    printf("LABEL $substr$lts");


    printf("LABEL $substr$whilestart");
    printf("PUSHS LF@i");
    printf("PUSHS LF@n");
    printf("LTS");
    printf("POPS LF@$bool");
    printf("JUMPIFEQ $substr$end LF@$bool bool@false");

    printf("GETCHAR LF@$tmpstring LF@string LF@i");
    printf("CONCAT LF@$retval1 LF@$retval1 LF@$tmpstring");

    printf("ADD LF@i LF@i int@1");
    printf("JUMP $substr$whilestart");

    printf("label $substr$error");
    printf("MOVE LF@$retval2 int@1");

    printf("label $substr$end");
    printf("POPFRAME");
    printf("RETURN");
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
