
#include "expression.h"

#define consume_token 1 //helper variable for MATCH macro
#define keep_token 0    //helper variable for MATCH macro


#define MATCH(x, y) do{                 \
  if (p->token->type != (x)){        \
   return handle_error(ERROR_SYN,      \
     "at line %i: expected token [%s] received [%s].\n", \
     p->token->lineno,token_enum_to_str((x)),token_enum_to_str(p->token->type));}\
  if((y))                               \
    CHECK(get_next_token(p), SUCCESS);  \
}while(0)




int get_next_token(parser_info* p){
    token_list_next(&p->token_list);
    p->token = p->token_list.act;
    return SUCCESS;
}

//int expression(parser_info *p) {//just for testing till expression module is working
//    int error_code;
//    st_item* item;
//    bool derive_type = true;
//    while(true){
//        switch(p->token->type){
//            case TOKEN_INTEGER:
//                if(derive_type){
//                    CHECK(str_add_char(&p->right_side_exp_types, TOKEN_INT), SUCCESS);
//                    derive_type = false;
//                }
//                CHECK(get_next_token(p), SUCCESS);
//                break;
//            case TOKEN_STR:
//                if(derive_type){
//                    CHECK(str_add_char(&p->right_side_exp_types, TOKEN_STRING), SUCCESS);
//                    derive_type = false;
//                }
//                CHECK(get_next_token(p), SUCCESS);
//                break;
//            case TOKEN_STRING:
//            case TOKEN_FLOAT:
//                if(derive_type){
//                    CHECK(str_add_char(&p->right_side_exp_types, TOKEN_FLOAT64), SUCCESS);
//                    derive_type = false;
//                }
//                CHECK(get_next_token(p), SUCCESS);
//                break;
//            case TOKEN_ID:
//                item = stack_lookup(p->local_st,&p->token->actual_value);
//                if(item == NULL)
//                    return ERROR_SEM_DEF;
//                if(derive_type){
//                    CHECK(str_add_char(&p->right_side_exp_types, item->data.as.variable.value_type), SUCCESS);
//                    derive_type = false;
//                }
//                CHECK(get_next_token(p), SUCCESS);
//                break;
//            case TOKEN_TRUE:
//            case TOKEN_FALSE:
//            case TOKEN_NOT:
//            case TOKEN_AND:
//            case TOKEN_OR:
//            case TOKEN_ADD:
//            case TOKEN_SUB:
//            case TOKEN_MUL:
//            case TOKEN_DIV:
//            case TOKEN_EQL:
//            case TOKEN_NEQ:
//            case TOKEN_LT:
//            case TOKEN_GT:
//            case TOKEN_LTE:
//            case TOKEN_GTE:
//                CHECK(get_next_token(p), SUCCESS);
//                break;
//            default:
//                goto out_of_while;
//        }
//    }
//    out_of_while:
//    return SUCCESS;
//}

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
        item = stack_lookup(p->local_st, &p->token->actual_value);
        if(item == NULL)//not defined beforehand
            return ERROR_SEM_DEF;
        CHECK(str_add_char(&p->left_side_vars_types,item->data.as.variable.value_type),SUCCESS);
        CHECK(get_next_token(p), SUCCESS);
        CHECK(assign(p), SUCCESS);
        MATCH(TOKEN_ASSIGN, consume_token);
        return end_assign(p);
    }

    return SUCCESS;
}

int for_def(parser_info *p) {
    int error_code;
    st_item* item;
    st_item* tmp;
    if (p->token->type == TOKEN_ID) {
        item = st_insert(&p->local_st->local_table, &p->token->actual_value, type_variable, &error_code);
        if(item->data.defined)
            return ERROR_SEM_DEF;
        item->data.defined = true;

        CHECK(get_next_token(p), SUCCESS);
        MATCH(TOKEN_DEFINITION, consume_token);

        if(p->token->type == TOKEN_ID){
            if((tmp = stack_lookup(p->local_st,&p->token->actual_value)) == NULL)
                return handle_error(ERROR_SEM_DEF,"at line %i: variable \"%s\" is not defined.\n",
                                    p->token->lineno, p->token->actual_value.str);
            item->data.as.variable.value_type = tmp->data.as.variable.value_type;
        }
        else if(p->token->type == TOKEN_STR)
            item->data.as.variable.value_type = type_str;
        else if(p->token->type == TOKEN_INTEGER)
            item->data.as.variable.value_type = type_int;
        else if(p->token->type == TOKEN_FLOAT)
            item->data.as.variable.value_type = type_float;
        else{
            ;//expression handles that type of error
        }

        return expression(p);
    }
    return SUCCESS;
}

