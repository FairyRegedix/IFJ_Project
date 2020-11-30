#include "expression.h"


unsigned int StackItems = 0;
unsigned int DisposedItems = 0;

eTypeTerm GetTerm(token_type Ttype) {
    switch (Ttype) {
        case TOKEN_ADD:
        case TOKEN_SUB:
            return T_SUBADD;

        case TOKEN_MUL:
        case TOKEN_DIV:
            return T_DIVMUL;

        case TOKEN_LT:
        case TOKEN_GT:
        case TOKEN_LTE:
        case TOKEN_GTE:
        case TOKEN_EQL:
        case TOKEN_NEQ:
            return T_LEG;

        case TOKEN_LBRACKET:
            return T_LB;

        case TOKEN_RBRACKET:
            return T_RB;

        case TOKEN_FLOAT:
        case TOKEN_ID:
        case TOKEN_INTEGER:
        case TOKEN_STR:
            return T_VAL;

        default:
            return T_DOLLAR;

    }

}


eTypeRel Relation(eTypeTerm current, eTypeTerm new) {
    switch (current) {
        case T_SUBADD:
            if (new == T_DIVMUL || new == T_LB || new == T_VAL)
                return T_open;
            return T_closed;
        case T_DIVMUL:
            if (new == T_LB || new == T_VAL)
                return T_open;
            return T_closed;
        case T_LEG:
            if (new == T_RB || new == T_LEG || new == T_DOLLAR)
                return T_closed;
            return T_open;
        case T_LB:
            if (new == T_RB) return T_equal;
            else if (new == T_DOLLAR) return T_nothing;
            return T_open;
        case T_RB:
        case T_VAL:
            if (new == T_LB || new == T_VAL)
                return T_nothing;
            return T_closed;

        case T_DOLLAR:
            if (new == T_RB || new == T_DOLLAR)
                return T_nothing;
            return T_open;

    }

    return T_nothing;
}


//                        STACK FUNCTIONS 

void init_e_stack(e_stack *stack) {

    //stack->p = malloc(sizeof(e_stack));
    for (int i = 0; i < MAX_STACK; i++) {
        stack->p[i] = NULL;
    }
    stack->top = -1;
}


e_stack_item pop_stack(e_stack *stack) {
    e_stack_item ret;
    ret = stack->p[stack->top];
    stack->p[stack->top] = NULL;
    stack->top--;
    StackItems = StackItems - 1;

    return ret;
}

void push_stack(e_stack *stack, e_stack_item tokenPushed, token_t *token_) {
    stack->top++;
    if (tokenPushed == NULL && token_ == NULL) {
        stack->p[stack->top] = tokenPushed;
    } else {
        tokenPushed = malloc(sizeof(struct expr_stack));
        if (tokenPushed != NULL) {
            //tokenPushed->next = stack->top;
            tokenPushed->token_stack = token_;
            tokenPushed->type = type_term;
            stack->p[stack->top] = tokenPushed;
            tokenPushed->dtype = token_->type;
            StackItems = StackItems + 1;

        }
    }
}

void push_nonterm(e_stack *stack, e_stack_item pushedNonterm) {
    stack->top++;
    StackItems = StackItems + 1;
    stack->p[stack->top] = pushedNonterm;

}


void push_openb(e_stack *stack, int position) {
    push_stack(stack, NULL, NULL);
    if (stack->top != 0) {
        for (int i = stack->top; i >= position; i--) {
            stack->p[i + 1] = stack->p[i];
        }
    }

    e_stack_item openb = malloc(sizeof(struct expr_stack));
    if (openb != NULL) {
        //openb->next = stack->top;
        openb->type = type_OPEN;
        stack->p[position] = openb;
        StackItems = StackItems + 1;
    }
}

e_stack_item FindNonTerminal(e_stack *stack) {
    e_stack_item item = NULL;

    for(int pos = 0; pos<MAX_STACK; pos++){
        item = stack->p[pos];
        if(item != NULL &&
        (item->type == type_non_term || item->type == type_non_term0)){
            break;
        }
    }
    return item;
}


int FindFirstTerminal(e_stack *stack) {
    int point = stack->top;
    while (point >= 0) {
        if (stack->p[point] != NULL && stack->p[point]->type == type_term)
            break;
        point = point - 1;
    }
    return point;

}

int FindFirstOpenB(e_stack *stack) {
    int point = stack->top;
    int ruleSize = 0;
    while (point >= 0) {
        if (stack->p[point] != NULL && stack->p[point]->type == type_OPEN)
            break;
        point--;
        ruleSize++;
    }
    return ruleSize;

}

