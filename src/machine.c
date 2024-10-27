// machine.c
// Copyright (c) 2024 Ishan Pranav
// Licensed under the MIT license.

// http://boundvariable.org

#include <inttypes.h>
#include "machine.h"
#include "opcode.h"
#define UM32_MACHINE_CHUNK_SIZE 256
#define UM32_MACHINE_MAX_SEGMENT_DUMP 16

bool machine(Machine instance, Reader reader, Writer writer)
{
    if (!segment(instance->segments))
    {
        return false;
    }

    instance->halted = false;

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
    uint32_t length;
    uint32_t chunk[UM32_MACHINE_CHUNK_SIZE];
    
    do
    {
        length = fread(chunk, sizeof * chunk, UM32_MACHINE_CHUNK_SIZE, input);
    
        for (uint32_t i = 0; i < length; i++)
        {
            chunk[i] = __builtin_bswap32(chunk[i]);
        }

        if (!segment_add_range(instance->segments, chunk, length))
        {
            return false;
        }
    }
    while (length == UM32_MACHINE_CHUNK_SIZE);

    return !ferror(input);
}

void machine_write_program_assembly(FILE* output, Machine instance)
{
    struct Segment segment = instance->segments[0];

    fprintf(output, "#include <stdint.h>\n" "#include <stdlib.h>\n\n");

    for (uint32_t i = 0; i < UM32_MACHINE_REGISTERS; i++)
    {
        fprintf(output, "uintptr_t r%d;\n", i);
    }

    fprintf(output, "\n");

    for (uint32_t i = 0; i < segment.count; i++)
    {
        uint32_t word = segment.buffer[i];
        Opcode opcode = word >> 28;
        int a = (word >> 6) & 0x7;
        int b = (word >> 3) & 0x7;
        int c = word & 0x7;

        fprintf(output, "%s %d %d %d\n", opcode_to_string(opcode), a, b, c);
    }
}

bool machine_execute(Machine instance)
{
    if (instance->instructionPointer >= instance->segments[0].count)
    {
        instance->halted = true;

        return true;
    }

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
    fprintf(output, "Registers:%17d word(s)\n", UM32_MACHINE_REGISTERS);
    machine_dump_many(output, instance->registers, UM32_MACHINE_REGISTERS);
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

        if (count > UM32_MACHINE_MAX_SEGMENT_DUMP)
        {
            count = UM32_MACHINE_MAX_SEGMENT_DUMP;
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
