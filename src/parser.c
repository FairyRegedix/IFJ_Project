
#include "expression.h"

#define consume_token 1 //helper variable for MATCH macro
#define keep_token 0    //helper variable for MATCH macro


#define MATCH(x, y) do{                 \
  if (p->token->type != (x)){        \
   return handle_error(ERROR_SYN,      \
     "at line %i: expected token [%s] received [%s].\n", \
     p->token->lineno,token_enum_to_str((x)),token_enum_to_str(p->token->type));}\
  if((y))                               \
    get_next_token(p);  \
}while(0)




int get_next_token(parser_info* p){
    token_list_next(&p->token_list);
    p->token = p->token_list.act;
    return SUCCESS;
}


int prolog(parser_info *p);

int prog(parser_info *p);

int func(parser_info *p);

int EOL_opt(parser_info *p);

int params(parser_info *p);

int params_n(parser_info *p);

int ret_type(parser_info *p);

int ret_type_n(parser_info *p);

int statement_list(parser_info *p);

int statement(parser_info *p);

int var(parser_info *p);

int assign(parser_info *p);

int end_assign(parser_info *p);

int exp_n(parser_info *p);

int for_def(parser_info *p);

int for_assign(parser_info *p);

int func_call(parser_info *p);

int call_params(parser_info *p);

int term_n(parser_info *p);

int return_exp(parser_info *p);

int for_assign(parser_info *p) {
    int error_code;
    st_item* item;
    if (p->token->type == TOKEN_ID) {

        str_reinit(&p->left_side_vars_types);
        if(strcmp(p->token->actual_value.str,"_") == 0)
            CHECK(str_add_char(&p->left_side_vars_types,'_'),SUCCESS);
        else {
            item = stack_lookup(p->local_st, &p->token->actual_value);
            if(item == NULL)//not defined beforehand
                return ERROR_SEM_DEF;
            CHECK(str_add_char(&p->left_side_vars_types,item->data.as.variable.value_type),SUCCESS);
        }

        get_next_token(p);
        CHECK(assign(p), SUCCESS);
        MATCH(TOKEN_ASSIGN, consume_token);
        return end_assign(p);
    }

    return SUCCESS;
}

int for_def(parser_info *p) {
    int error_code;
    st_item* item;

    if (p->token->type == TOKEN_ID) {
        if(!strcmp(p->token->next->actual_value.str, "_"))
            return ERROR_SEM_OTHER;
        item = st_insert(&p->local_st->local_table, &p->token->actual_value, type_variable);
        if(item->data.defined)
            return ERROR_SEM_DEF;

        get_next_token(p);
        MATCH(TOKEN_DEFINITION, consume_token);
        str_reinit(&p->right_side_exp_types);
        CHECK(expression(p), SUCCESS);
        if(p->right_side_exp_types.len == 0)
            return ERROR_SYN;
        item->data.as.variable.value_type = (data_type) p->right_side_exp_types.str[0];
        item->data.defined = true;

    }
    return SUCCESS;
}

int return_exp(parser_info *p) {
    int error_code;
    if (p->token->type == TOKEN_EOL) {
        if(p->in_function->data.as.function.ret_types.len != 0)
            return ERROR_SEM_PAR;

    } else {
        CHECK(expression(p), SUCCESS);
        CHECK(exp_n(p), SUCCESS);
    }
    return SUCCESS;
}

int term_n(parser_info *p) {
    int error_code;
    data_type type;
    while (p->token->type == TOKEN_COMMA) {
        get_next_token(p);
        CHECK(EOL_opt(p), SUCCESS);

        CHECK(set_data_type(p, p->token, &type, true), SUCCESS);
        CHECK(str_add_char(&p->right_side_exp_types, type), SUCCESS);

        get_next_token(p);
    }

    return SUCCESS;
}

