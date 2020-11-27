#include "expression.h"


unsigned int StackItems = 0;
unsigned int DisposedItems = 0;

eTypeTerm GetTerm(token_type Ttype)
{
    switch(Ttype)
    {
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
        case TOKEN_INT:
        case TOKEN_STR:
            return T_VAL;
        
        default:
            return T_DOLLAR;

    }

}


eTypeRel Relation(eTypeTerm current, eTypeTerm new)
{
    switch(current)
    {
        case T_SUBADD:
            if(new == T_DIVMUL || new == T_LB || new == T_VAL)
                return T_open;
            return T_closed;
        case T_DIVMUL:
            if(new == T_LB || new == T_VAL)
                return T_open;
            return T_closed;
        case T_LEG:
            if(new == T_RB || new == T_LEG || new == T_DOLLAR) 
                return T_closed;
            return T_open;
        case T_LB:
            if(new == T_RB) return T_equal;
            else if(new == T_DOLLAR) return T_nothing;
            return T_open;
        case T_RB:
        case T_VAL:
            if(new == T_LB || new == T_VAL)
                return T_nothing;
            return T_closed;
        
        case T_DOLLAR:
            if(new == T_RB || new == T_DOLLAR)
                return T_nothing;
            return T_open;
        
    }

    return T_nothing;
}


//                        STACK FUNCTIONS 

void init_e_stack(e_stack *stack)
{
    
    //stack->p = malloc(sizeof(e_stack));
    stack->top = 0;
}


e_stack_item pop_stack(e_stack *stack)
{
    int i = stack->top;
    if(stack->top > 0)
    {
        stack->top--;
        StackItems = StackItems - 1;
    }
    return stack->p[i];
}

void push_stack(e_stack* stack, e_stack_item tokenPushed, token_t *token_)
{
    stack->top++;
    if (tokenPushed == NULL && token_ == NULL)
    {
        stack->p[stack->top] = tokenPushed;
    }
    else{
        tokenPushed = malloc(sizeof(struct expr_stack));
        if(tokenPushed != NULL)
        {
            //tokenPushed->next = stack->top;
            tokenPushed->token_stack.type = token_->type;
            tokenPushed->type = type_term;
            stack->p[stack->top] = tokenPushed;
            //tokenPushed->dtype = token_->actual_value;
            StackItems = StackItems + 1 ;
        }
    }
}

void push_nonterm(e_stack* stack, e_stack_item pushedNonterm)
{
    stack->top++;
    pushedNonterm = malloc(sizeof(struct expr_stack));
    StackItems = StackItems + 1;
}


void push_openb(e_stack* stack,int position)
{
    push_stack(stack,NULL,NULL);
    for(int i =stack->top; i >=position;position--)
    {
        stack->p[i+1] = stack->p[i];
    }


    e_stack_item openb = malloc(sizeof(struct expr_stack));
    if(openb != NULL)
    {
        //openb->next = stack->top;
        openb->type = type_OPEN;
        stack->p[position] = openb;
        StackItems = StackItems + 1;
    }
}

int FindFirstTerminal(e_stack* stack)
{
    int point = stack->top;
    while(point >= 0)
    {
        if(stack->p[point]->type == type_term)
            break;
        point = point - 1;
    }
    return point;

}

int FindFirstOpenB(e_stack* stack)
{
    int point = stack->top;
    int ruleSize = 0;
    while(point >= 0)
    {
        if(stack->p[point]->type == type_OPEN)
            break;
        point = point - 1;
        ruleSize++;

    }
    return ruleSize;

}

bool e_stack_dispose(e_stack *stack)
{
    while(stack->top > 0)
    {
        // str free
        free (stack->p[stack->top]);
        stack->top--;
        DisposedItems = DisposedItems + 1 ;
    }
    return true;
}




//                        END OF STACK FUNCTIONS


