/**
 * IFJ Projekt 2020
 *
 * Implementation of expression parsing ( precedence analysis )
 *
 * @author <xbican03> Matúš Bičanovský
 * @author <xsabol03> František Sabol
 */

#include "error.h"
#include "parser.h"



#define MAX_STACK 1000

/**
*variables for categories of precedence table
*/
typedef enum {
    T_DIVMUL,             
    T_SUBADD,            
    T_LEG,               
    T_LB,                
    T_RB,                
    T_VAL,               
    T_DOLLAR,            
} eTypeTerm;

/**
*variables for result of function Relation 
*/
typedef enum{
    T_open,              // <
    T_closed,            // >
    T_equal,             // =
    T_nothing,           
} eTypeRel;

/**
*types of items on stack 
*/
typedef enum {
    type_OPEN,                                   //open bracket (<)
    type_term,                                   //term
    type_non_term,                               //nonterm
    type_non_term0,                              //nonterm == 0
}tType;

// STACK stuctures and functions


/**
*struct of item on the stack
*/
typedef struct expr_stack {
    token_t* token_stack;                     //type of token
    tType type;                               //typ of item 
    token_type dtype;                        // nonterm data type
} *e_stack_item;


/**
*stuct of stack
*/
typedef struct e_stack{
    int top;                            
    e_stack_item p[MAX_STACK];
} e_stack;



/**
 * function for inicialization of stack
 * 
 * @param stack pointer on stack 
 */
void init_e_stack(e_stack *stack);                               

/**
 * function pops first item on the stack and returns it
 * @param stack pointer on stack
 * @return e_stack_item , item on the stack
 */
e_stack_item pop_stack(e_stack *stack);                                  

/**
 * function pushs item on the top of the stack
 * @param stack pointer on stack
 * @param tokenPushed stuct of the pushed item
 * @param token pointer on stuct token , to get information about token
 */ 
void push_stack(e_stack *stack, e_stack_item tokenPushed, token_t *token);                

/**
 * function for disposing the stack after end of precedence analysis
 * @param stack pointer on stack
 * @return true after succesfull dispose of stack
 */ 
bool e_stack_dispose(e_stack *stack);                            

/**
 * function push on stack type of item type_OPEN 
 * @param stack pointer on stack
 * @param position position on stack where its supposed to be pushed, which is after first terminal from the top of stack
 */ 
void push_openb(e_stack *stack,int position);

/**
 * function finds first terminal from the top of the stack
 * @param stack pointer on stack
 * @return int point position of the first terminal on stack
 */ 
int FindFirstTerminal(e_stack *stack);

/**
 * function used to find first type_OPEN from the top, which is used when deciding which reducing rule to use in expressionParse
 * @param stack pointer on stack
 */ 
int FindFirstOpenB(e_stack *stack);

             

/**
 * function pushs nonterm on the stack after using one of rules to reduce the terms on stack
 * @param stack pointer on stack
 * @param pushedNonterm item of nonterm to be pushed
 */ 
void push_nonterm(e_stack* stack, e_stack_item pushedNonterm);


//               END OF STUCK FUNCTIONS AND STRUCTURES


/**
 * main function of the modul, called from parser 
 * function is declaring stack, getting tokens and assigning them to the right category, calling functions for their comparing
 * based on result of comparing its pushing tokens on stack and calling next token or calling expressionParse to apply reducing rule
 * @param p pointer on stuct parser_info which contains informations about tokens needed for precedence analysis
 * @return is either error code when some mistake occur or the successful end of precedence analysis
 */ 
int expression(parser_info *p);

/**
 * function to categorize each token based on precedence table
 * @param Ttype type of token
 * @return category of token given
 */ 
eTypeTerm GetTerm(token_type Ttype);

/**
 * function solving relation of 2 tokens based on precedence table
 * 1 of the token is first term on the stack and second one is the actual token 
 * @param current type of current token on the stack
 * @param new type of actual token
 */ 
eTypeRel Relation(eTypeTerm current, eTypeTerm new);

/**
 * function for applying reducing rules , to reduce terms on stack
 * @param stack pointer on stack
 * @param p pointer on stuct parser_info which contains informations about tokens needed for precedence analysis
 * @return int value based on success of the function or if some error occured
 */ 
int expressionParse(e_stack* stack,parser_info *p);




