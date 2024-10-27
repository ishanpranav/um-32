// machine.c
// Copyright (c) 2024 Ishan Pranav
// Licensed under the MIT license.

// http://boundvariable.org

#include <inttypes.h>
#include "machine.h"
#define UM_MACHINE_CHUNK_SIZE 1024
#define UM_MACHINE_MAX_SEGMENT_DUMP 16

bool machine(Machine instance, Reader reader, Writer writer)
{
    if (!segment(instance->segments))
    {
        return false;
    }

    instance->terminated = false;

    memset(instance->registers, 0, sizeof instance->registers);
    memset(instance->stack, 0, sizeof instance->stack);
    
    instance->stackPointer = 0;
    instance->segmentCount = 1;
    instance->instructionPointer = 0;
    instance->reader = reader;
    instance->writer = writer;

    return true;
}

bool machine_load_program(Machine instance, FILE* input)
{
    uint32_t count;
    uint32_t chunk[UM_MACHINE_CHUNK_SIZE];

    do
    {
        count = fread(chunk, sizeof * chunk, UM_MACHINE_CHUNK_SIZE, input);

        if (!segment_add_range(instance->segments, chunk, count))
        {
            return false;
        }
    }
    while (count == UM_MACHINE_CHUNK_SIZE);

    return !ferror(input);
}

bool machine_execute(Machine instance)
{
    if (instance->instructionPointer >= instance->segments[0].count)
    {
        instance->terminated = true;

        return true;
    }

    instance->instructionPointer++;

    return true;
}

static void machine_dump_many(FILE* output, uint32_t values[], uint32_t length)
{
    while (length >= 4)
    {
        fprintf(
            output,
            "%08" PRIx32 " %08" PRIx32 " %08" PRIx32 " %08" PRIx32 "\n",
            values[0], values[1], values[2], values[3]);

        values += 4;
        length -= 4;
    }

    if (!length)
    {
        fprintf(output, "\n");

        return;
    }

    while (length)
    {
        fprintf(output, "%08" PRIx32 " ", *values);

        values++;
        length--;
    }

    fprintf(output, "\n\n");
}

void machine_dump(FILE* output, Machine instance)
{
    fprintf(output, "Registers:%17d word(s)\n", REGISTERS_COUNT);
    machine_dump_many(output, instance->registers, REGISTERS_COUNT);
    fprintf(output, "Stack:%21d word(s)\n", instance->stackPointer);
    machine_dump_many(output, instance->stack, instance->stackPointer);
    fprintf(output, "Heap:%19d segment(s)\n\n", instance->segmentCount);

    for (uint32_t i = 0; i < instance->segmentCount; i++)
    {
        struct Segment segment = instance->segments[i];

        fprintf(
            output,
            "Segment %08" PRIx32 ":%10d word(s)\n", i, segment.count);
        
        uint64_t count = segment.count;

        if (count > UM_MACHINE_MAX_SEGMENT_DUMP)
        {
            count = UM_MACHINE_MAX_SEGMENT_DUMP;
        }

        machine_dump_many(output, segment.buffer, count);
    }
}

void finalize_machine(Machine instance)
{
    for (uint32_t i = 0; i < instance->segmentCount; i++)
    {
        finalize_segment(instance->segments + i);
    }
}
