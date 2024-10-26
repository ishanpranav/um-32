// segment.c
// Copyright (c) 2024 Ishan Pranav
// Licensed under the MIT license.

// http://boundvariable.org

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "segment.h"
#define UM_SEGMENT_DEFAULT 4

bool segment(Segment instance)
{
    instance->count = 0;
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

bool segment_add_range(Segment instance, uint32_t values[], uint32_t count)
{
    if (!segment_ensure_capacity(instance, instance->count + count))
    {
        return false;
    }

    memcpy(instance->buffer + instance->count, values, count * sizeof * values);

    instance->count += count;

    return true;
}

void finalize_segment(Segment instance)
{
    instance->count = 0;

    free(instance->buffer);
}
