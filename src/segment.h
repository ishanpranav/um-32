// machine.h
// Copyright (c) 2024 Ishan Pranav
// Licensed under the MIT license.

// http://boundvariable.org

#include "word.h"

struct Segment
{
    Word count;
    Word capacity;
    Word* buffer;
};

typedef struct Segment* Segment;

void segment(Segment instance);
void segment_ensure_capacity();
void finalize_segment(Segment instance);
