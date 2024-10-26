// machine.h
// Copyright (c) 2024 Ishan Pranav
// Licensed under the MIT license.

// http://boundvariable.org

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reader.h"
#include "segment.h"
#include "writer.h"
#define UM32_MACHINE_REGISTERS 8 
#define UM32_MACHINE_HEAP_SEGMENTS 4

struct Machine
{
    bool halted;
    uint32_t registers[UM32_MACHINE_REGISTERS];
    struct Segment segments[UM32_MACHINE_HEAP_SEGMENTS];
    uint32_t segmentCount;
    uint32_t instructionPointer;
    Reader reader;
    Writer writer;
};

typedef struct Machine* Machine;

bool machine(Machine instance, Reader reader, Writer writer);
bool machine_load_program(Machine instance, FILE* input);
bool machine_execute(Machine instance);
void machine_dump(FILE* output, Machine instance);
void machine_write_program_assembly(FILE* output, Machine instance);
void finalize_machine(Machine instance);