int call_params(parser_info *p) {
    int error_code;
    data_type type;
    str_reinit(&p->right_side_exp_types);

    if(p->token->type == TOKEN_RBRACKET)
        return SUCCESS; // call_params -> eps

    CHECK(set_data_type(p, p->token, &type, 0), SUCCESS);
    CHECK(str_add_char(&p->right_side_exp_types, type), SUCCESS);

    get_next_token(p);
    CHECK( term_n(p), SUCCESS);

    return SUCCESS;
}

int func_call(parser_info *p) {
    int error_code;
    MATCH(TOKEN_LBRACKET, consume_token);
    CHECK(EOL_opt(p), SUCCESS);
    CHECK(call_params(p), SUCCESS);
    MATCH(TOKEN_RBRACKET, consume_token);

    if(check_types(&p->left_side_vars_types,&p->function_called->data.as.function.ret_types))
        return ERROR_SEM_PAR;
    if(strcmp(p->function_called->key.str,"print") != 0){
        if(str_cmp(&p->function_called->data.as.function.param_types, &p->right_side_exp_types)) //parameter types or number off parameter does not match
            return ERROR_SEM_PAR;
    }
    return SUCCESS;
}


int exp_n(parser_info *p) {

    int error_code;
    while (p->token->type == TOKEN_COMMA) {
        get_next_token(p);
        CHECK(expression(p), SUCCESS);
    }
    return SUCCESS;
}

int end_assign(parser_info *p) {
    int error_code;
    st_item *item;
    str_reinit(&p->right_side_exp_types);

    if (p->token->type == TOKEN_ID) {
        item = stack_lookup(p->local_st, &p->token->actual_value);
        if (item == NULL)
            item = st_get_item(&p->st, &p->token->actual_value);
        if (item != NULL) {
            if( item->data.type == type_function){
                p->function_called = item;
                get_next_token(p);
                return func_call(p);
            }
        }
        else//identifier not defined
            return ERROR_SEM_DEF;
    }

    CHECK(expression(p), SUCCESS);//call to expression and then subsequent calls to expression in exp_n
    CHECK(exp_n(p), SUCCESS);
    if(p->right_side_exp_types.len == 0)
        return ERROR_SYN;
    if(check_types(&p->left_side_vars_types,&p->right_side_exp_types))
        return ERROR_SEM_OTHER;

    return SUCCESS;
}

int assign(parser_info *p) {
    int error_code;
    st_item* item;

    while(p->token->type == TOKEN_COMMA) {
        get_next_token(p);
        item = NULL;
        CHECK(EOL_opt(p), SUCCESS);
        //next token set
        MATCH(TOKEN_ID, keep_token);
        if(!strcmp(p->token->actual_value.str,"_"))
            CHECK(str_add_char(&p->left_side_vars_types,'_'),SUCCESS);
        else {
            item = stack_lookup(p->local_st, &p->token->actual_value);
            if(item == NULL)//not defined beforehand
                return ERROR_SEM_DEF;
            CHECK(str_add_char(&p->left_side_vars_types,item->data.as.variable.value_type),SUCCESS);
        }

        get_next_token(p);
    }

    return SUCCESS;
}


