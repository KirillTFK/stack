#pragma once

//-------------------------------------------Библиотеки---------------------------------------------------------
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <cassert>
#include <math.h>
//-------------------------------------------Макросы-----------------------------------------------------------
#define STACK_CONSTRUCTOR(stk, size, ptr) stack_ctor(stk, size, ptr, __func__, __FILE__, __LINE__, #stk)
//-------------------------------------------Константы---------------------------------------------------------
const double POISON_VALUE = NAN;
//-------------------------------------------ПРОТОТИПЫ ФУНКЦИЙ-------------------------------------------------
void stack_ctor (struct Stack_t* stk, unsigned long long capacity, FILE* f_ptr, const char* const func, const char* const file, const int line,
                const char* const name);
struct Stack_t* get_stack_pointer (void);
void poison (Stack_t *stk);
void check_open (FILE** f_ptr, const char *name, const char *mode);
