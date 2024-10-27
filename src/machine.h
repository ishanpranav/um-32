// machine.h
// Copyright (c) 2024 Ishan Pranav
// Licensed under the MIT license.

// http://boundvariable.org

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fault.h"
#include "reader.h"
#include "segment.h"
#include "writer.h"
#define UM32_MACHINE_REGISTERS 8 
#define UM32_MACHINE_HEAP_SEGMENTS 4
#define um32_machine_opcode(word) ((word) >> 28)
#define um32_machine_operand_a(word) (((word) >> 6) & 0x7)
#define um32_machine_operand_b(word) (((word) >> 3) & 0x7)
#define um32_machine_operand_c(word) ((word) & 0x7)
#define um32_machine_immediate_register(word) (((word) >> 25) & 0x7)
#define um32_machine_immediate_value(word) ((word) & 0x01ffffff)
#define um32_machine_word(opcode, a, b, c) \
    (((opcode) << 28) | ((a) << 6) | ((b) << 3) | (c))
#define um32_machine_immediate_word(opcode, a, immediate) \
    (((opcode) << 28) | ((a) << 25) | (immediate))

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
bool machine_read_program(Machine instance, FILE* output);
bool machine_write_program(Machine instance, FILE* input);
Fault machine_execute(Machine instance);
void machine_dump(FILE* output, Machine instance);
void finalize_machine(Machine instance);
