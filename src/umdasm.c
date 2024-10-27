// umdasm.c
// Copyright (c) 2024 Ishan Pranav
// Licensed under the MIT license.

// http://boundvariable.org

#include <errno.h>
#include "instruction.h"
#include "machine.h"
#include "opcode.h"

struct Machine um;

void dasm_write(FILE* output, Machine instance)
{
    for (uint32_t i = 0; i < instance->program.length; i++)
    {
        instruction_write_assembly(output, instance->program.buffer[i]);
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
