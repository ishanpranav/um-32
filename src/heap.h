// heap.h
// Copyright (c) 2024 Ishan Pranav
// Licensed under the MIT license.

// http://boundvariable.org

#include "segment.h"

struct Heap
{
    struct Segment segment;
};

typedef struct Heap* Heap;

bool heap(Heap instance);
uint32_t heap_allocate(Heap instance);
bool heap_free(Heap instance, uint32_t address);
void finalize_heap(Heap instance);
