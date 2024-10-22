#include <stdio.h>
#include <cassert>
#include <limits>
#include <math.h>
#include <cassert>

#include "colors.h"
#include "stack.h"
#include "destructor.h"
#include "constructor.h"
#include "stack_push_and_pop.h"
#include "stack_verify.h"

//Привет Дима, надеюсь ты чувствуешь себя лучше.
//Прошу не искать ошибки, связанные с файловым выводом, он еще не работает.
//Внизу пример работающего кода
//Желаю тебе удачи!



FILE* f_ptr = stdout;

int main ()
{
    struct Stack_t* stk = NULL;

    stk = get_stack_pointer();

    STACK_CONSTRUCTOR (stk, 10, f_ptr);

    for (int i = 0; i < 20; i++)
    {
        stack_push (stk,121221.32321);

    }

    for (int i = 0; i < 2; i++)
    {
        stack_pop (stk);

    }

    //stack_dump (stk);

    stack_dtor (stk);
}

