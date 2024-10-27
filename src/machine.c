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

    instance->segmentCount = 1;
    instance->instructionPointer = 0;
    instance->reader = reader;
    instance->writer = writer;

    return true;
}

bool machine_read_program(Machine instance, FILE* input)
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
    } while (length == UM32_MACHINE_CHUNK_SIZE);

    return !ferror(input);
}

bool machine_write_program(Machine instance, FILE* output)
{
    struct Segment segment = instance->segments[0];
    uint32_t chunk[UM32_MACHINE_CHUNK_SIZE];

    for (uint32_t i = 0; i < segment.count; i += UM32_MACHINE_CHUNK_SIZE)
    {
        uint32_t length = UM32_MACHINE_CHUNK_SIZE;

        if (length > segment.count - i)
        {
            length = segment.count - i;
        }

        for (uint32_t i = 0; i < length; i++)
        {
            chunk[i] = __builtin_bswap32(segment.buffer[i]);
        }

        if (fwrite(chunk, sizeof * chunk, length, output) != length)
        {
            return false;
        }
    }

    return true;
}

Fault machine_execute(Machine instance)
{
    if (instance->instructionPointer >= instance->segments[0].count)
    {
        instance->halted = true;

        return FAULT_NONE;
    }

    uint32_t a;
    uint32_t word = instance->segments[0].buffer[instance->instructionPointer];
    uint32_t opcode = um32_machine_opcode(word);

    if (opcode > OPCODES_COUNT)
    {
        return FAULT_INVALID_INSTRUCTION;
    }

    if (opcode == OPCODE_IMMEDIATE)
    {
        a = um32_machine_immediate_register(word);
        instance->registers[a] = um32_machine_immediate_value(word);
        instance->instructionPointer++;

        return FAULT_NONE;
    }

    a = um32_machine_operand_a(word);

    uint32_t b = um32_machine_operand_b(word);
    uint32_t c = um32_machine_operand_c(word);

    switch (opcode)
    {
    case OPCODE_CONDITIONAL_MOVE:
    {
        if (instance->registers[c])
        {
            instance->registers[a] = instance->registers[b];
        }
    }
    break;

    case OPCODE_GET:
    {
        uint32_t segment = instance->registers[b];
        uint32_t index = instance->registers[c];

        if (segment >= instance->segmentCount)
        {
            return FAULT_INVALID_SEGMENT;
        }

        if (index >= instance->segments[segment].count)
        {
            return FAULT_INVALID_INDEX;
        }

        instance->registers[a] = instance->segments[segment].buffer[index];
    }
    break;
    }

    instance->instructionPointer++;

    return FAULT_NONE;
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
