// opcode.h
// Copyright (c) 2024 Ishan Pranav
// Licensed under the MIT license.

// http://boundvariable.org

enum Opcode
{
    OPCODE_CONDITIONAL_MOVE = 0,
    OPCODE_GET = 1,
    OPCODE_SET = 2,
    OPCODE_ADD = 3,
    OPCODE_MULTIPLY = 4,
    OPCODE_DIVIDE = 5,
    OPCODE_NAND = 6,
    OPCODE_HALT = 7,
    OPCODE_ALLOCATE = 8,
    OPCODE_FREE = 9,
    OPCODE_WRITE = 10,
    OPCODE_READ = 11,
    OPCODE_LOAD = 12,
    OPCODE_IMMEDIATE = 13,
    OPCODES_COUNT
};

typedef enum Opcode Opcode;

const char* opcode_to_string(Opcode value);
