
#include "error.h"
#include "parser.h"
#include "scanner.h"

#define consume_token 1 //helper variable for MATCH macro
#define keep_token 0    //helper variable for MATCH macro


int token_pos = 0; //just for testing

#define MATCH(x, y) do{                      \
  if (p->token.type != (x)){                  \
    p->syntax_error_token_position = token_pos;\
    return ERROR_SYN;}                          \
  if((y))                                        \
    CHECK(get_next_token(&p->token), SUCCESS);    \
}while(0)


int expression(parser_info* p){
    int error_code;
    CHECK(get_next_token(&p->token),SUCCESS);
    return SUCCESS;
}

int prolog(parser_info* p);
int prog(parser_info* p);
int func(parser_info* p);
int EOL_opt(parser_info* p);
int params(parser_info* p);
int params_n(parser_info* p);
int ret_type(parser_info* p);
int ret_type_n(parser_info* p);
int statement_list(parser_info* p);
int statement(parser_info* p);
int var(parser_info* p);
int assign(parser_info* p);
int end_assign(parser_info* p);
int exp_n(parser_info* p);
int for_def(parser_info* p);
int for_assign(parser_info* p);
int func_call(parser_info* p);
int call_params(parser_info* p);
int term_n(parser_info* p);
int return_exp(parser_info* p);

int for_assign(parser_info* p){
    int error_code;
    if(p->token.type == TOKEN_ID){
        CHECK(get_next_token(&p->token),SUCCESS);
        CHECK(assign(p),SUCCESS);
        MATCH(TOKEN_ASSIGN, consume_token);
        return end_assign(p);
    }
    return SUCCESS;
}

int for_def(parser_info* p){
    int error_code;
    if(p->token.type == TOKEN_ID){
        CHECK(get_next_token(&p->token),SUCCESS);
        MATCH(TOKEN_DEFINITION, consume_token);
        return expression(p);
    }
    return SUCCESS;

}

int return_exp(parser_info* p){
    int error_code;
    if(p->token.type == TOKEN_EOL){
        return SUCCESS;
    }
    else{
        //current token not processed yet
        CHECK(expression(p), SUCCESS);
        return exp_n(p);
    }
}

int term_n(parser_info* p){
    int error_code;
    if(p->token.type == TOKEN_COMMA){
        CHECK(get_next_token(&p->token),SUCCESS);
        CHECK(EOL_opt(p),SUCCESS);
        switch(p->token.type){
            case TOKEN_INTEGER:
                break;
            case TOKEN_FLOAT:
                break;
            case TOKEN_STR:
                break;
//            case TOKEN_BOOLEAN:
//                break;
            default:
                return ERROR_SYN;
        }
        CHECK(get_next_token(&p->token),SUCCESS);
        return term_n(p);
    }
    else
        return SUCCESS;


}

int call_params(parser_info* p){
    int error_code;
    switch(p->token.type){
        case TOKEN_INTEGER:
            break;
        case TOKEN_FLOAT:
            break;
        case TOKEN_STR:
            break;
//        case TOKEN_BOOLEAN:
//            break;
        default:
            return SUCCESS;
    }
    CHECK(get_next_token(&p->token),SUCCESS);
    return term_n(p);
}

int func_call(parser_info* p){
    int error_code;
    MATCH(TOKEN_LBRACKET, consume_token);
    CHECK(EOL_opt(p),SUCCESS);
    CHECK(call_params(p),SUCCESS);
    MATCH(TOKEN_RBRACKET, consume_token);
    return SUCCESS;
}


int exp_n(parser_info* p){

    int error_code;
    if(p->token.type == TOKEN_COMMA){
        CHECK(get_next_token(&p->token),SUCCESS);
        CHECK(expression(p), SUCCESS);
        return exp_n(p);
    }
    else
        return SUCCESS;

}

int end_assign(parser_info* p){
    int error_code;
    st_item* item;
    if(p->token.type == TOKEN_ID){
        item = st_get_item(&p->st, &p->token.actual_value);
        if(item != NULL){
            CHECK(get_next_token(&p->token),SUCCESS);
            return func_call(p);
        }
    }
    CHECK(expression(p), SUCCESS);//call to expression and then subsequent calls to expression in exp_n
    return exp_n(p);
}

