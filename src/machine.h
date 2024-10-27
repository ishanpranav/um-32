// machine.h
// Copyright (c) 2024 Ishan Pranav
// Licensed under the MIT license.

// http://boundvariable.org

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reader.h"
#include "register.h"
#include "segment.h"
#include "writer.h"
#define UM32_MACHINE_HEAP_SEGMENTS 4
#define UM32_MACHINE_STACK_WORDS 4

struct Machine
{
    bool terminated;
    uint32_t registers[REGISTERS_COUNT];
    uint32_t stack[UM32_MACHINE_STACK_WORDS];
    struct Segment segments[UM32_MACHINE_HEAP_SEGMENTS];
    uint32_t stackPointer;
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
void finalize_machine(Machine instance);
