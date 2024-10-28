// heap.c
// Copyright (c) 2024 Ishan Pranav
// Licensed under the MIT license.

// http://boundvariable.org

#include "heap.h"

bool heap(Heap instance)
{
    if (!segment(&instance->segment))
    {
        return false;
    }

    return true;
}

void finalize_heap(Heap instance)
{
    finalize_segment(&instance->segment);
}
