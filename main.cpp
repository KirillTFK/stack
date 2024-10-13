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


FILE* f_ptr = stdout;

int main ()
{
    struct Stack_t* stk = NULL;

    stk = get_stack_pointer();

    STACK_CONSTRUCTOR (stk, 10, f_ptr);

    for (int i = 0; i < 13; i++)
    {
        stack_push (stk,121221.32321);

    }

    for (int i = 0; i < 1; i++)
    {
        stack_pop (stk);

    }

    stack_dump (stk);

    stack_dtor (stk);
}

