#include "error.h"
#include "scanner.h"
#include "parser.h"

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#define MAX_STACK 1000

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



//typedef enum {
//    T_INT = type_int,
//    T_FLOAT = type_float,
//    T_STRING = type_str,
//    T_ID = TOKEN_ID,
//    //T_NIL,
//    T_BOOL,
//    T_ELSE ,
//}NonTermDataType;

typedef enum {
    type_OPEN,                                   //otvorena zatvorka (<)
    type_term,                                   //terminal
    type_non_term,                               //neterminal
    type_non_term0,                              //neterminal 0
}tType;

// STACK stuctures and functions


// struktura tokenu na zasobniku
typedef struct expr_stack {
    token_t* token_stack;                     //typ tokenu
    tType type;                                 //typ / otvorena zatvorka (<) /  terminal    / neterminal
    token_type dtype;                         // neterminalovy typ
    struct  expr_stack *next;
} *e_stack_item;


//struktura zasobniku
typedef struct e_stack{
    int top;
    e_stack_item p[MAX_STACK];
} e_stack;



// inicializacia stacku
void init_e_stack(e_stack *stack);                               

// pop token z vrchu zasobniku
e_stack_item pop_stack(e_stack *stack);                                  

// pushne token na zasobnik
void push_stack(e_stack *stack, e_stack_item tokenPushed, token_t *token);                

//uvolnenie celeho zasobniku po ukonceni precedencnej analyzy vyrazu
bool e_stack_dispose(e_stack *stack);                            

//vlozenie znaku (<) , pouzivaneho ako zarazka pri upravovani vyrazu na zaklade precedencnej analyzy
void push_openb(e_stack *stack,int position);

//najde prvy terminal na stacku z vrchu
int FindFirstTerminal(e_stack *stack);

//najdenie itemu na stacku obsahujuceho ((<),zarazka) , funkcia vyuzivana pri uplatnovani pravidiel na zjednodusenie
int FindFirstOpenB(e_stack *stack);

             

//pushne na zasobnik neterminal , nahradanie za terminal ktory uz bol vygenerovany
void push_nonterm(e_stack* stack, e_stack_item pushedNonterm);


//               END OF STUCK FUNCTIONS AND STRUCTURES


//hlavna funkcia expression.c , volana parserom , spusta precedencnu analyzu vyrazu
int expression(parser_info *p);

//funkcia zistujuca skupinu precedencnej tabulky do ktorej dany token patri
eTypeTerm GetTerm(token_type Ttype);

//ziska vztah medzi aktualnym tokenom , a terminalom najvyssie na stacku
eTypeRel Relation(eTypeTerm current, eTypeTerm new);

//funkcia na uplatnovanie pravidiel podla precedencnej tabulky na zjednodusovanie vyrazu a generovanie kodu
int expressionParse(e_stack* stack,parser_info *p);  

long int Str_to_INT(string* value);

double Str_to_Float(string* value);




