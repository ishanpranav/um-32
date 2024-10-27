// opcode.c
// Copyright (c) 2024 Ishan Pranav
// Licensed under the MIT license.

// http://boundvariable.org

#include "opcode.h"

const char* OPCODES_STRINGS[OPCODES_COUNT] =
{
    [OPCODE_ADD] = "add",
    [OPCODE_ALLOCATE] = "alloc",
    [OPCODE_CONDITIONAL_MOVE] = "cmov",
    [OPCODE_DIVIDE] = "div",
    [OPCODE_FREE] = "free",
    [OPCODE_GET] = "getp",
    [OPCODE_HALT] = "halt",
    [OPCODE_LOAD] = "load",
    [OPCODE_IMMEDIATE] = "li",
    [OPCODE_MULTIPLY] = "mul",
    [OPCODE_NAND] = "nand",
    [OPCODE_READ] = "inb",
    [OPCODE_SET] = "setp",
    [OPCODE_WRITE] = "outb"
};

const char* opcode_to_string(Opcode value)
{
    if (value < 0 || value >= OPCODES_COUNT)
    {
        return "nop";
    }

    return OPCODES_STRINGS[value];
}
