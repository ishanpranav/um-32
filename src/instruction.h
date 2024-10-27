// instruction.h
// Copyright (c) 2024 Ishan Pranav
// Licensed under the MIT license.

// http://boundvariable.org

#include <stdint.h>
#include <stdio.h>
#define um32_instruction_opcode(word) ((word) >> 28)
#define um32_instruction_operand_a(word) (((word) >> 6) & 0x7)
#define um32_instruction_operand_b(word) (((word) >> 3) & 0x7)
#define um32_instruction_operand_c(word) ((word) & 0x7)
#define um32_instruction_immediate_register(word) (((word) >> 25) & 0x7)
#define um32_instruction_immediate_value(word) ((word) & 0x01ffffff)
#define um32_instruction(opcode, a, b, c) \
    (((opcode) << 28) | ((a) << 6) | ((b) << 3) | (c))
#define um32_instruction_from_immediate(opcode, a, immediate) \
    (((opcode) << 28) | ((a) << 25) | (immediate))

void instruction_write_assembly(FILE* output, uint32_t word);