int expressionParse(e_stack* stack,parser_info *p){
    /*e_stack_item itemP;
    itemP = pop_stack(stack); */
    int ruleSize = FindFirstOpenB(stack);
    switch (ruleSize)
    {
    case 1:
    {
        e_stack_item itemOP;
        itemOP = pop_stack(stack);
        NonTermDataType dataType;
        // pravidlo E -> <val>
        switch (itemOP->type)
        {
        case TOKEN_INT:
            dataType = T_INT;
            printf("PUSHS int@%a\n",itemOP->token_stack.actual_value);
            break;
        case TOKEN_FLOAT:
            dataType = T_FLOAT;
            printf("PUSHS float@%a\n",itemOP->token_stack.actual_value);
            break;
        case TOKEN_STRING:
            dataType = T_STRING;
            printf("PUSHS string@%a\n",itemOP->token_stack.actual_value);
            break;
        case TOKEN_TRUE:
            dataType = T_BOOL;
            printf("PUSHS bool@true\n");
            break;
        case TOKEN_FALSE:
            dataType = T_BOOL;
            printf("PUSHS bool@false\n");
            break;
        case TOKEN_ID:
            dataType = T_ELSE;
            printf("PUSHS LF@%a\n",itemOP->token_stack.actual_value);
            break;
        default:
            //error
            free(itemOP);
            return 2;
            break;
        }
        break;
        itemOP->type = type_non_term;
        itemOP->dtype = dataType;
        free(pop_stack(stack));
        push_nonterm(stack,itemOP);
    }
    case 3:
    {
        e_stack_item itemOP1;
        itemOP1 = pop_stack(stack);
        
        // VAL -> (VAL)
        if(itemOP1->token_stack.type == TOKEN_RBRACKET)
        {
            free(itemOP1);
            e_stack_item itemVAL = pop_stack(stack);
            e_stack_item itemBracket = pop_stack(stack);
            free(itemBracket);
            free(pop_stack(stack));
            push_stack(stack,itemVAL,&itemVAL->token_stack);

        }
        
        e_stack_item itemOP2;
        itemOP2 = pop_stack(stack);
        e_stack_item itemOP3;
        itemOP3 = pop_stack(stack);
        //int checking = str_cmp(&itemOP1->token_stack.actual_value,&itemOP3->token_stack.actual_value);
        
        

        if(itemOP1->dtype != itemOP3->dtype)
        {
            //error
            return 2;
        }
        switch (itemOP2->token_stack.type)
        {
            case TOKEN_ADD:
                // E -> E + E
                printf("ADDS\n");
                break;
            case TOKEN_SUB:
                //E -> E - E
                printf("SUBS\n");
                break;
            case TOKEN_MUL:
                //E -> E * E
                printf("MULS\n");
                break;
            case TOKEN_DIV:
                //E -> E / E 
                printf("DIVS\n");
                break;
            case TOKEN_LT:
                //E -> E < E
                printf("LTS\n");
                break;
            case TOKEN_EQL:
                printf("EQS\n");
                break;
            case TOKEN_NEQ:
                printf("EQS\nNOTS\n");
                break;
            case TOKEN_GT:
                printf("GTS\n");
                break;
            case TOKEN_GTE:
                //volanie megovej funkcie
                break;
            case TOKEN_LTE:
                //volanie megovej funkcie
                break;
            default:

                break;
        }
        break;   
        
    }
    default:
        //error nie je mozne uplatnit pravidlo
        break;
    }
    return 0;

}




int expression(parser_info *p)
{   
    e_stack stack;
    init_e_stack(&stack);
    int loop = 0;

    token_t token_;
    token_ = *p->token;

    while(loop < 1)
    {
        eTypeTerm current , new ;
        int check = FindFirstTerminal(&stack);
        if(check < 1)
        {
            current = T_DOLLAR;
        }
        else
        {
            current = GetTerm((&stack)->p[check]->token_stack.type);
        }
        new = GetTerm(token_.type);
        e_stack_item item; 

        switch(Relation(current,new))
        {
            case T_open :
                push_openb(&stack,check+1);

                push_stack(&stack,item, &token_);

                //volanie dalsieho tokenu , opytat sa fera ktora funkcia na to sluzi
                //token_ = *p->token->next;
                get_next_token(p);
                break;
            case T_closed :
                {
                    int result = expressionParse(&stack,p);
                    if(result > 0)
                    {
                        //chyba vo vyraze
                        loop = result;
                    }
                }
                break;


            case T_equal:
                push_stack(&stack,item, &token_);
                //volanie dalsieho tokenu
                //token_ = *p->token->next;
                get_next_token(p);
                break;

            case T_nothing :
                if(current == T_DOLLAR && new == T_DOLLAR)
                {
                    if(stack.top <= 0)
                    {
                        //error vyraz nemoze byt prazdny
                        loop = 2;
                    }
                    else
                    {
                        loop = 1;
                        //spravne ukoncenie analyzy vyrazu
                    }
                    
                }
                else
                {
                    // SYNTAX error
                    loop = 2;
                }
                
                break;
        }

        loop=2;

    }
    e_stack_dispose(&stack);
    return loop;
}

int main()
{
    return 0;
}