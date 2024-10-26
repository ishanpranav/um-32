// machine.c
// Copyright (c) 2024 Ishan Pranav
// Licensed under the MIT license.

// http://boundvariable.org

#include "machine.h"

void machine(Machine instance, Reader reader, Writer writer)
{
    memset(instance->stack, 0, sizeof instance->stack);
    memset(instance->registers, 0, sizeof instance->registers);
    segment(instance->segments);

    instance->reader = reader;
    instance->writer = writer;
}

void machine_load_program(Machine instance, FILE* input)
{
    
}