bool e_stack_dispose(e_stack *stack) {
   for(int i = 0; i < MAX_STACK; i++) {
        free(stack->p[i]);
        DisposedItems = DisposedItems + 1;
    }
    return true;
}




//                        END OF STACK FUNCTIONS


int expressionParse(e_stack *stack, parser_info *p) {
    /*e_stack_item itemP;
    itemP = pop_stack(stack); */
    e_stack_item itemOP;
    token_type dataType;
    int ruleSize = FindFirstOpenB(stack);
    st_item *itemCheck;
    switch (ruleSize) {
        case 1:{

            //token_type dataType;
            itemOP = pop_stack(stack);
            // pravidlo E -> <val>
            switch (itemOP->dtype) {
                case TOKEN_INT:
                case TOKEN_INTEGER:
                    dataType = TOKEN_INT;
                    printf("PUSHS int@%s\n", itemOP->token_stack->actual_value.str);
                    break;

                case TOKEN_FLOAT:
                case TOKEN_FLOAT64:
                    dataType = TOKEN_FLOAT64;
                    printf("PUSHS float@%a\n", Str_to_Float(&itemOP->token_stack->actual_value));
                    break;

                case TOKEN_STR:
                case TOKEN_STRING:
                    dataType = TOKEN_STRING;
                    printf("PUSHS string@%s\n", itemOP->token_stack->actual_value.str);
                    break;
                case TOKEN_TRUE:
                    dataType = TOKEN_TRUE;
                    printf("PUSHS bool@true\n");
                    break;
                case TOKEN_FALSE:
                    dataType = TOKEN_FALSE;
                    printf("PUSHS bool@false\n");
                    break;
                case TOKEN_ID:
                    itemCheck = stack_lookup(p->local_st, &itemOP->token_stack->actual_value);
                    if (itemCheck == NULL) {
                        return 3;
                    }
                    dataType = itemCheck->data.as.variable.value_type;
                    if (dataType == TOKEN_STRING) {
                        printf("PUSHS LF@%s\n", itemOP->token_stack->actual_value.str);
                    } else if (dataType == TOKEN_INT) {
                        printf("PUSHS LF@%s\n", itemOP->token_stack->actual_value.str);
                    } else {
                        printf("PUSHS float@%a\n", Str_to_Float(&itemOP->token_stack->actual_value));
                    }
                    break;

                default:
                    //error
                    free(itemOP);
                    return 2;
            }
            itemOP->dtype = dataType;

            if (itemOP->token_stack->type == TOKEN_INTEGER) {
                if (Str_to_INT(&itemOP->token_stack->actual_value) == 0) {
                    itemOP->type = type_non_term0;
                } else {
                    itemOP->type = type_non_term;
                }

            } else if (itemOP->token_stack->type == TOKEN_FLOAT){
                if (Str_to_Float(&itemOP->token_stack->actual_value) == 0.0) {
                    itemOP->type = type_non_term0;
                } else {
                    itemOP->type = type_non_term;
                }
            } else{
                itemOP->type = type_non_term;
            }
            free(pop_stack(stack));
            push_nonterm(stack, itemOP);
            break;
        }
        case 3: {
            e_stack_item itemOP1;
            itemOP1 = pop_stack(stack);

            // VAL -> (VAL)
            if (itemOP1->token_stack->type == TOKEN_RBRACKET) {
                free(itemOP1);
                e_stack_item itemVAL = pop_stack(stack);
                e_stack_item itemBracket = pop_stack(stack);
                free(itemBracket);
                free(pop_stack(stack));
                push_nonterm(stack, itemVAL);
                return 0;

            }

            e_stack_item itemOP2;
            itemOP2 = pop_stack(stack);

            e_stack_item itemOP3;
            itemOP3 = pop_stack(stack);


            //int checking = str_cmp(&itemOP1->token_stack.actual_value,&itemOP3->token_stack.actual_value);

            if (itemOP1->dtype != itemOP3->dtype) {
                //error
                return 5;
            }
            switch (itemOP2->token_stack->type) {
                case TOKEN_ADD:
                    // E -> E + E
                    if (itemOP1->dtype == TOKEN_STRING) {
                        printf("DEFVAR GF@exp$string1");
                        printf("DEFVAS GF@exp$string2");
                        printf("POPS GF@exp$string2");
                        printf("POPS GF@exp$string1");
                        printf("CONCAT GF@EXPRESULT GF@exp$string1 GF@exp$string2");
                        printf("PUSH GF@EXPRESULT");

                    }
                    printf("ADDS\n");
                    break;
                case TOKEN_SUB:
                    //E -> E - E
                    if (itemOP1->dtype != TOKEN_STRING) {
                        printf("SUBS\n");
                        break;
                    }
                    return 5;
                case TOKEN_MUL:
                    //E -> E * E
                    if (itemOP1->dtype != TOKEN_STRING) {
                        printf("MULS\n");
                        break;
                    }
                    return 5;
                case TOKEN_DIV:
                    //E -> E / E
                    if (itemOP1->dtype != TOKEN_STRING) {
                        if (itemOP1->type == type_non_term0) {
                            return 9;
                        } else {
                            printf("DIVS\n");
                            break;
                        }

                    }
                    return 5;
                case TOKEN_LT:
                    //E -> E < E
                    printf("LTS\n");
                    break;
                case TOKEN_EQL:
                    // E -> E == E
                    printf("EQS\n");
                    break;
                case TOKEN_NEQ:
                    // E -> E != E
                    printf("EQS\nNOTS\n");
                    break;
                case TOKEN_GT:
                    // E -> E < E
                    printf("GTS\n");
                    break;
                case TOKEN_GTE:
                    //volanie megovej funkcie
                    break;
                case TOKEN_LTE:
                    //volanie megovej funkcie
                    break;
                default:

                    return 2;

            }

            free(itemOP1);
            free(itemOP2);
            free(pop_stack(stack));
            push_nonterm(stack,itemOP3);
            break;

        }
        default:
            //error nie je mozne uplatnit pravidlo
            return 2;
            break;

    }


    return 0;

}


