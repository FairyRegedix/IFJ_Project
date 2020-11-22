#include "error.h"
#include "scanner.h"
//#include "libraries.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

typedef enum {
    T_DIVMUL,            //spolocne ozancenie scitania a odcitania
    T_SUBADD,            //spolocne oznacenie nasobenia a delenia
    T_LEG,               // spolocne oznacenie porovnavania
    T_LB,                // lava zatvorka
    T_RB,                // prava zatvorka
    T_VAL,               // premenna
    T_DOLLAR,            // ine (zaciatok a koniec vyrazu)
} eTypeTerm;

typedef enum{
    T_open,              // otvaracia zatvorka (<)
    T_closed,            // zatvaracia zatvorka (>)
    T_equal,             // rovnost (=)
    T_nothing,           // neexistuje
} eTypeRel;

int expression(token *token_);

eTypeTerm GetTerm(token_type Ttype);
eTypeRel Relation(eTypeTerm current, eTypeTerm new);



typedef enum {
    T_INT,
    T_FLOAT,
    T_STRING,
    T_NIL,
    T_BOOL,
    T_ELSE,
}TermDataType;

typedef enum {
    type_OPEN,                                   //otvorena zatvorka (<)
    type_term,                                   //terminal
    type_non_term,                               //neterminal
}tType;

// STACK stuctures and functions


// struktura tokenu na zasobniku
typedef struct expr_stack {
    token_type token_stack;                     //typ tokenu
    tType type;                                 //typ / otvorena zatvorka (<) /  terminal    / neterminal
    TermDataType dtype;                             
    struct  expr_stack *next;
} e_stack_item;


//struktura zasobniku
typedef struct e_stack{
    int top;
    e_stack_item p;
} e_stack;

void init_e_stack(e_stack *stack);                               // inicializacia stacku

e_stack_item pop_stack(e_stack *stack);                                  // pop token z vrchu zasobniku

void push_stack(e_stack *stack, e_stack_item* tokenPushed, token *token);                // pushne token na zasobnik

bool e_stack_dispose(e_stack *stack);                            // uvolni cely zasobnik

void push_openb(e_stack *stack);

int FindFirstTerminal(e_stack *stack);

int FindFirstOpenB(e_stack *stack);

int expressionParse(e_stack *stack);

// e_stack_item Top_stack_item(e_stack *stack);                  // zistenie tokenu na vrcholu zasobniku





