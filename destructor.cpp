#include <stdio.h>
#include <stdlib.h>
#include <cassert>
#include <math.h>

#include "colors.h"
#include "stack.h"
#include "stack_verify.h"
#include "constructor.h"
#include "destructor.h"


void stack_dtor (struct Stack_t* const stk)
{
    #ifndef NDEBUG

    MYASSERT (stk, stk);
    VERIFY_STACK (stk);

    poison (stk);

    #endif

    if (stk)
        free (stk);

    else
    {
        fprintf (stdout, "Указатель stk == 0 , не могу очистить данные по этому адресу\n");
        return;
    }

    if (stk->data)
        free (stk->data -2);

    else
        fprintf (stdout, "Указатель на массив data == 0, не могу очситить данные по этому адресу\n");

    //fclose (stk->f_ptr);
}




