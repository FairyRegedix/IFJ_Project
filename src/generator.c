//generator



#include "generator.h"

unsigned int GTEcounter = 0;
unsigned int LTEcounter = 0;
unsigned int ID = 0;
unsigned int elseCounter = 0;
StringList ListOfStrings;
StringList Vars;
StringList Exps;

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
    printf("LABEL $$final_end\n");
    //printf("WRITE string@Program\\032end!");
    printf("EXIT int@0\n");
    DisposeListString(&ListOfStrings);
    DisposeListString(&Vars);
    DisposeListString(&Exps);


}

void generate_header(){
    InitListString(&ListOfStrings);
    InitListString(&Vars);
    InitListString(&Exps);

    printf(".IFJcode20\n");
    printf("DEFVAR GF@EXPRESULT\n");
    printf("DEFVAR GF@CONCATRESULT\n");
    printf("DEFVAR GF@exp$string1\n");
    printf("DEFVAR GF@exp$string2\n");
    printf("CREATEFRAME\n");
    printf("CALL func$main\n");
    printf("JUMP $$final_end\n");

    gen_func_inputs();
    gen_func_inputf();
    gen_func_inputi();
    gen_func_int2float();
    gen_func_float2int();
    gen_func_print();

}

void gen_defvar(char* id,int scope, bool in_for){
    if(in_for){
        printf("MOVE LF@%s$%d nil@nil\n",id, scope);
    }else
    {
        printf("DEFVAR LF@%s$%d\n",id, scope);
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
    if(params->len <= 0)
        return;
    printf("DEFVAR LF@");
    for(int i = 0; i < params->len-1; i++){
        if(params->str[i] != '#'){
            printf("%c", params->str[i]);
        }else
        {
            printf("$0\nDEFVAR LF@");
        }
    }
    printf("$0\n");
    char tmpchar[params->len];
    int j = 0;
    for(int i = 0 ; i < params->len; i++){
        if(params->str[i] != '#'){
            tmpchar[j++] = params->str[i];
        }else
        {
            tmpchar[j] = '\0';
            printf("POPS LF@%s$0\n", tmpchar);
            j = 0;
        }
    }
}

void gen_call_params(token_t *last, st_stack_t *local_st) {
    token_t* prev = last;
    if (prev == NULL)
        return;
    while(prev->type != TOKEN_LBRACKET){
        gen_pushs_param(prev->type, &prev->actual_value, local_st);
        prev = prev->prev;
    }

}
void gen_pushs_param(token_type type, string *value, st_stack_t *local_st){
    st_item* item;
    switch (type)
    {
    case TOKEN_INTEGER:
        printf("PUSHS int@%s\n", value->str);
        break;

    case TOKEN_FLOAT:
        printf("PUSHS float@%a\n", Str_to_Float(value));
        break;
    case TOKEN_ID:
        item = stack_lookup(local_st, value);
        printf("PUSHS LF@%s$%d\n", value->str, item->data.scope);
        break;

    case TOKEN_STR:
        printf("PUSHS string@%s\n", value->str);
        break;

    default:
        break;
    }

}

void gen_add_to_vars(char *var_name, int scope) {
    char* tmp = malloc(strlen(var_name)+BLOCK_SIZE);
    if(scope < 0)
        sprintf(tmp,"%s",var_name);
    else
        sprintf(tmp,"%s$%i",var_name,scope);
    InsertFirstString(&Vars, tmp);
}
void gen_add_to_exp(char *exp, bool in_for) {
    char* tmp = malloc(strlen(exp)+1);
    strcpy(tmp,exp);
    if(in_for)
        InsertFirstString(&Exps, tmp);
    else
        InsertLastString(&Exps, tmp);
}


void gen_assign(int NumberOfVariables) {
    if(NumberOfVariables <= 0)
        return;
    printf("CREATEFRAME\n");
    for(int j = 0; j < NumberOfVariables; j++){
        printf("%s", Exps.First->data);
        DeleteFirstString(&Exps);
        printf("DEFVAR TF@$tmp$%d\n",ID);
        printf("POPS TF@$tmp$%d\n", ID);
        push_int();
    }

    for(int i = 0; i < NumberOfVariables; i++){
        if(strcmp(Vars.First->data, "_") == 0)
            continue;
        printf("MOVE LF@%s TF@$tmp$%d\n",Vars.First->data, IntStack[top]);
        DeleteFirstString(&Vars);
        pop_int();
    }

}

void gen_assign_return(int NumberOfVariables){
    for(int i = NumberOfVariables; i > 0; i--){
        if(strcmp(Vars.First->data, "_") == 0)
            continue;
        printf("MOVE LF@%s TF@retval$%i\n",Vars.First->data, i);
        DeleteFirstString(&Vars);
    }
}

void gen_set_retvals(int NumberOfReturns){
    for(int i = 1; i <= NumberOfReturns; i++){
        printf("%s", Exps.First->data);
        DeleteFirstString(&Exps);
        printf("POPS LF@retval$%i\n",i);
    }
}


void gen_for_start(char *expression){
    printf("LABEL CHECK$FOR$%d\n", ID);
    printf("%s",expression);
    gen_for_jump();
}

void gen_for_jump(){
    printf("PUSHS bool@true\n");
    printf("JUMPIFNEQS END$FOR$%d\n", ID);
    push_int();
}

void gen_for_end(){
    StringElementPtr tmp  = Vars.First;
    int count = 0;
    while(tmp != NULL){
        tmp = tmp->ptr;
        count++;
    }
    printf("JUMP CHECK$FOR$%d\n", IntStack[top]);
    printf("LABEL END$FOR$%d\n", IntStack[top]);
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

void gen_if_start(char* expression){
    printf("#IF $if$%d\n",ID);
    printf("%s", expression);
    printf("PUSHS bool@true\n");
    printf("JUMPIFNEQS $if$%d$else$%d\n",ID, elseCounter);
    push_int();
}

void gen_if_else(char* expression){
    printf("JUMP $if$%d$end\n", IntStack[top]);
    printf("LABEL $if$%d$else$%d\n", IntStack[top], elseCounter);
    printf("%s", expression);
    elseCounter++;
    printf("PUSHS bool@true\n");
    printf("JUMPIFNEQS $if$%d$else$%d\n",ID, elseCounter);
}

void gen_else(){
    printf("JUMP $if$%d$end\n", IntStack[top]);
    printf("LABEL $if$%d$else$%d\n", IntStack[top], elseCounter);
}

void gen_if_end(){
    printf("LABEL $if$%d$end\n", IntStack[top]);
    pop_int();
    elseCounter = 0;
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
    printf("LABEL func$inputs\n");
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
    printf("LABEL func$inputi\n");
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
    printf("LABEL func$inputf\n");
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
    printf("LABEL func$print\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@print$var\n");
    printf("LABEL while$print\n");
    printf("POPS LF@print$var\n");
    printf("WRITE LF@print$var\n");
    printf("SUB LF@$0 LF@$0 int@1\n");
    printf("JUMPIFNEQ while$print LF@$0 int@0\n");
    printf("CLEARS\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
}

void gen_func_int2float(){
    printf("#FUNCTION INT2FLOAT\n\n");
    printf("LABEL func$int2float\n");
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
    printf("LABEL func$float2int\n");
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
    printf("LABEL func$len\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@$retval1\n");
    printf("POPS LF@$retval1\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
}

void gen_func_substr(){
    printf("LABEL func$substr\n\n");
    printf("PUSHFRAME\n");
    printf("DEFVAR LF@string\n");
    printf("DEFVAR LF@i\n");
    printf("DEFVAR LF@n\n");

    printf("POPS LF@string\n");
    printf("POPS LF@i\n");
    printf("POPS LF@n\n");

    printf("DEFVAR LF@$retval1\n");
    printf("DEFVAR LF@$retval2\n");

    printf("DEFVAR LF@$bool\n");
    printf("DEFVAR LF@$len\n");
    printf("DEFVAR LF@$tmpstring\n");

    printf("STRLEN LF@$len LF@string\n");
    printf("MOVE LF@$retval1 string@\n");
    printf("MOVE LF@$retval2 int@0\n");

    printf("CLEARS\n");
    printf("PUSHS LF@$len\n");
    printf("PUSHS LF@i\n");
    printf("GTS\n");
    printf("PUSHS LF@i\n");
    printf("PUSHS int@0\n");
    printf("LTS\n");
    printf("NOTS\n");
    printf("ANDS\n");

    printf("PUSHS LF@n\n");
    printf("PUSHS int@0\n");
    printf("LTS\n");
    printf("NOTS\n");
    printf("ANDS\n");

    printf("POPS LF@$bool\n");
    printf("JUMPIFEQ $substr$error LF@$bool bool@false\n");

    printf("PUSHS LF@$len\n");
    printf("PUSHS LF@i\n");
    printf("SUBS\n");
    printf("PUSHS LF@n\n");
    printf("LTS\n");

    printf("POPS LF@$bool\n");
    printf("ADD LF@n LF@n LF@i #defaultvalue\n");
    printf("JUMPIFEQ $substr$lts bool@false LF@$bool\n");
    printf("MOVE LF@n LF@$len\n");
    printf("LABEL $substr$lts\n");


    printf("LABEL $substr$whilestart\n");
    printf("PUSHS LF@i\n");
    printf("PUSHS LF@n\n");
    printf("LTS\n");
    printf("POPS LF@$bool\n");
    printf("JUMPIFEQ $substr$end LF@$bool bool@false\n");

    printf("GETCHAR LF@$tmpstring LF@string LF@i\n");
    printf("CONCAT LF@$retval1 LF@$retval1 LF@$tmpstring\n");

    printf("ADD LF@i LF@i int@1\n");
    printf("JUMP $substr$whilestart\n");

    printf("label $substr$error\n");
    printf("MOVE LF@$retval2 int@1\n");

    printf("label $substr$end\n");
    printf("POPFRAME\n");
    printf("RETURN\n\n");
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