int var(parser_info *p) {
    int error_code;
    st_item* item;

    switch (p->token->type) {
        case TOKEN_DEFINITION:
            str_reinit(&p->right_side_exp_types);
            if(!strcmp(p->token->prev->actual_value.str,"_"))
                return ERROR_SEM_DEF;
            item = st_insert(&p->local_st->local_table, &p->token->prev->actual_value, type_variable);
            if (item == NULL)
                return ERROR_TRANS;
            if (item->data.defined) //redefinition of local variable
                return ERROR_SEM_DEF;
            get_next_token(p);
            CHECK(expression(p), SUCCESS);
            if(p->right_side_exp_types.len == 0)
                return ERROR_SYN;
            item->data.as.variable.value_type = (data_type) p->right_side_exp_types.str[0];
            item->data.defined = true;
            //....
            return SUCCESS;

        case TOKEN_LBRACKET:
            item = stack_lookup(p->local_st, &p->token->prev->actual_value);
            if (item == NULL)
                item = st_get_item(&p->st, &p->token->prev->actual_value);
            if (item != NULL){
                if( item->data.type == type_function){
                    str_reinit(&p->left_side_vars_types);
                    p->function_called = item;
                    return func_call(p);
                }else
                    return ERROR_SEM_OTHER;
            }
            else//identifier not defined
                return ERROR_SEM_DEF;

        case TOKEN_COMMA:
        case TOKEN_ASSIGN:
            str_reinit(&p->left_side_vars_types);
            if(!strcmp(p->token->prev->actual_value.str,"_")) {
                CHECK(str_add_char(&p->left_side_vars_types,'_'),SUCCESS);

            }else{
                item = stack_lookup(p->local_st, &p->token->prev->actual_value);
                if(item == NULL)//not defined beforehand
                    return ERROR_SEM_DEF;
                CHECK(str_add_char(&p->left_side_vars_types, item->data.as.variable.value_type),SUCCESS);
            }

            if(p->token->type == TOKEN_COMMA){
                CHECK(assign(p), SUCCESS);
                MATCH(TOKEN_ASSIGN, consume_token);
            }
            else
                get_next_token(p);

            CHECK(EOL_opt(p), SUCCESS);
            CHECK(end_assign(p), SUCCESS);
            return SUCCESS;

        default:
            return handle_error(ERROR_SYN,
                                "at line %i: unexpected token [%s].\n",
                                p->token->lineno, token_enum_to_str(p->token->type));
    }
    //return SUCCESS;

}

int statement(parser_info *p) {
    int error_code;
    if (p->token->type == TOKEN_ID) {
        get_next_token(p);
        CHECK(var(p), SUCCESS);

    } else if (p->token->type == TOKEN_IF) {
        get_next_token(p);
        CHECK(expression(p), SUCCESS);
        //next token set

        MATCH(TOKEN_LCURLY, consume_token);
        CHECK(enter_scope(&p->local_st, &p->scope), SUCCESS);

        MATCH(TOKEN_EOL, consume_token);
        CHECK(EOL_opt(p), SUCCESS);

        //next token set
        CHECK(statement_list(p), SUCCESS);

        //next token set
        MATCH(TOKEN_RCURLY, consume_token);
        CHECK(leave_scope(&p->local_st, &p->scope), SUCCESS);

        MATCH(TOKEN_ELSE, consume_token);

        MATCH(TOKEN_LCURLY, consume_token);
        CHECK(enter_scope(&p->local_st, &p->scope), SUCCESS);

        MATCH(TOKEN_EOL, consume_token);
        CHECK(EOL_opt(p), SUCCESS);

        //next token set
        CHECK(statement_list(p), SUCCESS);
        //next token set
        MATCH(TOKEN_RCURLY, consume_token);
        CHECK(leave_scope(&p->local_st, &p->scope), SUCCESS);

    } else if (p->token->type == TOKEN_FOR) {
        get_next_token(p);
        //CHECK(EOL_opt(p), SUCCESS);
        CHECK(enter_scope(&p->local_st, &p->scope), SUCCESS);
        CHECK(for_def(p), SUCCESS);
        //next token set
        MATCH(TOKEN_SEMICOLON, consume_token);
        //CHECK(EOL_opt(p), SUCCESS);
        CHECK(expression(p), SUCCESS);
        //if(exp_result != bool)
        //  return ERROR_SEM_COMP;
        //next token set
        MATCH(TOKEN_SEMICOLON, consume_token);
        //CHECK(EOL_opt(p), SUCCESS);

        CHECK(for_assign(p), SUCCESS);
        //next token set
        MATCH(TOKEN_LCURLY, consume_token);
        CHECK(enter_scope(&p->local_st, &p->scope), SUCCESS);

        MATCH(TOKEN_EOL, consume_token);
        CHECK(EOL_opt(p), SUCCESS);
        //next token set
        CHECK(statement_list(p), SUCCESS);
        //next token set
        MATCH(TOKEN_RCURLY, consume_token);
        CHECK(leave_scope(&p->local_st, &p->scope), SUCCESS);
        CHECK(leave_scope(&p->local_st, &p->scope), SUCCESS);

    } else if (p->token->type == TOKEN_RETURN) {
        //+semantic check return types
        str_reinit(&p->right_side_exp_types);
        get_next_token(p);
        CHECK(return_exp(p), SUCCESS);
        if(str_cmp(&p->in_function->data.as.function.ret_types, &p->right_side_exp_types))
            return ERROR_SEM_PAR;

    } else { ; }

    return SUCCESS;

}

