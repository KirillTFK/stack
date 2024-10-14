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
    MYASSERT (stk, stk);
    VERIFY_STACK (stk);

    poison (stk);

    if (!stk)
        free (stk);

    else
        return;

     if (!stk->data)
        free (stk->data -2);

    //fclose (stk->f_ptr);
}




