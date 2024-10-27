// machine.c
// Copyright (c) 2024 Ishan Pranav
// Licensed under the MIT license.

// http://boundvariable.org

#include <inttypes.h>
#include "machine.h"
#include "opcode.h"
#define UM32_MACHINE_CHUNK_SIZE 256
#define UM32_MACHINE_MAX_SEGMENT_DUMP 16
#define machine_isolate_bits(value, length, offset) \
    (((value) >> (offset)) & ((1u << (length)) - 1u))

bool machine(Machine instance, Reader reader, Writer writer)
{
    if (!segment(instance->segments))
    {
        return false;
    }

    instance->halted = false;

    memset(instance->registers, 0, sizeof instance->registers);

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
        uint32_t opcode = word >> 28;

        fprintf(output, "%08" PRIx32 ": %-5s ", word, opcode_to_string(opcode));

        if (opcode == OPCODE_IMMEDIATE)
        {
            uint32_t a = (word >> 25) & 0x7;
            uint32_t immediate = word & 0x01ffffff;

            fprintf(output, "%d $0x%" PRIx32 "\n", a, immediate);

            continue;
        }

        uint32_t a = (word >> 6) & 0x7;
        uint32_t b = (word >> 3) & 0x7;
        uint32_t c = word & 0x7;

        switch (opcode)
        {
        case OPCODE_ADD:
        case OPCODE_CONDITIONAL_MOVE:
        case OPCODE_DIVIDE:
        case OPCODE_GET:
        case OPCODE_MULTIPLY:
        case OPCODE_NAND:
        case OPCODE_SET:
            fprintf(output, "%d %d %d\n", a, b, c);
            break;

        case OPCODE_ALLOCATE:
        case OPCODE_LOAD:
            fprintf(output, "%d %d\n", b, c);
            break;

        case OPCODE_FREE:
        case OPCODE_READ:
        case OPCODE_WRITE:
            fprintf(output, "%d\n", c);
            break;

        default:
            fprintf(output, "\n");
        }
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
