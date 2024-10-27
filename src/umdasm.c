// umdasm.c
// Copyright (c) 2024 Ishan Pranav
// Licensed under the MIT license.

// http://boundvariable.org

#include <inttypes.h>
#include <errno.h>
#include <stdio.h>
#include "machine.h"
#include "opcode.h"

struct Machine um;

void dasm_write(FILE* output, Machine instance)
{
    struct Segment segment = instance->segments[0];

    for (uint32_t i = 0; i < segment.count; i++)
    {
        uint32_t word = segment.buffer[i];
        uint32_t opcode = um32_machine_opcode(word);

        fprintf(output, "%08" PRIx32 ": %-5s ", word, opcode_to_string(opcode));

        if (opcode == OPCODE_IMMEDIATE)
        {
            uint32_t a = um32_machine_immediate_register(word);
            uint32_t immediate = um32_machine_immediate_value(word);

            fprintf(output, "%d $0x%" PRIx32 "\n", a, immediate);

            continue;
        }

        uint32_t a = um32_machine_operand_a(word);
        uint32_t b = um32_machine_operand_b(word);
        uint32_t c = um32_machine_operand_c(word);

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
}

int main(int count, char* args[])
{
    char* app = args[0];

    if (count < 2)
    {
        fprintf(stderr, "Usage: %s FILE\n", app);

        return EXIT_FAILURE;
    }

    if (!machine(&um, NULL, NULL))
    {
        perror(app);

        return EXIT_FAILURE;
    }

    char* path = args[1];
    FILE* input = fopen(path, "rb");

    if (!input ||!machine_read_program(&um, input) || fclose(input) != 0)
    {
        finalize_machine(&um);
        fprintf(stderr, "%s: %s: %s\n", app, path, strerror(errno));

        return EXIT_FAILURE;
    }

    dasm_write(stdout, &um);
    finalize_machine(&um);

    return EXIT_SUCCESS;
}