//Statement_list -> Statement EOL EOL_opt Statement_list | eps
int statement_list(parser_info *p) {
    int error_code;

    switch (p->token->type) {
        case TOKEN_ID:
        case TOKEN_IF:
        case TOKEN_FOR:
        case TOKEN_RETURN:
            CHECK(statement(p), SUCCESS);
            MATCH(TOKEN_EOL, consume_token);
            CHECK(EOL_opt(p), SUCCESS);
            return statement_list(p);

        default:
            return SUCCESS;
    }
}

//Ret_type_n -> , Type Ret_type_n | eps
int ret_type_n(parser_info *p) {
    int error_code;
    //next token set already
    if (p->token->type == TOKEN_COMMA) {//->,<- Type Ret_type_n
        get_next_token(p);
        CHECK(EOL_opt(p), SUCCESS);

        //next token set
        if(is_data_type(p->token->type)){
            get_next_token(p);
            return ret_type_n(p);
        } else
            return handle_error(ERROR_SYN,
                                "at line %i: expected type specifier received [%s].\n",
                                p->token->lineno, token_enum_to_str(p->token->type));
    } else
        return SUCCESS; //eps
}

//Ret_type -> ( Type Ret_type_n ) | Type
int ret_type(parser_info *p) {
    int error_code;

    if (p->token->type == TOKEN_LBRACKET) {
        get_next_token(p);
        if(is_data_type(p->token->type)){
            get_next_token(p);
            CHECK(ret_type_n(p), SUCCESS);
        }
        MATCH(TOKEN_RBRACKET, consume_token);
        //return SUCCESS;

    } else if(is_data_type(p->token->type)) {
        CHECK(str_add_char(&p->in_function->data.as.function.ret_types,p->token->type),SUCCESS);
        //return SUCCESS;
    } else{
        //return SUCCESS;
    }
    return SUCCESS;
}

//Params_n -> , id Type Params_n | eps
int params_n(parser_info *p) {
    int error_code;
    if (p->token->type == TOKEN_COMMA) {
        get_next_token(p);
        CHECK(EOL_opt(p), SUCCESS);

        // next token set
        if (p->token->type == TOKEN_ID) {// ->id<- Type Params_n
            st_item *item = st_insert(&p->local_st->local_table, &p->token->actual_value, type_variable);
            if (item == NULL)
                return ERROR_TRANS;
            if (item->data.defined) //redefinition of local variables
                return ERROR_SEM_DEF;
            else
                item->data.defined = true;

            get_next_token(p);

            if(is_data_type(p->token->type))
                item->data.as.variable.value_type = p->token->type;
            else
                return handle_error(ERROR_SYN,
                                    "at line %i: expected type specifier received [%s].\n",
                                     p->token->lineno, token_enum_to_str(p->token->type));
            //everything ok, received id and type specifier
            get_next_token(p);
            return params_n(p); //, id Type ->Params_n<-

        } else
            return handle_error(ERROR_SYN,
                                "at line %i: expected identifier received [%s].\n",
                                p->token->lineno, token_enum_to_str(p->token->type));

    } else//Params_n -> eps
        return SUCCESS;
}

