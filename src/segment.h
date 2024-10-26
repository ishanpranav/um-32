// machine.h
// Copyright (c) 2024 Ishan Pranav
// Licensed under the MIT license.

// http://boundvariable.org

#include <stdint.h>

struct Segment
{
    uint32_t count;
    uint32_t capacity;
    uint32_t* buffer;
};

typedef struct Segment* Segment;

void segment(Segment instance);
void segment_ensure_capacity();
void finalize_segment(Segment instance);
