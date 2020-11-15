
#include "error.h"
#include "parser.h"
#include "scanner.h"

#define CHECK(x, y) do { \
  int error_code = (x); \
  if (error_code != y) { \
    return error_code; \
  } \
} while (0)




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
int def_var(parser_info* p);
int var(parser_info* p);
int assign(parser_info* p);
int end_assign(parser_info* p);
int exp_end_assign(parser_info* p);
int exp_n(parser_info* p);
int statement(parser_info* p);
int for_def(parser_info* p);
int for_assign(parser_info* p);
int func_call(parser_info* p);
int call_params(parser_info* p);
int type(parser_info* p);
int term(parser_info* p);
int term_n(parser_info* p);
int return_exp(parser_info* p);



//Prolog -> package id EOL EOL_opt Prog
int prolog(parser_info* p){
    int error_code;
    if(p->token.type == TOKEN_PACKAGE){
        CHECK(get_next_token(&p->token), SUCCESS);
        if(p->token.type == TOKEN_ID){
            if(str_cmp_c_str(&p->token.attribute,"main"))//identifier does not match "main"
                return ERROR_SEM_OTHER;
            else{
                CHECK(get_next_token(&p->token), SUCCESS);
                if(p->token.type == TOKEN_EOL){
                    do{//EOL_opt
                        CHECK(get_next_token(&p->token), SUCCESS);
                    }while(p->token.type == TOKEN_EOL);
                    error_code = prog(p);
                    return error_code;
                }
                else{
                    return ERROR_SYN;
                }
            }
        }
        else{
            return ERROR_SYN;
        }
    }
}

int token_init(token_t* token){
    int ret;
    token->type = TOKEN_ERROR;
    token->lineno = -1;
    token->pos = -1;
    if((ret = str_init(&token->attribute)) == SUCCESS)
        return SUCCESS;
    else
        return ret;
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
            return error_code;
    }
    else{
        error_code = prolog(&p);
        return error_code;
    }
}
