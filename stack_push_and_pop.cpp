#include <stdio.h>
#include <stdlib.h>
#include <cassert>
#include <math.h>

#include "colors.h"
#include "stack.h"
#include "stack_verify.h"
#include "stack_push_and_pop.h"
#include "constructor.h"

void decrease_capacity (struct Stack_t* const stk)
{
    MYASSERT (stk, stk);

    if (stk->capacity < MIN_CAPACITY_DELTA && stk->capacity != MIN_CAPACITY)
    {
        stk->capacity = MIN_CAPACITY;
        return;
    }

    check_hash (stk);
    SUM_ERRORS (stk->capacity == MIN_CAPACITY || stk->capacity > MAX_DATA_SIZE, SUSPICIOUS_SIZE);

    calculate_all_hash (stk);
    if (VERIFY_STACK (stk) == WITHOUT_ERROR)
    {
        if (stk->capacity < MIN_CAPACITY_TO_MULT)
        {
            stk->capacity -= MIN_CAPACITY_DELTA;
        }

        else if (stk->capacity < MAX_CAPACITY_DELTA)
        {
            stk->capacity /= 2;
        }

        else
        {
            stk->capacity -= MAX_CAPACITY_DELTA;
        }

        calculate_all_hash (stk);
    }

}

void increase_capacity (struct Stack_t* const stk)
{
    MYASSERT (stk, stk);

    if (VERIFY_STACK (stk) == WITHOUT_ERROR)
    {
        if (stk->capacity < MIN_CAPACITY_TO_MULT && stk->capacity + MIN_CAPACITY_DELTA < MAX_DATA_SIZE)
        {
            stk->capacity += MIN_CAPACITY_DELTA;
        }

        else if (stk->capacity < MAX_CAPACITY_DELTA && 2*stk->capacity < MAX_DATA_SIZE)
        {
            stk->capacity *= 2;
        }

        else if (stk->capacity + MAX_CAPACITY_DELTA < MAX_DATA_SIZE )
        {
            stk->capacity += MAX_CAPACITY_DELTA;
        }

        else if (stk->capacity + MAX_CAPACITY_DELTA >= MAX_DATA_SIZE && stk->capacity != MAX_DATA_SIZE)
        {
            stk->capacity = MAX_DATA_SIZE;
        }
        else
            SUM_ERRORS (stk->capacity == MAX_DATA_SIZE, MORE_THAN_MAX_DATA_SIZE);

        stack_dump (stk);
        printf ("Я в increase stack\n");
        calculate_all_hash (stk);
    }
}

void stack_push(struct Stack_t* const stk, const stack_elem_t new_elem)
{
    MYASSERT (stk, stk);

    check_hash (stk);
    SUM_ERRORS (stk->capacity < stk->size, SUSPICIOUS_SIZE);
    calculate_all_hash (stk);

    if (VERIFY_STACK (stk) == WITHOUT_ERROR)
    {
        push_realloc (stk);
        MYASSERT (stk, stk);
        if (stk->capacity != stk->size)
            stk->data[(stk->size)++] = new_elem;
        calculate_all_hash (stk);
    }

}

stack_elem_t stack_pop (struct Stack_t* const stk)
{
    MYASSERT (stk, stk);

    check_hash (stk);
    SUM_ERRORS (stk->size == 0, ATTEMPT_TO_TAKE_ELEM_FROM_EMPTY_STACK);
    SUM_ERRORS (stk->capacity < stk->size, SUSPICIOUS_SIZE);
    calculate_all_hash (stk);

    if (VERIFY_STACK (stk) == WITHOUT_ERROR)
    {
        stack_elem_t last_elem = 0;

        last_elem = stk->data[--(stk->size)];

        stk->data[(stk->size)] = NAN;

        pop_realloc (stk);
        calculate_all_hash (stk);

        return last_elem;
    }

}


void realloc_data (struct Stack_t* const stk)
{
    MYASSERT (stk, stk);

    stack_dump(stk);
    check_hash (stk);
    stk->data = (stack_elem_t*) realloc (stk->data-2, (stk->capacity + 3)*sizeof(stack_elem_t)) + 2;
    SUM_ERRORS (stk->data == 0, FAILED_TO_ALLOCATE_DYNAM_MEMORY);

    if (stack_error (stk->stack_error, stk->f_ptr, __FILE__, __func__, __LINE__) == WITHOUT_ERROR)
    {
        *(stk->data - 1) = CANARY_VALUE;
        stk->data[stk->capacity] = CANARY_VALUE;
        calculate_all_hash (stk);

        poison (stk);
    }
    calculate_all_hash (stk);
}


void push_realloc (struct Stack_t* const stk)
{
    MYASSERT (stk, stk);
    if (stk->size != stk->capacity)
        return;

    increase_capacity (stk);
    realloc_data (stk);
    VERIFY_STACK (stk);
}

void pop_realloc (struct Stack_t* const stk)
{
    MYASSERT (stk, stk);
    if (stk->capacity < MIN_CAPACITY_TO_MULT && 3*MIN_CAPACITY_DELTA < 2*(stk->capacity - stk->size) )
    {
        decrease_capacity (stk);
        realloc_data (stk);
    }

    else if (stk->capacity <= MAX_CAPACITY_DELTA && 2*stk->capacity < 3*(stk->capacity - stk->size))
    {
        decrease_capacity (stk);
        realloc_data (stk);
    }

    else if ( 10*MAX_CAPACITY_DELTA < 9*(stk->capacity - stk->size))
    {
        decrease_capacity (stk);
        realloc_data (stk);
    }
    VERIFY_STACK (stk);
}
