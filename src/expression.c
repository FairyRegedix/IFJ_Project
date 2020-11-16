#include "expression.h"
#include "scanner.h"

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
        
        case TOKEN_FLOAT64:
        case TOKEN_ID:
        case TOKEN_INT:
        case TOKEN_STRING:
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
    stack->top = NULL;
}


void pop_stack(e_stack *stack)
{
    if(stack->top != NULL)
    {
        e_stack_item tmp = stack->top;
        stack->top = stack->top->next;
        free(tmp);
        StackItems = StackItems - 1;
    }
}

void push_stack(e_stack *stack, int tokenPushed)
{
    e_stack_item *Pushed = malloc(sizeof(e_stack_item));
    if(Pushed != NULL)
    {
        Pushed->next = stack->top;
        Pushed->token_stack = tokenPushed;
        stack->top = Pushed;
        StackItems = StackItems + 1 ;
    }
}

bool e_stack_dispose(e_stack *stack)
{
    while(stack->top != NULL)
    {
        pop_stack(stack);
        DisposedItems = DisposedItems + 1 ;
    }
    return true;
}




//                        END OF STACK FUNCTIONS







int expression()
{

}

