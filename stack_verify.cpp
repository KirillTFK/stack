#include <stdio.h>
#include <stdlib.h>
#include <cassert>
#include <math.h>

#include "stack_verify.h"
#include "colors.h"
#include "stack.h"
#include "hash.h"



bool stack_error (const int error, FILE* f_ptr, const char *file, const char *func, const int line)
{
    bool error_flag = WITHOUT_ERROR;

    if (error == 0)
    {

        return error_flag;
    }

    fprintf (f_ptr,"В файле %s, в функции %s, в строке %d\n", file, func, line);

    if (error & RIGHT_STACK_CANARY_DIED)
    {
        fprintf (f_ptr,"%sИспорчена правая канарейка стека%s\n",MAGENTA, RESET);
        error_flag = WITH_ERROR;
    }

    if (error & LEFT_STACK_CANARY_DIED)
    {
        fprintf (f_ptr,"%sИспорчена левая канарейка стека%s\n",MAGENTA, RESET);
        error_flag = WITH_ERROR;
    }

    if (error & RIGHT_DATA_CANARY_DIED)
    {
        fprintf (f_ptr,"%sИспорчена правая канарейка массива элементов%s\n", MAGENTA, RESET);
        error_flag = WITH_ERROR;
    }

    if (error & LEFT_DATA_CANARY_DIED)
    {
        printf ("%sИспорчена левая канарейка массива элементов%s\n", MAGENTA, RESET);
        error_flag = WITH_ERROR;
    }

    if (error & FAILED_TO_ALLOCATE_DYNAM_MEMORY)
    {
        fprintf (f_ptr,"%sНе смог выделить динамическую память%s\n", MAGENTA, RESET);
        error_flag = WITH_ERROR;
    }

    if (error & MORE_THAN_MAX_DATA_SIZE)
    {
        fprintf (f_ptr,"%sПопытка превысить максимально допустимый размер стека%s\n", MAGENTA, RESET);
        error_flag = WITH_ERROR;
    }

    if (error & SUSPICIOUS_SIZE)
    {
        fprintf (f_ptr,"%sПопытка уменьшить емкость ниже минимальной"
                        "Недопустимое число элементов или недопустимая ёмкость массива%s\n", MAGENTA, RESET);
        error_flag = WITH_ERROR;
    }

    if (error & ATTEMPT_TO_TAKE_ELEM_FROM_EMPTY_STACK)
    {
        fprintf (f_ptr,"%sПопытка взять элемент из пустого стека%s\n", MAGENTA, RESET);
        error_flag = WITH_ERROR;
    }

    if (error & POISON_EMPTY_STACK)
    {
        fprintf (f_ptr,"%sПопытка отравить пустой стек%s\n", MAGENTA, RESET);
        error_flag = WITH_ERROR;
    }

    if (error & WRONG_STACK_HASH)
    {
        fprintf (f_ptr, "%sНесанкцианированное изменение хэша структуры стек%s\n", MAGENTA, RESET);
        error_flag = WITH_ERROR;
    }

    if (error & WRONG_DATA_HASH)
    {
        fprintf (f_ptr, "%sНесанкцианированное изменение хэша массива элементов%s\n", MAGENTA, RESET);
        error_flag = WITH_ERROR;
    }

    return error_flag;
}

bool verify_stack (struct Stack_t * const stk,const char* const file,const char* const func,const int line)
{
    bool error_flag = WITHOUT_ERROR;
    MYASSERT (stk , stk);

    check_canaries (stk);
    check_hash (stk);

    if ((error_flag = STACK_ERROR (stk->stack_error, stk->f_ptr)) == WITH_ERROR)
        stack_dump(stk);

    return error_flag;
}


void check_canaries(struct Stack_t* const stk)
{
    MYASSERT (stk,stk);
    MYASSERT (stk->data,stk);


    SUM_ERRORS (fabs(stk->right_canary - CANARY_VALUE) > DEVIATION, RIGHT_STACK_CANARY_DIED);

    SUM_ERRORS (fabs(stk->left_canary - CANARY_VALUE) > DEVIATION, LEFT_STACK_CANARY_DIED);

    SUM_ERRORS (fabs(*(stk->data -1) - CANARY_VALUE )> DEVIATION, LEFT_DATA_CANARY_DIED);

    SUM_ERRORS (fabs((stk->data[stk->capacity]) - CANARY_VALUE)> DEVIATION, RIGHT_DATA_CANARY_DIED);
}

