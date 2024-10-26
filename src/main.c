// main.c
// Copyright (c) 2024 Ishan Pranav
// Licensed under the MIT license.

// http://boundvariable.org

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

int main()
{
    machine(&um, main_read, main_write);
    machine_dump(stdout, &um);
    finalize_machine(&um);

    return 0;
}
