// main.c
// Copyright (c) 2024 Ishan Pranav
// Licensed under the MIT license.

// http://boundvariable.org

#include <errno.h>
#include <stdio.h>
#include "machine.h"

struct Machine um;

static uint8_t main_read()
{
    return getchar();
}

static void main_write(uint8_t value)
{
    putchar(value);
}

static void main_print_usage(FILE* output, char* app)
{
    fprintf(output, "Usage: %s FILE\n", app);
}

int main(int count, char* args[])
{
    char* app = args[0];

    if (count < 2)
    {
        main_print_usage(stderr, app);

        return 1;
    }

    machine(&um, main_read, main_write);

    char* path = args[1];
    FILE* input = fopen(path, "r");

    if (!input)
    {
        fprintf(stderr, "%s: %s: %s\n", app, path, strerror(errno));

        return 1;
    }

    machine_load_program(&um, input);
    fclose(input);
    machine_dump(stdout, &um);
    finalize_machine(&um);

    return 0;
}
