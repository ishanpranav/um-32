// main.c
// Copyright (c) 2024 Ishan Pranav
// Licensed under the MIT license.

// http://boundvariable.org

#include <stdio.h>
#include "machine.h"

struct Machine um;

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
    machine(&um, main_read, main_write);
    printf("Hello, world!\n");

    return 0;
}
