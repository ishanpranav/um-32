// heap.c
// Copyright (c) 2024 Ishan Pranav
// Licensed under the MIT license.

// http://boundvariable.org

#include <stdlib.h>
#include <string.h>
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
    Segment segment = &instance->segment;
    uint32_t address = segment->length;

    if (!segment_ensure_capacity(segment, address + capacity + 1))
    {
        return 0;
    }
    
    segment->buffer[address] = capacity;
    address++;

    memset(segment->buffer + address, 0, capacity * sizeof * segment->buffer);

    segment->length += capacity + 1;

    return address;
}

uint32_t* heap_index(
    Heap instance, 
    uint32_t address, 
    uint32_t offset, 
    uint32_t* length)
{
    if (!address)
    {
        return NULL;
    }

    uint32_t capacity = instance->segment.buffer[address - 1];

    if (offset >= capacity)
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
    return true;
}

void finalize_heap(Heap instance)
{
    finalize_segment(&instance->segment);
}