//Params -> id Type Params_n | eps
int params(parser_info *p) {
    int error_code;

    CHECK(EOL_opt(p), SUCCESS);
    // next token set
    if (p->token->type == TOKEN_ID) {// ->id<- Type Params_n | else eps
        st_item *item = st_insert(&p->local_st->local_table, &p->token->actual_value, type_variable);
        if (item == NULL)
            return ERROR_TRANS;
        if (item->data.defined) //redefinition of local variables
            return ERROR_SEM_DEF;

        item->data.defined = true;

        get_next_token(p);

        if(is_data_type(p->token->type))//id ->Type<- Params_n
            item->data.as.variable.value_type = p->token->type;
        else
            return handle_error(ERROR_SYN,
                                "at line %i: expected type specifier received [%s].\n",
                                p->token->lineno, token_enum_to_str(p->token->type));

        //everything ok, received type specifier
        get_next_token(p);
        //id Type ->Params_n<-
        return params_n(p);
    } else //Params -> eps
        return SUCCESS;
}

//Func -> id ( Params ) Ret_type { EOL EOL_opt Statement_list }
int func(parser_info *p) {
    int error_code;
    MATCH(TOKEN_ID, keep_token);
    p->in_function = st_get_item(&p->st, &p->token->actual_value);
    CHECK(enter_scope(&p->local_st, &p->scope), SUCCESS);

    get_next_token(p);
    MATCH(TOKEN_LBRACKET, consume_token);
    CHECK(params(p), SUCCESS);
    MATCH(TOKEN_RBRACKET, consume_token);
    CHECK(ret_type(p), SUCCESS);

    MATCH(TOKEN_LCURLY, consume_token);
    MATCH(TOKEN_EOL, consume_token);
    CHECK(EOL_opt(p), SUCCESS);
    //next token set
    CHECK(statement_list(p), SUCCESS);
    //next token set
    MATCH(TOKEN_RCURLY, consume_token);

    CHECK(leave_scope(&p->local_st, &p->scope), SUCCESS);
    return SUCCESS;
}


//Prog -> func Func EOL EOL_opt Prog
//Prog -> EOF
int prog(parser_info *p) {
    int error_code;

    if (p->token->type == TOKEN_FUNC) {//Prog - > ->func<- ...
        get_next_token(p);
        CHECK(func(p), SUCCESS);
        MATCH(TOKEN_EOL, consume_token);
        CHECK(EOL_opt(p), SUCCESS);
        // next token set
        return prog(p);
    } else if (p->token->type == TOKEN_EOF) {//Prog -> EOF
        return SUCCESS;
    } else {
        return handle_error(ERROR_SYN,
                            "at line %i: expected EOF or func received [%s].\n",
                             p->token->lineno, token_enum_to_str(p->token->type));
    }

}

//Prolog -> package id EOL EOL_opt Prog
int prolog(parser_info *p) {
    int error_code;

    CHECK(EOL_opt(p),SUCCESS);
    MATCH(TOKEN_PACKAGE, consume_token);
    MATCH(TOKEN_ID, keep_token);
    if (str_cmp_c_str(&p->token->actual_value, "main"))//package identifier not "main"
        return ERROR_SEM_OTHER;
    //token not consumed , get next
    get_next_token(p);
    MATCH(TOKEN_EOL, consume_token);
    CHECK(EOL_opt(p), SUCCESS);

    return prog(p);
}

int EOL_opt(parser_info *p) {
    //int error_code;
    while (p->token->type == TOKEN_EOL) {
        get_next_token(p);
    }
    return SUCCESS;
}

/*parser driver code
 * returns error code*/