int return_exp(parser_info *p) {
    int error_code;
    if (p->token->type == TOKEN_EOL) {
        if(p->in_function->data.as.function.ret_types.len != 0)
            return ERROR_SEM_PAR;
        else
            return SUCCESS;
    } else {
        //parser information about index of a currently proccessed expression??
        //current token not processed yet
        CHECK(expression(p), SUCCESS);
        CHECK(exp_n(p), SUCCESS);

        return SUCCESS;
    }
}

int term_n(parser_info *p) {
    int error_code;
    st_item* item;

    if (p->token->type == TOKEN_COMMA) {
        CHECK(get_next_token(p), SUCCESS);
        CHECK(EOL_opt(p), SUCCESS);
        switch (p->token->type) {
            case TOKEN_ID:
                if((item = stack_lookup(p->local_st, &p->token->actual_value)) == NULL)
                    return ERROR_SEM_DEF;
                CHECK(str_add_char(&p->right_side_exp_types, item->data.as.variable.value_type), SUCCESS);
                break;
            case TOKEN_INTEGER:
                CHECK(str_add_char(&p->right_side_exp_types, TOKEN_INT), SUCCESS);
                break;
            case TOKEN_FLOAT:
                CHECK(str_add_char(&p->right_side_exp_types, TOKEN_FLOAT64), SUCCESS);
                break;
            case TOKEN_STR:
                CHECK(str_add_char(&p->right_side_exp_types, TOKEN_STRING), SUCCESS);
                break;
            case TOKEN_BOOLEAN:
                CHECK(str_add_char(&p->right_side_exp_types, TOKEN_BOOL), SUCCESS);
                break;
            default:
                return handle_error(ERROR_SYN,
                                    "at line %i: expected literal received [%s].\n",
                                    p->token->lineno, token_enum_to_str(p->token->type));
        }
        CHECK(get_next_token(p), SUCCESS);
        return term_n(p);
    } else
        return SUCCESS;


}

int call_params(parser_info *p) {
    int error_code;
    st_item* item;
    str_reinit(&p->right_side_exp_types);

    switch (p->token->type) {
        case TOKEN_ID:
            if((item = stack_lookup(p->local_st, &p->token->actual_value)) == NULL)
                return ERROR_SEM_DEF;
            CHECK(str_add_char(&p->right_side_exp_types, item->data.as.variable.value_type), SUCCESS);
            break;
        case TOKEN_INTEGER:
            CHECK(str_add_char(&p->right_side_exp_types, TOKEN_INT), SUCCESS);
            break;
        case TOKEN_FLOAT:
            CHECK(str_add_char(&p->right_side_exp_types, TOKEN_FLOAT64), SUCCESS);
            break;
        case TOKEN_STR:
            CHECK(str_add_char(&p->right_side_exp_types, TOKEN_STRING), SUCCESS);
            break;
        case TOKEN_BOOLEAN:
            CHECK(str_add_char(&p->right_side_exp_types, TOKEN_BOOL), SUCCESS);
            break;
        default:
            return SUCCESS;
    }
    CHECK(get_next_token(p), SUCCESS);
    CHECK( term_n(p), SUCCESS);

    return SUCCESS;
}

