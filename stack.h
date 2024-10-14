#pragma once
#include <stdio.h>
#include "hash.h"
//------------------------------------------typedef-----------------------------------------------------------------------------
typedef double stack_elem_t;

//------------------------------------------Структуры---------------------------------------------------------------------------
struct Stack_t
{
    stack_elem_t left_canary;
    const char *name;
    const char *parent_file;
    const char *parent_func;
    int parent_string;
    unsigned long long capacity ;
    stack_elem_t *data;
    unsigned size;
    int stack_error;
    FILE* f_ptr;
    hash_t stk_hash;
    stack_elem_t right_canary;
};

//------------------------------------------Константы-----------------------------------------------------------------------------

const double DEVIATION = 1e-30;
