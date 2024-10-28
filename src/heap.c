// heap.c
// Copyright (c) 2024 Ishan Pranav
// Licensed under the MIT license.

// http://boundvariable.org

#include <stdlib.h>
#include "heap.h"

bool heap(Heap instance)
{
    if (!segment(&instance->segment))
    {
        return false;
    }

    return true;
}

uint32_t heap_allocate(Heap instance, uint32_t capacity)
{
    return 0;
}

uint32_t* heap_index(Heap instance, uint32_t address, uint32_t offset)
{
    return NULL;
}

bool heap_free(Heap instance, uint32_t address)
{
    return true;
}

void finalize_heap(Heap instance)
{
    finalize_segment(&instance->segment);
}
