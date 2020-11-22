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
        //int i = stack->top;
        stack->top--;
        //free(tmp);
        StackItems = StackItems - 1;
    }
    return stack->p[stack->top];
}

void push_stack(e_stack* stack, e_stack_item tokenPushed, token_t *token_)
{
    tokenPushed = malloc(sizeof(e_stack_item));
    if(tokenPushed != NULL)
    {
        //tokenPushed->next = stack->top;
        tokenPushed->token_stack.type = token_->type;
        tokenPushed->type = type_term;
        stack->p[stack->top] = tokenPushed;
        StackItems = StackItems + 1 ;
    }
}

void push_openb(e_stack* stack)
{
    e_stack_item openb = malloc(sizeof(e_stack_item));
    if(openb != NULL)
    {
        //openb->next = stack->top;
        openb->type = type_OPEN;
        stack->p[stack->top] = openb;
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


int expressionParse(e_stack* stack){
    /*e_stack_item itemP;
    itemP = pop_stack(stack); */
    int ruleSize = FindFirstOpenB(stack);
    switch (ruleSize)
    {
    case 1:
    {
        e_stack_item itemOP;
        itemOP = pop_stack(stack);
        break;
    }
    case 3:
    {
        e_stack_item itemOP1;
        itemOP1 = pop_stack(stack);
        e_stack_item itemOP2;
        itemOP2 = pop_stack(stack);
        e_stack_item itemOP3;
        itemOP3 = pop_stack(stack);
        break;
    }
    default:
        //error nie je mozne uplatnit pravidlo
        break;
    }

}




int expression(token_t *token)
{   
    e_stack stack;
    init_e_stack(&stack);
    int loop = 0;

    token_t token_;
    token_ = *token;

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
                push_openb(&stack);

                push_stack(&stack,item, &token_);

                //volanie dalsieho tokenu , opytat sa fera ktora funkcia na to sluzi

                break;
            case T_closed :
                {
                    int result = expressionParse(&stack);
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
                break;

            case T_nothing :
                if(current == T_DOLLAR && new == T_DOLLAR)
                {
                    if(stack.top <= 0)
                    {
                        //error vyraz nemoze byt prazdny
                        loop = 2;
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