int func_call(parser_info *p) {
    int error_code;
    MATCH(TOKEN_LBRACKET, consume_token);
    CHECK(EOL_opt(p), SUCCESS);
    CHECK(call_params(p), SUCCESS);
    MATCH(TOKEN_RBRACKET, consume_token);

    CHECK(check_types(&p->left_side_vars_types,&p->function_called->data.as.function.ret_types), SUCCESS);
    if(!strcmp(p->function_called->key.str,"print"))
        goto end_func_call;
    if(str_cmp(&p->function_called->data.as.function.param_types, &p->right_side_exp_types)) //parameter types or number off parameter does not match
        return ERROR_SEM_PAR;
    end_func_call:
    return SUCCESS;
}


int exp_n(parser_info *p) {

    int error_code;
    if (p->token->type == TOKEN_COMMA) {
        CHECK(get_next_token(p), SUCCESS);
        CHECK(expression(p), SUCCESS);
        CHECK(exp_n(p), SUCCESS);

        return SUCCESS;

    } else
        return SUCCESS;
}

int end_assign(parser_info *p) {
    int error_code;
    st_item *item;
    if (p->token->type == TOKEN_ID) {
        item = stack_lookup(p->local_st, &p->token->actual_value);
        if (item == NULL)
            item = st_get_item(&p->st, &p->token->actual_value);
        if (item != NULL) {
            if( item->data.type == type_function){
                p->function_called = item;
                CHECK(get_next_token(p), SUCCESS);
                return func_call(p);
            }
        }
        else//identifier not defined
            return ERROR_SEM_DEF;
    }
    str_reinit(&p->right_side_exp_types);
    CHECK(expression(p), SUCCESS);//call to expression and then subsequent calls to expression in exp_n
    CHECK(exp_n(p), SUCCESS);

    CHECK(check_types(&p->left_side_vars_types,&p->right_side_exp_types), SUCCESS);
    if(str_cmp(&p->left_side_vars_types,&p->right_side_exp_types))
        return ERROR_SEM_PAR;

    return SUCCESS;
}

int assign(parser_info *p) {
    int error_code;
    st_item* item;

    if (p->token->type == TOKEN_COMMA) {
        CHECK(get_next_token(p), SUCCESS);
        CHECK(EOL_opt(p), SUCCESS);
        //next token set
        MATCH(TOKEN_ID, keep_token);
        if(!strcmp(p->token->actual_value.str,"_")){
            CHECK(str_add_char(&p->left_side_vars_types,'_'),SUCCESS);
            goto assign_underscore;
        }

        item = stack_lookup(p->local_st, &p->token->actual_value);
        if(item == NULL)//not defined beforehand
            return ERROR_SEM_DEF;

        CHECK(str_add_char(&p->left_side_vars_types,item->data.as.variable.value_type),SUCCESS);
        assign_underscore:

        CHECK(get_next_token(p),SUCCESS);
        return assign(p);
    } else // eps
        return SUCCESS;
}


