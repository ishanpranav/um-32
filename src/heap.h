// heap.h
// Copyright (c) 2024 Ishan Pranav
// Licensed under the MIT license.

// http://boundvariable.org

#include "segment.h"
#include "heap_block.h"

struct Heap
{
    struct Segment segment;
};

typedef struct Heap* Heap;

bool heap(Heap instance);
uint32_t heap_allocate(Heap instance, uint32_t capacity);

uint32_t* heap_index(
    Heap instance, 
    uint32_t address, 
    uint32_t offset, 
    uint32_t* length);

void heap_first(HeapBlock result);
bool heap_next(HeapBlock result, Heap instance);

bool heap_free(Heap instance, uint32_t address);
void finalize_heap(Heap instance);
