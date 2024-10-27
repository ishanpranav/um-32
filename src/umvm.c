// main.c
// Copyright (c) 2024 Ishan Pranav
// Licensed under the MIT license.

// http://boundvariable.org

#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include "machine.h"

struct Machine um;

static uint8_t vm_read()
{
    int result = getchar();

    if (result == EOF)
    {
        return -1;
    }

    return result;
}

static void vm_write(uint8_t value)
{
    putchar(value);
}

int main(int count, char* args[])
{
    char* app = args[0];

    if (count < 2)
    {
        fprintf(stderr, "Usage: %s FILE\n", app);

        return EXIT_FAILURE;
    }

    if (!machine(&um, vm_read, vm_write))
    {
        fprintf(stderr, "%s: %s\n", app, strerror(errno));

        return EXIT_FAILURE;
    }

    char* path = args[1];
    FILE* input = fopen(path, "rb");

    if (!input)
    {
        finalize_machine(&um);
        fprintf(stderr, "%s: %s: %s\n", app, path, strerror(errno));

        return EXIT_FAILURE;
    }

    if (!machine_read_program(&um, input) || fclose(input) != 0)
    {
        finalize_machine(&um);        
        fprintf(stderr, "%s: %s\n", app, strerror(errno));

        return EXIT_FAILURE;
    }

    Fault fault;

    do
    {
        fault = machine_execute(&um);

        if (fault)
        {
            fprintf(stderr, "%s: %s\n", path, fault_to_string(fault));
        
            if (fault != FAULT_HALTED &&
                fault != FAULT_TERMINATED)
            {
                machine_dump(stderr, &um);
                finalize_machine(&um);
            
                return EXIT_FAILURE;
            }
        }
    }
    while (fault != FAULT_TERMINATED);

    fprintf(stdout, "%s: %s\n", path, fault_to_string(fault));
    machine_dump(stdout, &um);
    finalize_machine(&um);

    return EXIT_SUCCESS;
}