int expression(parser_info *p) {
    e_stack stack;
    init_e_stack(&stack);
    int loop = 0;
    int check = 0;


    while (loop < 1) {
        eTypeTerm current, new;
        check = FindFirstTerminal(&stack);
        if (check < 1) {
            current = T_DOLLAR;
        } else {
            current = GetTerm((&stack)->p[check]->token_stack->type);
        }
        new = GetTerm(p->token->type);
        e_stack_item item = NULL;

        switch (Relation(current, new)) {
            case T_open :
                push_openb(&stack, ++check);

                push_stack(&stack, item, p->token);

                //volanie dalsieho tokenu , opytat sa fera ktora funkcia na to sluzi
                //p->token = *p->token->next;
                get_next_token(p);
                break;
            case T_closed : {
                int result = expressionParse(&stack, p);
                if (result > 0) {
                    //chyba vo vyraze
                    loop = result;
                }
               // push_stack(&stack, item, p->token);
            }
                break;


            case T_equal:
                push_stack(&stack, item, p->token);
                //volanie dalsieho tokenu
                //p->token = *p->token->next;
                get_next_token(p);
                break;

            case T_nothing :
                if (current == T_DOLLAR && new == T_DOLLAR) {
                    if (stack.top < 0) {
                        //error vyraz nemoze byt prazdny
                        loop = 2;
                    } else {
                        loop = 1;
                        printf("POPS GF@EXPRESULT\n");
                        //spravne ukoncenie analyzy vyrazu
                    }

                } else {
                    // SYNTAX error
                    loop = 2;
                }

                break;
        }


    }
    e_stack_item tmp = FindNonTerminal(&stack);
    if(tmp == NULL)
        goto after_add;

    str_add_char(&p->right_side_exp_types, tmp->dtype);

    after_add:
    e_stack_dispose(&stack);

    if (loop == 1)                                //uspesne ukoncenie precedencnej analyzy vyrazu
    {
        return SUCCESS;
    } else if (loop == 2)                          //chyba v rámci syntaktickej analýzy
    {
        return ERROR_SYN;
    } else if (loop == 3)                          //sémantická chyba v programe - nedefinovaná funkcia/premenná
    {
        return ERROR_SEM_DEF;
    } else if (loop == 5)                          //sémantická chyba typovej kompatibility v aritmetických reťazových a relačných výrazoch
    {
        return ERROR_SEM_COMP;
    } else if (loop == 9)                          //semanticka chyba delenia nulov
    {
        return ERROR_NULL;
    } else
        return ERROR_TRANS;

}


long int Str_to_INT(string *value) {
    int converted;
    converted = atol(value->str);
    return converted;
}

double Str_to_Float(string *value) {
    double converted;
    converted = atof(value->str);
    return converted;
}






