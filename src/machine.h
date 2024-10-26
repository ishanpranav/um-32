// machine.h
// Copyright (c) 2024 Ishan Pranav
// Licensed under the MIT license.

// http://boundvariable.org

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "byte.h"
#include "reader.h"
#include "register.h"
#include "segment.h"
#include "word.h"
#include "writer.h"
#define UM32_MACHINE_HEAP_SIZE 4 * sizeof(struct Segment)
#define UM32_MACHINE_STACK_SIZE 4

struct Machine
{
    Word stack[UM32_MACHINE_STACK_SIZE];
    Word registers[REGISTERS_COUNT];
    struct Segment segments[UM32_MACHINE_HEAP_SIZE / sizeof(struct Segment)];
    Reader reader;
    Writer writer;
};

typedef struct Machine* Machine;

void machine(Machine instance, Reader reader, Writer writer);
void machine_load_program(Machine instance, FILE* input);
void finalize_machine(Machine instance);