int parser() {
    int error_code;
    parser_info p;
    p.token = NULL;
    p.local_st = NULL;
    p.in_function = NULL;
    p.scope = 0;

    p.function_called = NULL;
    str_init(&p.left_side_vars_types);
    str_init(&p.right_side_exp_types);
    st_init(&p.st);
    token_list_init(&p.token_list);

    if((error_code = scanner_fill_token_list(&p.token_list)) != SUCCESS){
        token_list_dispose(&p.token_list);
        str_free(&p.right_side_exp_types);
        str_free(&p.left_side_vars_types);
        return error_code;
    }
    else if((error_code = fill_st_with_builtin(&p.st)) != SUCCESS){
        st_dispose(&p.st);
        str_free(&p.right_side_exp_types);
        str_free(&p.left_side_vars_types);
        token_list_dispose(&p.token_list);
        return error_code;
    }

    else if((error_code = first_pass(&p)) != SUCCESS){
        st_dispose(&p.st);
        str_free(&p.right_side_exp_types);
        str_free(&p.left_side_vars_types);
        token_list_dispose(&p.token_list);
        return error_code;
    }
    else if ((error_code = get_next_token(&p)) != SUCCESS) {
        st_dispose(&p.st);
        str_free(&p.right_side_exp_types);
        str_free(&p.left_side_vars_types);
        token_list_dispose(&p.token_list);
        return error_code;
    } else {
        error_code = prolog(&p);
        if(error_code == SUCCESS)
            error_code = check_main(&p.st);
        str_free(&p.right_side_exp_types);
        str_free(&p.left_side_vars_types);
        st_dispose(&p.st);
        token_list_dispose(&p.token_list);
        while(p.local_st != NULL)
            leave_scope(&p.local_st,&p.scope);

        return error_code;
    }
}

int first_pass(parser_info* p){
    //int error_code;
    st_item *func;
    get_next_token(p);
    get_next_token(p);
    //"package main" at the start of a valid IFJ20 code file so we can safely skip 2 tokens;
    while(p->token->type != TOKEN_EOF){
        get_next_token(p);
        if(p->token->type != TOKEN_FUNC)
            continue; //only interested in the function header
        get_next_token(p);
        if(p->token->type != TOKEN_ID) //incorrect syntax skip
            continue;

        func = st_insert(&p->st,
                         &p->token->actual_value,
                         type_function);
        if (func == NULL)
            return ERROR_TRANS;
        if (func->data.defined) //redefinition of function/variable
            return handle_error(ERROR_SEM_DEF,
                                "at line %i: redefinition of function \"%s\"",
                                p->token->lineno, func->key.str);

        func->data.defined = true;

        while(p->token->type != TOKEN_EOL && p->token->type != TOKEN_EOF){
            get_next_token(p);
            if(p->token->type == TOKEN_ID){
                str_concat(&func->data.as.function.params, p->token->actual_value.str, p->token->actual_value.len);
                str_add_char(&func->data.as.function.params,'#');

                get_next_token(p);

                if(is_data_type(p->token->type))
                    str_add_char(&func->data.as.function.param_types, p->token->type);
                else
                    return handle_error(ERROR_SYN,
                                        "at line %i: incorrect syntax of a function header of \"%s\".\n",
                                        p->token->lineno,func->key.str);

            }
            else if(is_data_type(p->token->type))
                str_add_char(&func->data.as.function.ret_types, p->token->type);
            else{}
        }
    }
    p->token_list.act = NULL;
    return SUCCESS;
}

int main() {//testing
    //printf("Done\n");

    int ret = parser();
    if (ret)
        return ret;
    else
        printf("Parsing Successful!!\n");
    return 0;
}


//func inputs() (string, int)
//func inputi() (int,int)
//func inputf() (float64,int)
//func int2float(i int) (float64)
//func float2int(f float64) (int)
//func len(s string) (int)
//func substr(s string, i int, n int) (string, int)
//func ord(s string, i int) (int, int)
//func chr(i int) (string, int)
//func print( ... )

