// segment.c
// Copyright (c) 2024 Ishan Pranav
// Licensed under the MIT license.

// http://boundvariable.org

#include <stdlib.h>
#include "segment.h"
#define UM_SEGMENT_DEFAULT 4

void segment(Segment instance)
{
    instance->count = 0;
    instance->capacity = UM_SEGMENT_DEFAULT;
    instance->buffer = malloc(UM_SEGMENT_DEFAULT * sizeof * instance->buffer);
}

void finalize_segment(Segment instance)
{
    instance->count = 0;

    free(instance->buffer);
}
