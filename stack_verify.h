#pragma once

//-------------------------------------------Библиотеки---------------------------------------------------------
#include <stdio.h>

#include "stack.h"
#include "destructor.h"
//-------------------------------------------Макросы-----------------------------------------------------------
#define MYASSERT(condition,stk) if(!condition)\
                                    {\
                                        printf ("%sВ файле %s, в функции %s, в строке %d не выполнено условие: %s%s\n", RED, __FILE__, __func__, __LINE__, #condition, RESET);\
                                        if(stk != NULL)\
                                        free(stk);\
                                        exit(EXIT_FAILURE);\
                                    }
#define VERIFY_STACK(stk) verify_stack (stk, __FILE__, __func__, __LINE__)
#define STACK_ERROR(error, f_ptr) stack_error (error, f_ptr, file , func, line)
#define SUM_ERRORS(condition, ERROR) if(condition)\
                                        stk->stack_error = stk->stack_error | ERROR;
//-------------------------------------------Константы---------------------------------------------------------

const int SLIDER = 1;
const double CANARY_VALUE = 0xBAADF00D;
const char BORDER[] =     ".!.!.!.!.!.!.!.!.!.!.!.!.!.!.!.!.!.!.!.!.!.!.!.!.!.!.!.!.!.!.!.!.!.!.!.!."
                         ".!.!.!.!.!.!.!.!.!.!.!.!.!.!.!.!.!.!.!.!.!.!.!.!.!.!.!.!.!.!.!.!.!.!.!.!."
                         ".!.!.!.!.!.!.!.!.!.!.!.!.!.!.!.!.!.!.!.!.!.!.!.!.!.!.!.!.";
const char string_of_ZOV[] = "ZOVZOVZOVZOVZOVZOVZOVZOVZOVZOVZOVZOVZOVZOVZOVZOVZOVZOVZOVZOVZOVZOVZOVZOVZOVZOVZOVZOVZOV"
                             "ZOVZOVZOVZOVZOVZOVZOVZOVZOVZOVZOVZOVZOVZOVZOVZOVZOVZOVZOVZOVZOVZOVZOVZOVZOVZOVZOVZOVZOV"
                             "ZOVZOVZOVZOVZOVZOVZOVZOVZOV";
//-------------------------------------------Enum--------------------------------------------------------------
enum ERRORS
{
    RIGHT_STACK_CANARY_DIED = SLIDER,
    LEFT_STACK_CANARY_DIED = SLIDER << 1,
    RIGHT_DATA_CANARY_DIED = SLIDER << 2,
    LEFT_DATA_CANARY_DIED = SLIDER << 3,
    FAILED_TO_ALLOCATE_DYNAM_MEMORY = SLIDER << 4,
    MORE_THAN_MAX_DATA_SIZE = SLIDER << 5,
    SUSPICIOUS_SIZE = SLIDER << 6,
    ATTEMPT_TO_TAKE_ELEM_FROM_EMPTY_STACK = SLIDER << 7,
    POISON_EMPTY_STACK = SLIDER << 8,
    WRONG_STACK_HASH = SLIDER << 9,
    WRONG_DATA_HASH = SLIDER << 10
};

enum PRESENCE_OF_ERROR
{
    WITHOUT_ERROR = 0,
    WITH_ERROR = 1
};

//-------------------------------------------ПРОТОТИПЫ ФУНКЦИЙ-------------------------------------------------
void stack_dump (struct Stack_t *stk);
void check_canaries(struct Stack_t* const stk);
bool verify_stack (struct Stack_t * const stk,const char* const file, const char* const func, const int line);
bool stack_error (const int error, FILE* f_ptr, const char *file, const char *func, const int line);
void print_ZOV (FILE* f_ptr, int times);
void check_hash (Stack_t* stk);
void calculate_all_hash (Stack_t* stk);