int fill_st_with_builtin(symbol_table_t* st){
    //int error_code;
    string name;
    //type_float= \005,  type_int = \011,  type_string = \014,
    char* func[10][4] = {{"inputs","","","\014\011"},
                        {"inputi","","","\011\011"},
                        {"inputf","","","\005\011"},
                        {"int2float","i#","\011","\005"},
                        {"float2int","f#","\005","\011"},
                        {"len","s#","\014","\011"},
                        {"substr","s#i#n#","\014\011\011","\014\011"},
                        {"ord","s#i#","\014\011","\011\011"},
                        {"chr","i#","\011","\014\011"},
                        {"print","","",""}};
    str_init(&name);
    for(int i = 0; i<10;i++){
        str_reinit(&name);
        str_concat(&name,func[i][0],(int)strlen(func[i][0]));
        st_item *item = st_insert(st, &name, type_function);

        if(item == NULL){
            str_free(&name);
            return ERROR_TRANS;
        }

        str_concat(&item->data.as.function.params,func[i][1],(int)strlen(func[i][1]));
        str_concat(&item->data.as.function.param_types,func[i][2],(int)strlen(func[i][2]));
        str_concat(&item->data.as.function.ret_types,func[i][3],(int)strlen(func[i][3]));

        item->data.defined = true;
    }
    str_free(&name);
    return SUCCESS;
}





int add_symbol(symbol_table_t* table ,token_t* token, item_type type){
    st_item* item;
    char* error_msg;
    if(type == type_function){
        error_msg = "at line %i: redefinition of function \"%s\"";
    }else{
        error_msg = "at line %i: redefinition of variable \"%s\"";
    }
    item = st_insert(table,
                     &token->actual_value,
                     type);
    if (item == NULL)
        return ERROR_TRANS;
    if (item->data.defined) //redefinition of function/variable
        return handle_error(ERROR_SEM_DEF,
                            error_msg,
                            token->lineno, item->key.str);

    item->data.defined = true;
    return SUCCESS;
}

int check_main(symbol_table_t* st){
    string name = c_str_to_str("main");
    st_item* item = st_get_item(st,&name);
    str_free(&name);

    if(item == NULL)
        return ERROR_SEM_DEF;
    if(strcmp(item->data.as.function.param_types.str,""))
        return ERROR_SEM_PAR;
    if(strcmp(item->data.as.function.ret_types.str,""))
        return ERROR_SEM_PAR;

    return SUCCESS;
}

int set_data_type(parser_info *p, token_t *token, data_type *type, bool throw_error_on_default) {
    st_item* item = NULL;
    switch(token->type){
        case TOKEN_INT:
        case TOKEN_INTEGER:
            *type = type_int;
            break;
        case TOKEN_FLOAT:
        case TOKEN_FLOAT64:
            *type = type_float;
            break;
        case TOKEN_STR:
        case TOKEN_STRING:
            *type = type_str;
            break;
        case TOKEN_ID:
            if((item = stack_lookup(p->local_st, &token->actual_value)) == NULL)
                return handle_error(
                        ERROR_SEM_DEF,
                        "at line %i: undefined variable \"%s\".\n",
                        token->lineno, token->actual_value.str);
            else
                *type = item->data.as.variable.value_type;
            break;
        default:
            if(throw_error_on_default)
                return handle_error(
                        ERROR_SYN,
                        "at line %i: unexpected token [%s].\n",
                        token->lineno, token_enum_to_str(token->type));
    }
    return SUCCESS;
}

bool is_data_type(token_type type){
    switch(type){
        case TOKEN_INT:
        case TOKEN_FLOAT64:
        case TOKEN_STRING:
            return true;
        default:
           return false;
    }
}
int check_types(string* s1, string* s2){
    int error_code = SUCCESS;
    string tmp1, tmp2;

    str_init(&tmp1);
    str_init(&tmp2);
    str_copy(&tmp1,s1);
    str_copy(&tmp2,s2);

    if(s1->len == s2->len){
        for(int i = 0; i<s1->len; i++){
            if(tmp1.str[i] == '_')
                tmp2.str[i] = '_';
        }
        if(str_cmp(&tmp1,&tmp2))
            error_code = -1;
    }
    else
        error_code = -1;

    str_free(&tmp1);
    str_free(&tmp2);
    return error_code;
}