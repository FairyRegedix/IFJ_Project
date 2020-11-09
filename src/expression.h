#include "error.h"
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

int expression();

eTypeTerm GetTerm();
eTypeRel Relation(eTypeTerm current, eTypeTerm new);