#include <stdio.h>

#include "stack.h"
#include "colors.h"

unsigned long long calculate_hash (void* arr, unsigned long long arr_size)
{

    hash_t hash = 5381;


    for (unsigned i = 0; i < arr_size; i++)
    {
        hash = ((hash << 5) + hash) + (hash_t)(*(((char*) arr + i)));

    }

    return hash;
}