int assign(parser_info* p){
    int error_code;

    if(p->token.type == TOKEN_COMMA){
        CHECK(get_next_token(&p->token),SUCCESS);
        CHECK(EOL_opt(p),SUCCESS);
        //next token set
        MATCH(TOKEN_ID, consume_token);
        return assign(p);
    }
    else // eps
        return SUCCESS;
}


int var(parser_info* p){
    int error_code;

    switch(p->token.type){
        case TOKEN_DEFINITION:
            CHECK(get_next_token(&p->token),SUCCESS);
            CHECK(expression(p),SUCCESS);
            //next token set
            //CHECK(get_next_token(&p->token),SUCCESS);
            break;
        case TOKEN_LBRACKET:
            CHECK(func_call(p),SUCCESS);
            break;
        case TOKEN_ASSIGN:
            CHECK(get_next_token(&p->token),SUCCESS);
            CHECK(EOL_opt(p),SUCCESS);
            //next token set
            CHECK(end_assign(p),SUCCESS);
            break;
        case TOKEN_COMMA:
            //CHECK(get_next_token(&p->token), SUCCESS);
            CHECK(assign(p),SUCCESS);
            //next token set
            MATCH(TOKEN_ASSIGN , consume_token);
            CHECK(EOL_opt(p),SUCCESS);
            //next token set
            CHECK(end_assign(p),SUCCESS);
            break;
        default:
            return ERROR_SYN;
    }
    return SUCCESS;

}

int statement(parser_info* p){
    int error_code;
    if(p->token.type == TOKEN_ID){
        CHECK(get_next_token(&p->token),SUCCESS);
        CHECK(var(p), SUCCESS);
    }
    else if(p->token.type == TOKEN_IF){
        CHECK(get_next_token(&p->token),SUCCESS);
        CHECK(expression(p), SUCCESS);
        //next token set

        MATCH(TOKEN_LCURLY, consume_token);
        //scope++
        MATCH(TOKEN_EOL, consume_token);
        CHECK(EOL_opt(p),SUCCESS);

        //next token set
        CHECK(statement_list(p), SUCCESS);

        //next token set
        MATCH(TOKEN_RCURLY,consume_token);
        MATCH(TOKEN_ELSE, consume_token);

        MATCH(TOKEN_LCURLY, consume_token);
        //scope++
        MATCH(TOKEN_EOL, consume_token);
        CHECK(EOL_opt(p), SUCCESS);

        //next token set
        CHECK(statement_list(p), SUCCESS);
        //next token set
        MATCH(TOKEN_RCURLY , consume_token);
    }
    else if(p->token.type == TOKEN_FOR){
        CHECK(get_next_token(&p->token),SUCCESS);
        CHECK(EOL_opt(p), SUCCESS);
        //next token set
        CHECK(for_def(p), SUCCESS);
        //next token set
        MATCH(TOKEN_SEMICOLON , consume_token);
        CHECK(EOL_opt(p), SUCCESS);
        CHECK(expression(p), SUCCESS);
        //next token set
        //CHECK(get_next_token(&p->token),SUCCESS);

        MATCH(TOKEN_SEMICOLON , consume_token);
        CHECK(EOL_opt(p), SUCCESS);
        //next token set
        CHECK(for_assign(p), SUCCESS);
        //next token set
        MATCH(TOKEN_LCURLY , consume_token);
        MATCH(TOKEN_EOL , consume_token);
        CHECK(EOL_opt(p), SUCCESS);
        //next token set
        CHECK(statement_list(p), SUCCESS);
        //next token set
        MATCH(TOKEN_RCURLY , consume_token);
    }
    else if(p->token.type == TOKEN_RETURN){
        //+semantic check for current function(p->in_func.as.function.ret_types) return types
        CHECK(get_next_token(&p->token),SUCCESS);
        return return_exp(p);
    }
    else{;}

    return SUCCESS;

}
//Statement_list -> Statement EOL EOL_opt Statement_list | eps
int statement_list(parser_info* p){
    int error_code;

    switch(p->token.type){
        case TOKEN_ID:
        case TOKEN_IF:
        case TOKEN_FOR:
        case TOKEN_RETURN:
            CHECK(statement(p),SUCCESS);
            MATCH(TOKEN_EOL, consume_token);
            CHECK(EOL_opt(p),SUCCESS);
            return statement_list(p);

        default:
            return SUCCESS;
    }
}