int var(parser_info *p) {
    int error_code;
    st_item* item;
    st_item* tmp;

    switch (p->token->type) {
        case TOKEN_DEFINITION:
            if(!strcmp(p->token->prev->actual_value.str,"_"))
                return ERROR_SEM_DEF;
            item = st_insert(&p->local_st->local_table, &p->token->prev->actual_value, type_variable,&error_code);
            if (item == NULL)
                return ERROR_TRANS;
            if (item->data.defined) //redefinition of local variable
                return ERROR_SEM_DEF;
            item->data.defined = true;
            if(p->token->next->type == TOKEN_ID){
                if((tmp = stack_lookup(p->local_st,&p->token->next->actual_value)) == NULL)
                    return handle_error(ERROR_SEM_DEF,"at line %i: variable \"%s\" is not defined.\n",
                                        p->token->lineno, p->token->next->actual_value.str);
                item->data.as.variable.value_type = tmp->data.as.variable.value_type;
            }
            else if(p->token->next->type == TOKEN_STR)
                item->data.as.variable.value_type = type_str;
            else if(p->token->next->type == TOKEN_INTEGER)
                item->data.as.variable.value_type = type_int;
            else if(p->token->next->type == TOKEN_FLOAT)
                item->data.as.variable.value_type = type_float;
            else{
                ;//expression handles that type of error
            }
            CHECK(get_next_token(p), SUCCESS);
            CHECK(expression(p), SUCCESS);
            //next token set
            break;

        case TOKEN_LBRACKET:
            item = st_get_item(&p->st, &p->token->prev->actual_value);
            if(item == NULL)
                return ERROR_SEM_DEF;
            str_reinit(&p->left_side_vars_types);
            p->function_called = item;
            CHECK(func_call(p), SUCCESS);
            break;

        case TOKEN_ASSIGN:
            str_reinit(&p->left_side_vars_types);

            if(!strcmp(p->token->prev->actual_value.str,"_")) {
                CHECK(str_add_char(&p->left_side_vars_types,'_'),SUCCESS);
                goto after_item_search;
            }

            item = stack_lookup(p->local_st, &p->token->prev->actual_value);
            if(item == NULL)//not defined beforehand
                return ERROR_SEM_DEF;
            CHECK(str_add_char(&p->left_side_vars_types,item->data.as.variable.value_type),SUCCESS);
        after_item_search:
            CHECK(get_next_token(p), SUCCESS);
            CHECK(EOL_opt(p), SUCCESS);
            //next token set
            CHECK(end_assign(p), SUCCESS);
            break;
        case TOKEN_COMMA:
            str_reinit(&p->left_side_vars_types);
            if(!strcmp(p->token->prev->actual_value.str,"_")) {
                CHECK(str_add_char(&p->left_side_vars_types,'_'),SUCCESS);
                goto after_item_search2;
            }
            item = stack_lookup(p->local_st, &p->token->prev->actual_value);
            if(item == NULL)//not defined beforehand
                return ERROR_SEM_DEF;
            CHECK(str_add_char(&p->left_side_vars_types,item->data.as.variable.value_type),SUCCESS);
        after_item_search2:
            //CHECK(get_next_token(p), SUCCESS);
            CHECK(assign(p), SUCCESS);
            //next token set
            MATCH(TOKEN_ASSIGN, consume_token);
            CHECK(EOL_opt(p), SUCCESS);
            //next token set
            CHECK(end_assign(p), SUCCESS);
            break;
        default:
            return handle_error(ERROR_SYN,
                                "at line %i: unexpected token [%s].\n",
                                p->token->lineno, token_enum_to_str(p->token->type));
    }
    return SUCCESS;

}

int statement(parser_info *p) {
    int error_code;
    if (p->token->type == TOKEN_ID) {
        CHECK(get_next_token(p), SUCCESS);
        CHECK(var(p), SUCCESS);

    } else if (p->token->type == TOKEN_IF) {
        CHECK(get_next_token(p), SUCCESS);
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
        CHECK(get_next_token(p), SUCCESS);
        CHECK(EOL_opt(p), SUCCESS);
        //next token set
        CHECK(enter_scope(&p->local_st, &p->scope), SUCCESS);
        CHECK(for_def(p), SUCCESS);
        //next token set
        MATCH(TOKEN_SEMICOLON, consume_token);
        CHECK(EOL_opt(p), SUCCESS);
        CHECK(expression(p), SUCCESS);
        //next token set
        //CHECK(get_next_token(p),SUCCESS);

        MATCH(TOKEN_SEMICOLON, consume_token);
        CHECK(EOL_opt(p), SUCCESS);
        //next token set
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
        //+semantic check for current function(p->in_func.as.function.ret_types) return types
        str_reinit(&p->right_side_exp_types);
        CHECK(get_next_token(p), SUCCESS);
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
        CHECK(get_next_token(p), SUCCESS);
        CHECK(EOL_opt(p), SUCCESS);

        //next token set
        switch (p->token->type) {// , ->Type<- Ret_type_n
            case TOKEN_INT:
            case TOKEN_FLOAT64:
            case TOKEN_STRING:
                break;
            default:
                return handle_error(ERROR_SYN,
                                    "at line %i: expected type specifier received [%s].\n",
                                    p->token->lineno, token_enum_to_str(p->token->type));
        }
        CHECK(get_next_token(p), SUCCESS);
        return ret_type_n(p);//, Type ->Ret_type_n<-


    } else
        return SUCCESS; //eps


}

