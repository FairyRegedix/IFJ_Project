/**
 * IFJ Projekt 2020
 *
 * Implementation of syntactic and semantic analysis
 *
 * @author <xsabol03> František Sabol
 */

#ifndef IFJ_PARSER_H
#define IFJ_PARSER_H

#include "symtable.h"
#include "scanner.h"

typedef struct parser_info{

    symbol_table_t st; //global symbol table  - only functions
    st_stack_t* local_st; //stack of symbol tables for currently proccessed function, the deepest scope at the top
    token_t* token; //active token
    token_list_t token_list; //list of tokens
    st_item* in_function; //pointer to the function which definition we are currently parsing
    int scope; //current scope number
    bool in_for; //

    string exp_instruction; //string of instructions for the last processed expression
    string for_exp; //string of instructions for an expression in the for header
    string left_side_vars_types; // v1,v2,...,v_n = ...
    string right_side_exp_types; // ... = exp1,exp2,...,exp_n | call params types
    st_item* function_called; // ... = f()
    token_t* last_param; //pointer to the token which is the last paramater passed to the function

}parser_info;

/*
 * Declarations of functions which represent non terminals of the grammar.
 * One function for one non terminal.
 *************************************************************************/
/* Rule:
 *      Prolog ::= EOL_opt package id EOL EOL_opt Prog
 *      */
int prolog(parser_info *p);

/* Rule:
 *     Prog ::= func Func EOL EOL_opt Prog |
 *              func Func eof |
 *              eof
 *     */
int prog(parser_info *p);

/* Rule:
 *     Func ::= id ( Params ) Ret_type { EOL_opt Statement_list }
 *     */
int func(parser_info *p);

/* Rule:
 *     EOL_opt ::= eol EOL_opt |
 *                 eps
 *     */
int EOL_opt(parser_info *p);

/* Rule:
 *     Params ::= id Type Params_n |
 *                eps
 *     */
int params(parser_info *p);

/* Rule:
 *     Params_n ::= , id Type Params_n |
 *                  eps
 *     */
int params_n(parser_info *p);

/* Rule:
 *     Ret_type ::= ( Type Ret_type_n ) |
 *                  eps
 *     */
int ret_type(parser_info *p);

/* Rule:
 *     Ret_type_n ::= , Type Ret_type_n |
 *                    eps
 *     */
int ret_type_n(parser_info *p);

/* Rule:
 *     Statement_list ::= Statement EOL EOL_opt Statement_list |
 *                        eps
 *     */
int statement_list(parser_info *p);

/* Rule:
 *     Statement ::= if EOL_opt exp { EOL EOL_opt Statement_list } else { EOL EOL_opt Statement_list } |
 *                   for  For_def ; exp ; For_assign { EOL EOL_opt Statement_list } |
 *                   return Return_exp |
*                    id Var
 *     */
int statement(parser_info *p);

/* Rule:
 *     Var ::= := exp |
 *             Func_call |
 *             Assign = EOL_opt End_assign
 *     */
int var(parser_info *p);

/* Rule:
 *     Assign ::= , EOL_opt id Assign |
 *                eps
 *     */
int assign(parser_info *p);

/* Rule:
 *     End_assign ::= id Func_call |
 *                    exp Exp_n
 *     */
int end_assign(parser_info *p);

/* Rule:
 *     Exp_n ::= , EOL_opt exp Exp_n |
 *               eps
 *     */
int exp_n(parser_info *p);

/* Rule:
 *     For_def ::= id := exp |
 *                 eps
 *     */
int for_def(parser_info *p);

/* Rule:
 *     For_assign ::= id Assign = EOL_opt End_assign |
 *                    eps
 *     */
int for_assign(parser_info *p);

/* Rule:
 *     Func_call ::= ( EOL_opt Call_params )
 *     */
int func_call(parser_info *p);

/* Rule:
 *     Call_params ::= Term Term_n |
 *                     eps
 *     */
int call_params(parser_info *p);

/* Rule:
 *     Term_n ::= , EOL_opt Term Term_n |
 *                eps
 *     */
int term_n(parser_info *p);

/* Rule:
 *     Return_exp ::= exp Exp_n |
 *                    eps
 *     */
int return_exp(parser_info *p);

/* Rule:
 *     Type ::= int |
 *              float64 |
 *              string
 *     */
bool is_data_type(token_type type);

/* Rule:
 *     Term ::= id |
 *              integer literal |
 *              float literal |
 *              string literal
 *     */
int set_data_type(parser_info *p, token_t *token, data_type *type, bool throw_error_on_default);
/*************************************************************************/


/*
 * Fills symbol table with builtin functions
 * @param st:   A pointer to a symbol table to fill.
 * */
int fill_st_with_builtin(symbol_table_t* st);

/*
 * Semantic check for mandatory function "main"
 * @param st:   A pointer to a symbol table.
 * */
int check_main(symbol_table_t* st);

/*
 * Semantic check for correct return types, assignment types, definition types, function call parameter types.
 * @param s1:   A pointer to a string where each character represents type.
 * @param s2:   A pointer to a string where each character represents type.
 * */
int check_types(string* s1, string* s2);

/*
 * Retrieves the next token from token list.
 * @param p:   A pointer to a struct parser_info.
 * */
int get_next_token(parser_info* p);

/*
 * Fills the global symbol table with function declarations.
 * @param p:   A pointer to a struct parser_info.
 * */
int first_pass(parser_info* p);

/*
 * Parser's main function.
 * */
int parser();

/*
 * Initializes parser info struct members.
 * @param p :   A pointer to a struct parser_info
 * */
int init_parser(parser_info* p);

/*
 * Cleanup function for initialized parser_info.
 * @param p :  A pointer to a struct parser_info
 * */
void dispose_parser(parser_info* p);

#endif //IFJ_PARSER_H