//Ret_type_n -> , Type Ret_type_n | eps
int ret_type_n(parser_info* p){
    int error_code;

    //next token set already

    if(p->token.type == TOKEN_COMMA){//->,<- Type Ret_type_n
        CHECK(get_next_token(&p->token), SUCCESS);
        CHECK(EOL_opt(p), SUCCESS);

        //next token set
        switch (p->token.type) {// , ->Type<- Ret_type_n
            case TOKEN_INT:
                str_add_char(&p->in_function->data.as.function.ret_types, 'i');
                break;
            case TOKEN_FLOAT64:
                str_add_char(&p->in_function->data.as.function.ret_types, 'f');
                break;
            case TOKEN_STRING:
                str_add_char(&p->in_function->data.as.function.ret_types, 's');
                break;
            case TOKEN_BOOL:
                str_add_char(&p->in_function->data.as.function.ret_types, 'b');
                break;
            default:
                return ERROR_SYN;
        }
        CHECK(get_next_token(&p->token), SUCCESS);
        return ret_type_n(p);//, Type ->Ret_type_n<-


    }else
        return SUCCESS; //eps


}

//Ret_type -> ( Type Ret_type_n ) | Type
int ret_type(parser_info* p){
    int error_code;
    bool in_paren = false;

    if(p->token.type == TOKEN_LBRACKET){
        in_paren = true;
        CHECK(get_next_token(&p->token), SUCCESS);
    }

    switch(p->token.type) {//type -> int | float | string | bool
        case TOKEN_INT:
            str_add_char(&p->in_function->data.as.function.ret_types, 'i');
            break;
        case TOKEN_FLOAT64:
            str_add_char(&p->in_function->data.as.function.ret_types, 'f');
            break;
        case TOKEN_STRING:
            str_add_char(&p->in_function->data.as.function.ret_types, 's');
            break;
        case TOKEN_BOOL:
            str_add_char(&p->in_function->data.as.function.ret_types, 'b');
            break;
        case TOKEN_RBRACKET:
            if(!in_paren)//wrong syntax
                return ERROR_SYN;
            else
                in_paren = false;
            break;
        default:
            goto end; //token not processed, ret_type -> eps or syntax error
    }
    CHECK(get_next_token(&p->token), SUCCESS);
    if(in_paren){//( Type ->Ret_type_n<- )
        CHECK(ret_type_n(p), SUCCESS);
        //next token set
        MATCH(TOKEN_RBRACKET, consume_token);
    }
    //set next token for consistency
    end:
    return SUCCESS;
}


//Params_n -> , id Type Params_n | eps
int params_n(parser_info* p){
    int error_code;
    if(p->token.type == TOKEN_COMMA){
        CHECK(get_next_token(&p->token),SUCCESS);
        CHECK(EOL_opt(p), SUCCESS);

        // next token set
        if(p->token.type == TOKEN_ID){// ->id<- Type Params_n

            st_item* item = st_insert(&p->local_st,&p->token.actual_value,type_variable, &p->internal_error);
            if(item == NULL)
                return ERROR_TRANS;
            if(item->data.defined) //redefinition of local variables
                return ERROR_SEM_DEF;

            CHECK(get_next_token(&p->token),SUCCESS);

            switch(p->token.type){//id ->Type<- Params_n
                case TOKEN_INT:
                    item->data.as.variable.value_type = type_int;
                    break;
                case TOKEN_FLOAT64:
                    item->data.as.variable.value_type = type_float;
                    break;
                case TOKEN_STRING:
                    item->data.as.variable.value_type = type_str;
                    break;
                case TOKEN_BOOL:
                    item->data.as.variable.value_type = type_bool;
                    break;
                default:
                    return ERROR_SYN;
                    //break;
            }//everything ok, received id and type specifier
            CHECK(get_next_token(&p->token),SUCCESS);
            return params_n(p); //, id Type ->Params_n<-
        }
        else
            return ERROR_SYN;

    }else//Params_n -> eps
        return SUCCESS;
}

