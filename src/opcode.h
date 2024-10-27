// opcode.h
// Copyright (c) 2024 Ishan Pranav
// Licensed under the MIT license.

// http://boundvariable.org

#ifndef UM32_OPCODE
#define UM32_OPCODE

enum Opcode
{
    OPCODE_CONDITIONAL_MOVE = 0x0,
    OPCODE_GET = 0x1,
    OPCODE_SET = 0x2,
    OPCODE_ADD = 0x3,
    OPCODE_MULTIPLY = 0x4,
    OPCODE_DIVIDE = 0x5,
    OPCODE_NAND = 0x6,
    OPCODE_HALT = 0x7,
    OPCODE_ALLOCATE = 0x8,
    OPCODE_FREE = 0x9,
    OPCODE_WRITE = 0xa,
    OPCODE_READ = 0xb,
    OPCODE_LOAD = 0xc,
    OPCODE_IMMEDIATE = 0xd,
    OPCODES_COUNT
};

typedef enum Opcode Opcode;

const char* opcode_to_string(Opcode value);
Opcode opcode_from_string(const char* value);

#endif
