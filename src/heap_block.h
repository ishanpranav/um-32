// heap_block.h
// Copyright (c) 2024 Ishan Pranav
// Licensed under the MIT license.

// http://boundvariable.org

#include <stdint.h>

struct HeapBlock
{
    bool allocated;
    uint32_t address;
    uint32_t capacity;
};

typedef struct HeapBlock* HeapBlock;
