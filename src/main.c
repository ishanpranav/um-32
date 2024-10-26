// main.c
// Copyright (c) 2024 Ishan Pranav
// Licensed under the MIT license.

// http://boundvariable.org

#include <stdio.h>
#include "machine.h"

struct Machine machine;

static Byte main_read()
{
    return getchar();
}

static void main_write(Byte value)
{
    putchar(value);
}

int main()
{
    machine.read = main_read;
    machine.write = main_write;

    printf("Hello, world!\n");

    return 0;
}
