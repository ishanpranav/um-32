// instruction.c
// Copyright (c) 2024 Ishan Pranav
// Licensed under the MIT license.

// http://boundvariable.org

#include <inttypes.h>
#include "instruction.h"
#include "opcode.h"

void instruction_write_assembly(FILE* output, uint32_t word)
{
    uint32_t opcode = um32_instruction_opcode(word);

    fprintf(output, "%08" PRIx32 ": %-5s ", word, opcode_to_string(opcode));

    if (opcode == OPCODE_IMMEDIATE)
    {
        uint32_t a = um32_instruction_immediate_register(word);
        uint32_t immediate = um32_instruction_immediate_value(word);

        fprintf(output, "%d $0x%" PRIx32 "\n", a, immediate);

        return;
    }

    uint32_t a = um32_instruction_operand_a(word);
    uint32_t b = um32_instruction_operand_b(word);
    uint32_t c = um32_instruction_operand_c(word);

    switch (opcode)
    {
    case OPCODE_ADD:
    case OPCODE_CONDITIONAL_MOVE:
    case OPCODE_DIVIDE:
    case OPCODE_GET:
    case OPCODE_MULTIPLY:
    case OPCODE_NAND:
    case OPCODE_SET:
        fprintf(output, "%" PRIu32 " %" PRIu32 " %" PRIu32 "\n", a, b, c);
        break;

    case OPCODE_ALLOCATE:
    case OPCODE_LOAD:
        fprintf(output, "%" PRIu32 " %" PRIu32 "\n", b, c);
        break;

    case OPCODE_FREE:
    case OPCODE_READ:
    case OPCODE_WRITE:
        fprintf(output, "%" PRIu32 "\n", c);
        break;

    default:
        fprintf(output, "\n");
        break;
    }
}
