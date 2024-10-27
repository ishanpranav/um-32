// machine.h
// Copyright (c) 2024 Ishan Pranav
// Licensed under the MIT license.

// http://boundvariable.org

#include <stdbool.h>
#include <stdint.h>

struct Segment
{
    uint32_t count;
    uint32_t capacity;
    uint32_t* buffer;
};

typedef struct Segment* Segment;

bool segment(Segment instance);
bool segment_ensure_capacity(Segment instance, uint32_t capacity);
bool segment_add(Segment instance, uint32_t value);
bool segment_add_range(Segment instance, uint32_t values[], uint32_t count);
void finalize_segment(Segment instance);
