#include <stdio.h>
#include <stdlib.h>
#include <cassert>
#include <math.h>
#include <string.h>

#include "colors.h"
#include "stack.h"
#include "stack_verify.h"
#include "stack_push_and_pop.h"
#include "constructor.h"
#include "hash.h"

struct Stack_t* get_stack_pointer (void)
{
    struct Stack_t *stk = (Stack_t*)calloc (1, sizeof(Stack_t));

    #ifndef NDEBUG

    MYASSERT (stk, stk);

    #endif

    return stk;

}

void stack_ctor (struct Stack_t* stk, unsigned long long capacity, FILE* f_ptr, const char* const func, const char* const file, const int line,
                 const char* const name)
{
    #ifndef NDEBUG

    SUM_ERRORS (capacity > MAX_DATA_SIZE, MORE_THAN_MAX_DATA_SIZE );
    MYASSERT (stk, stk);

    #endif

    stk->right_canary = CANARY_VALUE;
    stk->left_canary = CANARY_VALUE;
    stk->parent_func = func;
    stk->parent_file = file;
    stk->parent_string = line;
    stk->name = name;
    stk->f_ptr = f_ptr;

    //check_open (&(stk->f_ptr), "log.txt", "w");

    if (capacity < MIN_CAPACITY)
        stk->capacity = MIN_CAPACITY;
    else
        stk->capacity = capacity;

    stk->data = (stack_elem_t*)calloc (stk->capacity + 3, sizeof(stack_elem_t)) + 2;

    #ifndef NDEBUG

    SUM_ERRORS (stk->data == NULL, FAILED_TO_ALLOCATE_DYNAM_MEMORY);

    #endif

    #ifndef NDEBUG

    if (STACK_ERROR (stk->stack_error, stk->f_ptr) == WITHOUT_ERROR)
    {
        *(stk->data - 1) = CANARY_VALUE;
        stk->data[stk->capacity] = CANARY_VALUE;
        calculate_all_hash (stk);
    }

    #endif

    poison (stk);

}

void poison (Stack_t* stk)
{
    #ifndef NDEBUG

    MYASSERT (stk, stk);
    check_hash (stk);

    SUM_ERRORS (stk->capacity < stk->size, SUSPICIOUS_SIZE);
    SUM_ERRORS (!(stk->data), FAILED_TO_ALLOCATE_DYNAM_MEMORY);

    calculate_all_hash (stk);
    if (VERIFY_STACK(stk) == WITHOUT_ERROR)
    {
        do_poison (stk);
    }
    calculate_all_hash (stk);

    #endif

    #ifdef NDEBUG

    do_poison (stk);

    #endif

}


void check_open (FILE** f_ptr, const char *name, const char *mode)
{
    if ((*f_ptr = fopen(name, mode)) == NULL)
    {
        fprintf (stdout, "Не удалось открыть файл\n");

        exit (EXIT_FAILURE);
    }

    return;
}

void do_poison (struct Stack_t *stk)
{
    for (unsigned i = stk->size; i < stk->capacity; i++)
    {
        (stk->data)[i] = POISON_VALUE;
    }
}
