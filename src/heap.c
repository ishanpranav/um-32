// heap.c
// Copyright (c) 2024 Ishan Pranav
// Licensed under the MIT license.

// http://boundvariable.org

// References:
//  - https://stackoverflow.com/questions/3479330/how-is-malloc-implemented-internally
//  - https://embeddedartistry.com/blog/2017/02/15/implementing-malloc-first-fit-free-list
//  - https://csit.kutztown.edu/~schwesin/fall20/csc235/lectures/Dynamic_Memory_Allocation_Basic.html
//  - https://web.stanford.edu/class/archive/cs/cs107/cs107.1246/lectures/24/Lecture24.pdf

#include <stdlib.h>
#include <string.h>
#include "heap.h"
#define UM32_HEAP_HEADER 2
#define UM32_HEAP_FOOTER 1
#define UM32_HEAP_OVERHEAD (UM32_HEAP_HEADER + UM32_HEAP_FOOTER)
#define um32_heap_capacity(instance, address) \
    ((instance)->segment.buffer + (address) - 2)
#define um32_heap_allocated(instance, address) \
    ((instance)->segment.buffer + (address) - 1)

bool heap(Heap instance)
{
    if (!segment(&instance->segment, UM32_HEAP_OVERHEAD))
    {
        return false;
    }

    return true;
}

// uint32_t heap_first_fit(Heap instance) 
// {
//     uint32_t address = UM32_HEAP_OVERHEAD;

//     while (address < instance->segment.length)
//     {
//         uint32_t capacity = instance->segment.buffer[address]
//     }

//     return address;
// }

void heap_first(HeapBlock result)
{
    result->address = UM32_HEAP_HEADER;
    result->allocated = false;
    result->capacity = 0;
}

bool heap_next(HeapBlock result, Heap instance)
{
    if (result->address < UM32_HEAP_HEADER)
    {
        return false;
    }

    result->capacity = instance->segment.buffer[result->address - 2];
    result->allocated = instance->segment.buffer[result->address - 1];
    result->address += result->capacity + UM32_HEAP_OVERHEAD;

    return result->address < instance->segment.length;
}

uint32_t heap_allocate(Heap instance, uint32_t capacity)
{
    Segment segment = &instance->segment;
    uint32_t address = segment->length + UM32_HEAP_HEADER;
    uint32_t length = address + capacity + UM32_HEAP_FOOTER;

    if (!segment_ensure_capacity(segment, length))
    {
        return 0;
    }

    *um32_heap_capacity(instance, address) = capacity;
    *um32_heap_allocated(instance, address) = true;

    memset(segment->buffer + address, 0, capacity * sizeof * segment->buffer);

    segment->buffer[address + capacity] = capacity;
    segment->length = length;

    return address;
}

uint32_t* heap_index(
    Heap instance,
    uint32_t address,
    uint32_t offset,
    uint32_t* length)
{
    if (address < UM32_HEAP_HEADER || !*um32_heap_allocated(instance, address))
    {
        return NULL;
    }

    uint32_t capacity = *um32_heap_capacity(instance, address);
    uint32_t footer = instance->segment.buffer[address + capacity];
    
    if (capacity != footer || offset >= capacity)
    {
        return NULL;
    }

    if (length)
    {
        *length = capacity;
    }

    return instance->segment.buffer + address + offset;
}

bool heap_free(Heap instance, uint32_t address)
{
    if (address < UM32_HEAP_HEADER || !*um32_heap_allocated(instance, address))
    {
        return false;
    }

    *um32_heap_allocated(instance, address) = false;

    return true;
}

void finalize_heap(Heap instance)
{
    finalize_segment(&instance->segment);
}
