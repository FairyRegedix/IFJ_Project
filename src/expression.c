/**
 * IFJ Projekt 2020
 *
 * Implementation of expression parsing ( precedence analysis )
 *
 * @author <xbican03> Matúš Bičanovský
 * @author <xsabol03> František Sabol
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "expression.h"



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

        }
    }
}

void push_nonterm(e_stack *stack, e_stack_item pushedNonterm) {
    stack->top++;
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
    }
    return true;
}




//                        END OF STACK FUNCTIONS


int expressionParse(e_stack *stack, parser_info *p) {
    e_stack_item itemOP;
    token_type dataType;
    int ruleSize = FindFirstOpenB(stack);
    st_item *itemCheck;
    switch (ruleSize) {
        case 1:{
            char scope[50];
            itemOP = pop_stack(stack);
            // rule E -> <val>
            switch (itemOP->dtype) {

                case TOKEN_INT:
                case TOKEN_INTEGER:
                    dataType = TOKEN_INT;
                    str_concat(&p->exp_instruction,"PUSHS int@",strlen("PUSHS int@"));
                    str_concat(&p->exp_instruction,itemOP->token_stack->actual_value.str,itemOP->token_stack->actual_value.len);
                    str_add_char(&p->exp_instruction,'\n');
                    break;

                case TOKEN_FLOAT:
                case TOKEN_FLOAT64:
                    dataType = TOKEN_FLOAT64;
                    sprintf(scope,"%a",Str_to_Float(&itemOP->token_stack->actual_value));
                    str_concat(&p->exp_instruction,"PUSHS float@",strlen("PUSHS float@"));
                    str_concat(&p->exp_instruction,scope,strlen(scope));
                    str_add_char(&p->exp_instruction,'\n');
                    break;

                case TOKEN_STR:
                case TOKEN_STRING:
                    dataType = TOKEN_STRING;
                    str_concat(&p->exp_instruction,"PUSHS string@",strlen("PUSHS string@"));
                    str_concat(&p->exp_instruction,itemOP->token_stack->actual_value.str,itemOP->token_stack->actual_value.len);
                    str_add_char(&p->exp_instruction,'\n');
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
                    str_concat(&p->exp_instruction,"PUSHS LF@",strlen("PUSHS LF@"));
                    str_concat(&p->exp_instruction,itemOP->token_stack->actual_value.str,itemOP->token_stack->actual_value.len);
                    sprintf(scope,"$%d\n", itemCheck->data.scope);
                    str_concat(&p->exp_instruction, scope, strlen(scope));
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

            // E -> (E)
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
            if ((itemOP1->dtype != itemOP3->dtype) || itemOP1->dtype == TOKEN_BOOLEAN) {
                //error
                return 5;
            }
            switch (itemOP2->token_stack->type) {
                case TOKEN_ADD:
                    // E -> E + E
                    if (itemOP1->dtype == TOKEN_STRING) {
                        str_concat(&p->exp_instruction,"POPS GF@exp$string2",strlen("POPS GF@exp$string2"));
                        str_add_char(&p->exp_instruction,'\n');
                        str_concat(&p->exp_instruction,"POPS GF@exp$string1",strlen("POPS GF@exp$string1"));
                        str_add_char(&p->exp_instruction,'\n');
                        str_concat(&p->exp_instruction,"CONCAT GF@CONCATRESULT GF@exp$string1 GF@exp$string2",strlen("CONCAT GF@CONCATRESULT GF@exp$string1 GF@exp$string2"));
                        str_add_char(&p->exp_instruction,'\n');
                        str_concat(&p->exp_instruction,"PUSHS GF@CONCATRESULT",strlen("PUSHS GF@CONCATRESULT"));
                        str_add_char(&p->exp_instruction,'\n');
                    }
                    else{
                        str_concat(&p->exp_instruction,"ADDS",strlen("ADDS"));
                        str_add_char(&p->exp_instruction,'\n');
                    }
                    break;
                case TOKEN_SUB:
                    //E -> E - E
                    if (itemOP1->dtype != TOKEN_STRING) {
                        str_concat(&p->exp_instruction,"SUBS",strlen("SUBS"));
                        str_add_char(&p->exp_instruction,'\n');
                        break;
                    }
                    return 5;
                case TOKEN_MUL:
                    //E -> E * E
                    if (itemOP1->dtype != TOKEN_STRING) {
                        str_concat(&p->exp_instruction,"MULS",strlen("MULS"));
                        str_add_char(&p->exp_instruction,'\n');
                        break;
                    }
                    return 5;
                case TOKEN_DIV:
                    //E -> E / E
                    if (itemOP1->dtype != TOKEN_STRING) {
                        if (itemOP1->type == type_non_term0) {
                            return 9;
                        } else {
                            if(itemOP1->dtype == TOKEN_INT){
                                str_concat(&p->exp_instruction,"IDIVS",strlen("IDIVS"));
                                str_add_char(&p->exp_instruction,'\n');
                            }else{
                                str_concat(&p->exp_instruction,"DIVS",strlen("DIVS"));
                                str_add_char(&p->exp_instruction,'\n');
                            }
                            break;
                        }

                    }
                    return 5;
                case TOKEN_LT:
                    //E -> E < E
                    str_concat(&p->exp_instruction,"LTS",strlen("LTS"));
                    str_add_char(&p->exp_instruction,'\n');
                    itemOP3->dtype = TOKEN_BOOLEAN;
                    break;
                case TOKEN_EQL:
                    // E -> E == E
                    str_concat(&p->exp_instruction,"EQS",strlen("EQS"));
                    str_add_char(&p->exp_instruction,'\n');
                    itemOP3->dtype = TOKEN_BOOLEAN;
                    break;
                case TOKEN_NEQ:
                    // E -> E != E
                    str_concat(&p->exp_instruction,"EQS",strlen("EQS"));
                    str_add_char(&p->exp_instruction,'\n');
                    str_concat(&p->exp_instruction,"NOTS",strlen("NOTS"));
                    str_add_char(&p->exp_instruction,'\n');
                    itemOP3->dtype = TOKEN_BOOLEAN;
                    break;
                case TOKEN_GT:
                    // E -> E > E
                    str_concat(&p->exp_instruction,"GTS",strlen("GTS"));
                    str_add_char(&p->exp_instruction,'\n');
                    itemOP3->dtype = TOKEN_BOOLEAN;
                    break;
                case TOKEN_GTE:
                    // E -> E >= E
                    str_concat(&p->exp_instruction,"LTS",strlen("LTS"));
                    str_add_char(&p->exp_instruction,'\n');
                    str_concat(&p->exp_instruction,"NOTS",strlen("NOTS"));
                    str_add_char(&p->exp_instruction,'\n');
                    itemOP3->dtype = TOKEN_BOOLEAN;
                    break;
                case TOKEN_LTE:
                    // E - > E <= E
                    str_concat(&p->exp_instruction,"GTS",strlen("GTS"));
                    str_add_char(&p->exp_instruction,'\n');
                    str_concat(&p->exp_instruction,"NOTS",strlen("NOTS"));
                    str_add_char(&p->exp_instruction,'\n');
                    itemOP3->dtype = TOKEN_BOOLEAN;
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
            //error no rule to be used
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
    str_reinit(&p->exp_instruction);
    str_concat(&p->exp_instruction,"CLEARS",strlen("CLEARS"));
    str_add_char(&p->exp_instruction,'\n');


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
                get_next_token(p);
                break;
            case T_closed : {
                int result = expressionParse(&stack, p);
                if (result > 0) {
                    //error in expression
                    loop = result;
                }
            }
                break;


            case T_equal:
                push_stack(&stack, item, p->token);
                get_next_token(p);
                break;

            case T_nothing :
                if (current == T_DOLLAR && new == T_DOLLAR) {
                    if (stack.top < 0) {
                        //error , expression cannot be empty
                        loop = 2;
                    } else {
                        loop = 1;
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

    if (loop == 1)                                 //successful end of precedence analysis
    {
        return SUCCESS;
    } else if (loop == 2)                          //syntax error
    {
        return ERROR_SYN;
    } else if (loop == 3)                          //semantic error - undefined variable
    {
        return ERROR_SEM_DEF;
    } else if (loop == 5)                          //semantic error of different data type in expression
    {
        return ERROR_SEM_COMP;
    } else if (loop == 9)                          //semantic error - division by 0 constant
    {
        return ERROR_NULL;
    } else                                        //intern error of compiler
        return ERROR_TRANS;

}


