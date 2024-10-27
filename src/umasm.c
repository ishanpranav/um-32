// umasm.c
// Copyright (c) 2024 Ishan Pranav
// Licensed under the MIT license.

// http://boundvariable.org

#include <ctype.h>
#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include "machine.h"
#include "opcode.h"
#define UM32_ASM_BUFFER_SIZE 256

struct Machine um;

size_t asm_read(FILE* input, Machine instance)
{
    size_t lineNumber = 0;
    char* instruction;
    char buffer[UM32_ASM_BUFFER_SIZE];

    while ((instruction = fgets(buffer, UM32_ASM_BUFFER_SIZE, input)))
    {
        lineNumber++;

        char* labelOffset = strchr(instruction, ':');

        if (labelOffset)
        {
            instruction = labelOffset + 1;
        }

        while (isspace(*instruction))
        {
            instruction++;
        }

        if (*instruction == '#')
        {
            continue;
        }

        char opcodeString[6];
        uint32_t a;
        uint32_t b;
        uint32_t c;
        int count = sscanf(instruction,
            "%5s %" SCNu32 " %" SCNu32 " %" SCNu32,
            opcodeString, &a, &b, &c);

        if (count < 1)
        {
            return lineNumber;
        }

        uint32_t opcode = opcode_from_string(opcodeString);

        switch (opcode)
        {
        case OPCODE_IMMEDIATE:
        {
            uint32_t immediate;

            count = sscanf(instruction, "%5s %" SCNx32 " $0x%" SCNx32,
                opcodeString, &a, &immediate);

            if (count != 3 || a > 7)
            {
                return lineNumber;
            }

            uint32_t word = um32_machine_immediate_word(opcode, a, immediate);

            segment_add(instance->segments, word);
        }
        break;

        case OPCODE_ADD:
        case OPCODE_CONDITIONAL_MOVE:
        case OPCODE_DIVIDE:
        case OPCODE_GET:
        case OPCODE_MULTIPLY:
        case OPCODE_NAND:
        case OPCODE_SET:
        {
            if (count != 4 || a > 7 || b > 7 || c > 7)
            {
                return lineNumber;
            }

            segment_add(instance->segments, um32_machine_word(opcode, a, b, c));
        }
        break;

        case OPCODE_ALLOCATE:
        case OPCODE_LOAD:
        {
            if (count != 3 || b > 7 || c > 7)
            {
                return lineNumber;
            }

            segment_add(instance->segments, um32_machine_word(opcode, 0, b, c));
        }
        break;

        case OPCODE_FREE:
        case OPCODE_READ:
        case OPCODE_WRITE:
        {
            if (count != 2 || c > 7)
            {
                return lineNumber;
            }

            segment_add(instance->segments, um32_machine_word(opcode, 0, 0, c));
        }
        break;

        case OPCODE_HALT:
        {
            if (count != 1)
            {
                return lineNumber;
            }

            segment_add(instance->segments, um32_machine_word(opcode, 0, 0, 0));
        }
        break;

        default: return lineNumber;
        }
    }

    return 0;
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

    size_t lineNumber = asm_read(stdin, &um);

    if (lineNumber != 0)
    {
        fprintf(stderr, "%s: syntax error on line %zu\n", app, lineNumber);

        return EXIT_FAILURE;
    }

    char* path = args[1];
    FILE* output = fopen(path, "wb");

    if (!output || !machine_write_program(&um, output) || fclose(output) != 0)
    {
        finalize_machine(&um);
        fprintf(stderr, "%s: %s: %s\n", app, path, strerror(errno));

        return EXIT_FAILURE;
    }

    finalize_machine(&um);

    return EXIT_SUCCESS;
}