//Ret_type -> ( Type Ret_type_n ) | Type
int ret_type(parser_info *p) {
    int error_code;
    bool in_paren = false;

    if (p->token->type == TOKEN_LBRACKET) {
        in_paren = true;
        CHECK(get_next_token(p), SUCCESS);
    }

    switch (p->token->type) {//type -> int | float | string | bool
        case TOKEN_INT:
        case TOKEN_FLOAT64:
        case TOKEN_STRING:
            break;
        case TOKEN_RBRACKET:
            if (!in_paren)//wrong syntax
                return handle_error(ERROR_SYN,
                                    "at line %i: unexpected [%s].\n",
                                    p->token->lineno, token_enum_to_str(p->token->type));
            else
                in_paren = false;
            break;
        default:
            goto end; //token not processed, ret_type -> eps or syntax error
    }
    CHECK(get_next_token(p), SUCCESS);
    if (in_paren) {//( Type ->Ret_type_n<- )
        CHECK(ret_type_n(p), SUCCESS);
        //next token set
        MATCH(TOKEN_RBRACKET, consume_token);
    }
    //set next token for consistency
    end:
    return SUCCESS;
}


//Params_n -> , id Type Params_n | eps
int params_n(parser_info *p) {
    int error_code;
    if (p->token->type == TOKEN_COMMA) {
        CHECK(get_next_token(p), SUCCESS);
        CHECK(EOL_opt(p), SUCCESS);

        // next token set
        if (p->token->type == TOKEN_ID) {// ->id<- Type Params_n
            st_item *item = st_insert(&p->local_st->local_table, &p->token->actual_value, type_variable, &p->internal_error);
            if (item == NULL)
                return ERROR_TRANS;
            if (item->data.defined) //redefinition of local variables
                return ERROR_SEM_DEF;
            else
                item->data.defined = true;

            CHECK(get_next_token(p), SUCCESS);

            switch (p->token->type) {//id ->Type<- Params_n
                case TOKEN_INT:
                case TOKEN_FLOAT64:
                case TOKEN_STRING:
                    item->data.as.variable.value_type = p->token->type;
                    break;
                default:
                    return handle_error(ERROR_SYN,
                                        "at line %i: expected type specifier received [%s].\n",
                                         p->token->lineno, token_enum_to_str(p->token->type));
                    //break;
            }//everything ok, received id and type specifier
            CHECK(get_next_token(p), SUCCESS);
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
        st_item *item = st_insert(&p->local_st->local_table, &p->token->actual_value, type_variable, &p->internal_error);
        if (item == NULL)
            return ERROR_TRANS;
        if (item->data.defined) //redefinition of local variables
            return ERROR_SEM_DEF;
        else
            item->data.defined = true;

        CHECK(get_next_token(p), SUCCESS);

        switch (p->token->type) {//id ->Type<- Params_n
            case TOKEN_INT:
            case TOKEN_FLOAT64:
            case TOKEN_STRING:
                item->data.as.variable.value_type = p->token->type;
                break;
            default:
                return handle_error(ERROR_SYN,
                                    "at line %i: expected type specifier received [%s].\n",
                                    p->token->lineno, token_enum_to_str(p->token->type));
                //break;
        }//everything ok, received type specifier
        CHECK(get_next_token(p), SUCCESS);
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
    CHECK(get_next_token(p), SUCCESS);

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
        CHECK(get_next_token(p), SUCCESS);
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
    CHECK(get_next_token(p), SUCCESS);
    MATCH(TOKEN_EOL, consume_token);
    CHECK(EOL_opt(p), SUCCESS);

    return prog(p);
}

int EOL_opt(parser_info *p) {
    int error_code;
    while (p->token->type == TOKEN_EOL) {
        CHECK(get_next_token(p), SUCCESS);
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
    else if((error_code = fill_with_builtin(&p.st)) != SUCCESS){
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

    st_item *func;

    get_next_token(p);//get the first token

    while(p->token->type != TOKEN_EOF){
        //package main at the start of a valid IFJ code so we can safely skip 1 token;
        get_next_token(p);
        if(p->token->type != TOKEN_FUNC)
            continue; //only interested in the function header

        get_next_token(p);
        if(p->token->type != TOKEN_ID) //incorrect syntax skip
            continue;

        func = st_insert(&p->st,
                         &p->token->actual_value,
                         type_function,
                         &p->internal_error);
        if (func == NULL)
            return ERROR_TRANS;
        if (func->data.defined) //redefinition of function/variable
            return handle_error(ERROR_SEM_DEF,
                                "at line %i: redefinition of function \"%s\".\n",
                                p->token->lineno, func->key.str);
        func->data.defined = true;

        while(p->token->type != TOKEN_EOL && p->token->type != TOKEN_EOF){
            get_next_token(p);
            if(p->token->type == TOKEN_ID){
                str_concat(&func->data.as.function.params, p->token->actual_value.str, p->token->actual_value.len);
                str_add_char(&func->data.as.function.params,'#');

                get_next_token(p);

                if(p->token->type != TOKEN_INT &&
                p->token->type != TOKEN_FLOAT64 &&
                p->token->type != TOKEN_STRING)
                    return handle_error(ERROR_SYN,
                                        "at line %i: incorrect syntax of a function header of \"%s\".\n",
                                         p->token->lineno,func->key.str);
                else{
                    str_add_char(&func->data.as.function.param_types, p->token->type);
                }
            }
            else if(p->token->type == TOKEN_INT)
                str_add_char(&func->data.as.function.ret_types, p->token->type);
            else if(p->token->type == TOKEN_FLOAT64)
                str_add_char(&func->data.as.function.ret_types, p->token->type);
            else if(p->token->type == TOKEN_STRING)
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
//func len(string) (int)
//func substr(s string, i int, n int) (string, int)
//func ord(s string, i int) (int, int)
//func chr(i int) (string, int)

int fill_with_builtin(symbol_table* st){
    int error_code;
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

    //float= \005,  int = \011,  string = \014,
    string name;
    str_init(&name);
    for(int i = 0; i<10;i++){
        str_reinit(&name);
        str_concat(&name,func[i][0],strlen(func[i][0]));
        st_item *item = st_insert(st,&name, type_function, &error_code);

        if(item == NULL)
            return ERROR_TRANS;

        str_concat(&item->data.as.function.params,func[i][1],strlen(func[i][1]));
        str_concat(&item->data.as.function.param_types,func[i][2],strlen(func[i][2]));
        str_concat(&item->data.as.function.ret_types,func[i][3],strlen(func[i][3]));

        item->data.defined = true;
    }
    str_free(&name);
    return SUCCESS;
}

int check_main(symbol_table* st){
    string name;
    st_item* item;
    str_init(&name);
    str_concat(&name,"main",4);
    item = st_get_item(st,&name);
    if(item == NULL)
        return ERROR_SEM_DEF;
    if(strcmp(item->data.as.function.param_types.str,""))
        return ERROR_SEM_PAR;
    if(strcmp(item->data.as.function.ret_types.str,""))
        return ERROR_SEM_PAR;
    str_free(&name);
    return SUCCESS;
}

int token_type_to_data_type(token_type type){
    switch(type){
        case TOKEN_INT:
        case TOKEN_INTEGER:
            return type_int;
        case TOKEN_FLOAT:
        case TOKEN_FLOAT64:
            return type_float;
        case TOKEN_STR:
        case TOKEN_STRING:
            return type_str;
        default:
            return -1;
    }
}

int check_types(string* s1, string* s2){
    int error_code = SUCCESS;
    string tmp1;
    string tmp2;
    str_init(&tmp1);
    str_init(&tmp2);
    str_copy(&tmp1,s1);
    str_copy(&tmp2,s2);

    if(s1->len != s2->len ){
        error_code = ERROR_SEM_PAR;
        goto after_for99;
    }

    for(int i = 0; i<s1->len; i++){
        if(tmp1.str[i] == '_')
            tmp2.str[i] = '_';
    }
    if(str_cmp(&tmp1,&tmp2))
        error_code = ERROR_SEM_PAR;
    after_for99:
    str_free(&tmp1);
    str_free(&tmp2);
    return error_code;
}