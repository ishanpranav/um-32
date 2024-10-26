// machine.h
// Copyright (c) 2024 Ishan Pranav
// Licensed under the MIT license.

// http://boundvariable.org

#include "byte.h"
#include "register.h"
#include "word.h"
#define MACHINE_STACK_SIZE 4

struct Machine
{
    Word stack[MACHINE_STACK_SIZE];
    Word registers[REGISTERS_COUNT];
    Word** segments;
    
    Byte (*read)();
    void (*write)(Byte value);
};
