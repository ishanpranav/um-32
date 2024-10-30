// segment.c
// Copyright (c) 2024 Ishan Pranav
// Licensed under the MIT license.

// http://boundvariable.org

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "segment.h"
#define UM_SEGMENT_DEFAULT 4

bool segment(Segment instance, uint32_t capacity)
{
    instance->length = 0;

    if (capacity < UM_SEGMENT_DEFAULT)
    {
        capacity = UM_SEGMENT_DEFAULT;
    }
    
    instance->capacity = UM_SEGMENT_DEFAULT;
    instance->buffer = malloc(UM_SEGMENT_DEFAULT * sizeof * instance->buffer);

    return instance->buffer;
}

bool segment_ensure_capacity(Segment instance, uint32_t capacity)
{
    if (instance->capacity >= capacity)
    {
        return true;
    }

    uint32_t newCapacity = instance->capacity * 2;

    if (capacity > newCapacity)
    {
        newCapacity = capacity;
    }

    uint32_t* buffer = realloc(instance->buffer, newCapacity * sizeof * buffer);

    if (!buffer)
    {
        return false;
    }

    instance->capacity = newCapacity;
    instance->buffer = buffer;

    return true;
}

bool segment_add(Segment instance, uint32_t value)
{
    if (!segment_ensure_capacity(instance, instance->length + 1))
    {
        return false;
    }

    instance->buffer[instance->length] = value;
    instance->length++;

    return true;
}

bool segment_add_range(Segment instance, uint32_t values[], uint32_t count)
{
    if (!segment_ensure_capacity(instance, instance->length + count))
    {
        return false;
    }

    memcpy(
        instance->buffer + instance->length, 
        values, 
        count * sizeof * values);

    instance->length += count;

    return true;
}

void finalize_segment(Segment instance)
{
    instance->length = 0;

    if (instance->buffer)
    {
        free(instance->buffer);

        instance->buffer = NULL;
    }
}
