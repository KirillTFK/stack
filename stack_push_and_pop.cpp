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
    #ifndef NDEBUG

    MYASSERT (stk, stk);

    SUM_ERRORS (stk->capacity == MIN_CAPACITY || stk->capacity > MAX_DATA_SIZE, SUSPICIOUS_SIZE);

    calculate_all_hash (stk);
    if (VERIFY_STACK (stk) == WITHOUT_ERROR)
    {
        do_increase_capacity (&(stk->capacity));
    }

    #endif

    #ifdef NDEBUG

    do_increase_capacity (&(stk->capacity));

    #endif

}

void increase_capacity (struct Stack_t* const stk)
{
    #ifndef NDEBUG

    MYASSERT (stk, stk);

    if (VERIFY_STACK (stk) == WITHOUT_ERROR)
    {
        SUM_ERRORS (stk->capacity == MAX_DATA_SIZE, MORE_THAN_MAX_DATA_SIZE);

        do_increase_capacity (&(stk->capacity));
    }

    #endif

    #ifdef NDEBUG

    do_increase_capacity (&(stk->capacity));

    #endif
}

void stack_push(struct Stack_t* const stk, const stack_elem_t new_elem)
{

    #ifndef NDEBUG

    MYASSERT (stk,stk);
    check_hash (stk);
    SUM_ERRORS (stk->capacity < stk->size, SUSPICIOUS_SIZE);
    calculate_all_hash (stk);

    if (VERIFY_STACK (stk) == WITHOUT_ERROR)
    {
       do_stack_push (stk, new_elem);
       calculate_all_hash (stk);
    }

    #endif

    #ifdef NDEBUG

    do_stack_push (stk, new_elem);

    #endif

}

stack_elem_t stack_pop (struct Stack_t* const stk)
{
    #ifndef NDEBUG

    MYASSERT (stk, stk);

    check_hash (stk);
    SUM_ERRORS (stk->size == 0, ATTEMPT_TO_TAKE_ELEM_FROM_EMPTY_STACK);
    SUM_ERRORS (stk->capacity < stk->size, SUSPICIOUS_SIZE);
    calculate_all_hash (stk);

    if (VERIFY_STACK (stk) == WITHOUT_ERROR)
    {
        stack_elem_t last_elem = 0;

        do_stack_pop (stk, &last_elem);

        calculate_all_hash (stk);

        return last_elem;
    }

    #endif

    #ifdef NDEBUG

    stack_elem_t last_elem = 0;

    do_stack_pop (stk, &last_elem);

    return last_elem;

    #endif
}


void realloc_data (struct Stack_t* const stk)
{

    #ifndef NDEBUG

    MYASSERT (stk, stk);

    #endif
    stk->data = (stack_elem_t*) realloc (stk->data-2, (stk->capacity + 3)*sizeof(stack_elem_t)) + 2;

    #ifndef NDEBUG

    SUM_ERRORS (stk->data == 0, FAILED_TO_ALLOCATE_DYNAM_MEMORY);


    if (stack_error (stk->stack_error, stk->f_ptr, __FILE__, __func__, __LINE__) == WITHOUT_ERROR)
    {
        *(stk->data - 1) = CANARY_VALUE;
        stk->data[stk->capacity] = CANARY_VALUE;
        calculate_all_hash (stk);

        poison (stk);
    }
    calculate_all_hash (stk);

    #endif

    #ifdef NDEBUG

    *(stk->data - 1) = CANARY_VALUE;
    stk->data[stk->capacity] = CANARY_VALUE;
    poison (stk);

    #endif
}


void push_realloc (struct Stack_t* const stk)
{
    #ifndef NDEBUG

    MYASSERT (stk, stk);

    #endif

    if (stk->size != stk->capacity)
        return;

    increase_capacity (stk);
    realloc_data (stk);

    #ifndef NDEBUG

    VERIFY_STACK (stk);

    #endif
}

void pop_realloc (struct Stack_t* const stk)
{
    #ifndef NDEBUG

    MYASSERT (stk, stk);

    #endif

    unsigned long long old_capacity = stk->capacity;
    decrease_capacity (stk);

    if (stk->capacity != old_capacity)
        realloc_data (stk);
}


void do_decrease_capacity (unsigned long long* const capacity, const unsigned size)
{
    if (*capacity == MIN_CAPACITY)
    {
        return;
    }

    else if (*capacity < MIN_CAPACITY_DELTA)
    {
        *capacity = MIN_CAPACITY;
    }

    else if (*capacity < MIN_CAPACITY_TO_MULT && 3*MIN_CAPACITY_DELTA < 2*(*capacity - size))
    {
        *capacity -= MIN_CAPACITY_DELTA;
    }

    else if (*capacity < MAX_CAPACITY_DELTA && 2*(*capacity) < 3*(*capacity - size))
    {
        *capacity /= 2;
    }

    else if (10*MAX_CAPACITY_DELTA < 9*(*capacity - size))
    {
        *capacity -= MAX_CAPACITY_DELTA;
    }
}

void do_increase_capacity (unsigned long long* const capacity)
{
    if (*capacity < MIN_CAPACITY_TO_MULT && *capacity + MIN_CAPACITY_DELTA < MAX_DATA_SIZE)
    {
        *capacity += MIN_CAPACITY_DELTA;
    }

    else if (*capacity < MAX_CAPACITY_DELTA && 2*(*capacity) < MAX_DATA_SIZE)
    {
        *capacity *= 2;
    }

    else if (*capacity + MAX_CAPACITY_DELTA < MAX_DATA_SIZE )
    {
        *capacity += MAX_CAPACITY_DELTA;
    }

    else if (*capacity + MAX_CAPACITY_DELTA >= MAX_DATA_SIZE && *capacity != MAX_DATA_SIZE)
    {
        *capacity = MAX_DATA_SIZE;
    }
}


void do_stack_push (struct Stack_t* const stk, const stack_elem_t new_elem)
{
    push_realloc (stk);

    #ifndef NDEBUG

    MYASSERT (stk, stk);

    #endif

    if (stk->capacity != stk->size)
        stk->data[(stk->size)++] = new_elem;
}

void do_stack_pop (struct Stack_t* const stk, stack_elem_t* const last_elem)
{
    *last_elem = stk->data[--(stk->size)];

    stk->data[(stk->size)] = NAN;

    pop_realloc (stk);
}
