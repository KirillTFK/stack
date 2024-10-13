#pragma once
//-------------------------------------------Библиотеки---------------------------------------------------------
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <cassert>
#include <math.h>
//-------------------------------------------Константы---------------------------------------------------------
const size_t MIN_CAPACITY_DELTA = 20;
const size_t MAX_CAPACITY_DELTA = 100000;
const int MAX_DATA_SIZE = 200000000;
const unsigned long long MIN_CAPACITY = 0;
const int MIN_CAPACITY_TO_MULT = 100;
//-------------------------------------------ПРОТОТИПЫ ФУНКЦИЙ-------------------------------------------------
void decrease_capacity (struct Stack_t* const stk);
void increase_capacity (struct Stack_t* const stk);
void stack_push(struct Stack_t* const stk, const stack_elem_t new_elem);
stack_elem_t stack_pop (struct Stack_t* const stk);
void realloc_data (struct Stack_t* const stk);
void push_realloc (struct Stack_t* const stk);
void pop_realloc (struct Stack_t* const stk);
