// machine.c
// Copyright (c) 2024 Ishan Pranav
// Licensed under the MIT license.

// http://boundvariable.org

#include <inttypes.h>
#include "machine.h"

void machine(Machine instance, Reader reader, Writer writer)
{
    memset(instance->registers, 0, sizeof instance->registers);
    memset(instance->stack, 0, sizeof instance->stack);
    segment(instance->segments);

    instance->stackPointer = 0;
    instance->segmentCount = 1;
    instance->reader = reader;
    instance->writer = writer;
}

static void machine_dump_many(FILE* output, uint32_t values[], uint32_t length)
{
    if (!length)
    {
        fprintf(output, "(empty)\n");

        return;
    }

    fprintf(output, "\n");

    while (length > 4)
    {
        fprintf(
            output,
            "%08" PRIx32 " %08" PRIx32 " %08" PRIx32 " %08" PRIx32 "\n",
            values[0], values[1], values[2], values[3]);

        values += 4;
        length -= 4;
    }

    for (uint32_t i = 0; i < length; i++)
    {
        fprintf(output, "%08" PRIx32 " ", values[i]);
    }

    fprintf(output, "\n\n");
}

void machine_dump(FILE* output, Machine instance)
{
    fprintf(output, "%-28s", "Registers:");
    machine_dump_many(output, instance->registers, REGISTERS_COUNT);
    fprintf(output, "%-28s", "Stack:");
    machine_dump_many(output, instance->stack, instance->stackPointer);

    for (uint32_t i = 0; i < instance->segmentCount; i++)
    {
        fprintf(output, "%-8s %08" PRIx32 ":          ", "Segment", i);

        struct Segment segment = instance->segments[i];

        machine_dump_many(output, segment.buffer, segment.count);
    }
}

void machine_load_program(Machine instance, FILE* input)
{

}

void finalize_machine(Machine instance)
{
    for (uint32_t i = 0; i < instance->segmentCount; i++)
    {
        finalize_segment(instance->segments + i);
    }
}