void stack_dump (struct Stack_t *stk)
{
    MYASSERT (stk, stk);
    MYASSERT (stk->data, stk);




    print_ZOV (stk->f_ptr, 1);
    fprintf (stk->f_ptr,"%sСтек%s: %s%s%s\n%sСозданный в файле%s: %s%s%s\n%sСозданный на строчке%s: %s%d%s\n"
                        "%sАдрес stack_data%s: %s%p%s\n%sЗначение stack_capacity%s: %s%llu%s\n"
                        "%sЗначение stack_size%s: %s%u%s\n%sЭлементы стека%s:\n",STANDARD, RESET, DARK_RED, stk->name, RESET,
                                                                                 STANDARD, RESET, RED, stk->parent_file,
                                                                                 RESET, STANDARD, RESET, ORANGE,
                                                                                 stk->parent_string,  RESET, STANDARD, RESET,
                                                                                 GREEN, stk->data,  RESET, STANDARD, RESET,
                                                                                 BLUE, stk->capacity, RESET, STANDARD, RESET,
                                                                                 MAGENTA, stk->size, RESET, STANDARD, RESET );

    if (stk->stack_error !=  SUSPICIOUS_SIZE && stk->stack_error != MORE_THAN_MAX_DATA_SIZE && stk->size != 0)
    {
        int count = 0;

        for (unsigned i = 0; i < stk->size - 1; i++, count++ )
        {
            if (count%8 == 0)
                putc ('\n',stk->f_ptr);
            fprintf (stk->f_ptr,"%s%10.3lf%s ", BOLD_WHITE, (stk->data)[i], RESET);
        }

        if (count%8 == 0)
                putc ('\n',stk->f_ptr);
        fprintf (stk->f_ptr,"%s%10.3lf%s ", BOLD_DARK_BLUE, (stk->data)[(stk->size)-1], RESET); //Выделяю последний добавленный в стек элемент
        count++;

        for (unsigned i = stk->size; i < stk->capacity; count++, i++ )
        {
            if (count%8 == 0)
                putc ('\n',stk->f_ptr);
            fprintf (stk->f_ptr,"%s%10.3lf%s ", BOLD_DARK_RED, (stk->data)[i], RESET);
        }

        putc ('\n', stk->f_ptr);
        fprintf (stk->f_ptr, "%s%s%s\n", FLASHING_DARK_RED, BORDER, RESET);
        putc ('\n', stk->f_ptr);
        putc ('\n', stk->f_ptr);
    }

    else
        fprintf (stk->f_ptr,"В целях безопасности не напечатал элементы стека, так как проблема с числом элементов массива\n");
}



void print_ZOV (FILE* f_ptr,int times)
{

    for (int i = 0; i < 3*times; i++)
    {
        if ( i%3 == 0)
            fprintf (f_ptr, "%s%s%s\n", FLASHING_WHITE, string_of_ZOV, RESET);

        if (i%3 == 1)
            fprintf (f_ptr, "%s%s%s\n", FLASHING_DARK_BLUE, string_of_ZOV, RESET);

        if (i%3 == 2)
            fprintf (f_ptr, "%s%s%s\n", FLASHING_DARK_RED, string_of_ZOV, RESET);

    }
    putc ('\n', f_ptr);
}


void calculate_all_hash (Stack_t* stk)
{
    MYASSERT (stk, stk);
    MYASSERT (stk->data, stk);

    stk->stk_hash = calculate_hash (stk, sizeof(Stack_t)-sizeof(hash_t)-sizeof(stack_elem_t));
    *((unsigned long long*)(stk->data)-2) = calculate_hash (stk->data, sizeof(stack_elem_t)*stk->capacity);
}


void check_hash (Stack_t* stk)
{
    MYASSERT (stk,stk);
    MYASSERT (stk->data, stk);

    hash_t control_hash_stk = 0;
    control_hash_stk = calculate_hash (stk, sizeof(Stack_t)-sizeof(hash_t)-sizeof(stack_elem_t));

    printf ("control_hash_stk ==:%llu\n", control_hash_stk);
    printf ("real hash value: %llu\n", stk->stk_hash);

    hash_t control_hash_data = 0;
    control_hash_data = calculate_hash (stk->data, sizeof(stack_elem_t)*stk->capacity);

    printf ("control_hash_data ==:%llu\n", control_hash_data);
    printf ("real  hash value: %llu\n", *((unsigned long long*)(stk->data - 2)));

    SUM_ERRORS (stk->stk_hash != control_hash_stk, WRONG_STACK_HASH);

    SUM_ERRORS (fabs(*((unsigned long long*)((stk->data)-2)) - control_hash_data) > DEVIATION, WRONG_DATA_HASH);


}