//Params -> id Type Params_n | eps
int params(parser_info* p){
    int error_code;

    CHECK(EOL_opt(p), SUCCESS);
    // next token set
    if(p->token.type == TOKEN_ID){// ->id<- Type Params_n | else eps

        st_item* item = st_insert(&p->local_st,&p->token.actual_value,type_variable, &p->internal_error);
        if(item == NULL)
            return ERROR_TRANS;
        if(item->data.defined) //redefinition of local variables
            return ERROR_SEM_DEF;

        CHECK(get_next_token(&p->token),SUCCESS);

        switch(p->token.type){//id ->Type<- Params_n
            case TOKEN_INT:
                item->data.as.variable.value_type = type_int;
                break;
            case TOKEN_FLOAT64:
                item->data.as.variable.value_type = type_float;
                break;
            case TOKEN_STRING:
                item->data.as.variable.value_type = type_str;
                break;
            case TOKEN_BOOL:
                item->data.as.variable.value_type = type_bool;
                break;
            default:
                return ERROR_SYN;
                //break;
        }//everything ok, received type specifier
        CHECK(get_next_token(&p->token),SUCCESS);
        //id Type ->Params_n<-
        return params_n(p);
    }
    else //Params -> eps
        return SUCCESS;


}
//Func -> id ( Params ) Ret_type { EOL EOL_opt Statement_list }
int func(parser_info* p){
    int error_code;
    MATCH(TOKEN_ID, keep_token);
    st_item* item = st_insert(&p->st,&p->token.actual_value,type_function, &p->internal_error);
    if(item == NULL)
        return ERROR_TRANS;
    if(item->data.defined) //redefinition of function/variable
        return ERROR_SEM_DEF;

    p->in_function = item;

    CHECK(get_next_token(&p->token), SUCCESS);

    MATCH(TOKEN_LBRACKET, consume_token);
    CHECK(params(p),SUCCESS);
    MATCH(TOKEN_RBRACKET, consume_token);

    CHECK(ret_type(p),SUCCESS);

    MATCH(TOKEN_LCURLY, consume_token);
    MATCH(TOKEN_EOL, consume_token);
    CHECK(EOL_opt(p), SUCCESS);
    //next token set
    CHECK(statement_list(p), SUCCESS);
    //next token set
    MATCH(TOKEN_RCURLY, consume_token);

    item->data.defined = true;

    return SUCCESS;
}


//Prog -> func Func EOL EOL_opt Prog
//Prog -> EOF
int prog(parser_info* p){
    int error_code;

    if(p->token.type == TOKEN_FUNC){//Prog - > func ...
        CHECK(get_next_token(&p->token), SUCCESS);
        CHECK(func(p), SUCCESS);
        MATCH(TOKEN_EOL, consume_token);
        CHECK(EOL_opt(p), SUCCESS);
        // next token set
        return prog(p);
    }
    else if(p->token.type == TOKEN_EOF){//Prog -> EOF
        return SUCCESS;
    }
    else{
        return ERROR_SYN;
    }

}

//Prolog -> package id EOL EOL_opt Prog
int prolog(parser_info* p){
    int error_code;
    MATCH(TOKEN_PACKAGE , consume_token);
    MATCH(TOKEN_ID, keep_token);
    if(str_cmp_c_str(&p->token.actual_value,"main"))//package identifier does not match "main"
        return ERROR_SEM_OTHER;
    //token not consumed , get next
    CHECK(get_next_token(&p->token), SUCCESS);
    MATCH(TOKEN_EOL, consume_token);
    CHECK(EOL_opt(p),SUCCESS);

    return prog(p);
}

int EOL_opt(parser_info* p){
    int error_code;
    while(p->token.type == TOKEN_EOL){
        CHECK(get_next_token(&p->token),SUCCESS);
    }
    return SUCCESS;
}

/*parser driver code
 * returns error code*/
int parser(){
    int error_code;
    parser_info p;

    token_init(&p.token);
    st_init(&p.st);
    st_init(&p.local_st);

    if ((error_code = get_next_token(&p.token)) != SUCCESS){
            str_free(&p.token.actual_value);
            st_dispose(&p.st);
            st_dispose(&p.local_st);
            return error_code;
    }
    else{
        error_code = prolog(&p);
        if(error_code == ERROR_SYN){
            printf("Syntax error at token [%i]\n", p.syntax_error_token_position);
        }
        str_free(&p.token.actual_value);
        st_dispose(&p.st);
        st_dispose(&p.local_st);
        return error_code;
    }


}



