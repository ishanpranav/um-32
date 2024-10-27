// machine.c
// Copyright (c) 2024 Ishan Pranav
// Licensed under the MIT license.

// http://boundvariable.org

#include <inttypes.h>
#include "instruction.h"
#include "machine.h"
#include "opcode.h"
#define UM32_MACHINE_CHUNK_SIZE 256
#define UM32_MACHINE_MAX_DUMP 16

bool machine(Machine instance, Reader reader, Writer writer)
{
    if (!segment(&instance->program))
    {
        return false;
    }

    if (!segment(&instance->heap))
    {
        finalize_segment(&instance->program);

        return false;
    }

    memset(instance->registers, 0, sizeof instance->registers);

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

        if (!segment_add_range(&instance->program, chunk, length))
        {
            return false;
        }
    } 
    while (length == UM32_MACHINE_CHUNK_SIZE);

    return !ferror(input);
}

bool machine_write_program(FILE* output, Machine instance)
{
    struct Segment program = instance->program;
    uint32_t chunk[UM32_MACHINE_CHUNK_SIZE];

    for (uint32_t i = 0; i < program.length; i += UM32_MACHINE_CHUNK_SIZE)
    {
        uint32_t length = UM32_MACHINE_CHUNK_SIZE;

        if (length > program.length - i)
        {
            length = program.length - i;
        }

        for (uint32_t i = 0; i < length; i++)
        {
            chunk[i] = __builtin_bswap32(program.buffer[i]);
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
    if (instance->instructionPointer >= instance->program.length)
    {
        return FAULT_TERMINATED;
    }

    uint32_t a;
    uint32_t word = instance->program.buffer[instance->instructionPointer];
    uint32_t opcode = um32_instruction_opcode(word);

    if (opcode > OPCODES_COUNT)
    {
        return FAULT_INVALID_INSTRUCTION;
    }

    if (opcode == OPCODE_IMMEDIATE)
    {
        a = um32_instruction_immediate_register(word);
        instance->registers[a] = um32_instruction_immediate_value(word);
        instance->instructionPointer++;

        return FAULT_NONE;
    }

    a = um32_instruction_operand_a(word);

    uint32_t b = um32_instruction_operand_b(word);
    uint32_t c = um32_instruction_operand_c(word);

    switch (opcode)
    {
    case OPCODE_ADD:
    {
        uint32_t sum = instance->registers[b] + instance->registers[c];

        instance->registers[a] = sum;
    }
    break;

    case OPCODE_ALLOCATE:
    {
        // uint32_t capacity = instance->registers[c];

        // if (instance->segmentCount >= UM32_MACHINE_HEAP_SEGMENTS)
        // {
        //     return FAULT_OUT_OF_MEMORY;
        // }

        // bool result = segment_from_capacity(
        //     instance->segments + instance->segmentCount,
        //     capacity);

        // if (!result)
        // {
        //     return FAULT_OUT_OF_MEMORY;
        // }

        // instance->registers[b] = instance->segmentCount;
        // instance->segmentCount++;

        return FAULT_OUT_OF_MEMORY;
    }
    break;

    case OPCODE_CONDITIONAL_MOVE:
        if (instance->registers[c])
        {
            instance->registers[a] = instance->registers[b];
        }
        break;

    case OPCODE_DIVIDE:
    {
        uint32_t denominator = instance->registers[c];

        if (!denominator)
        {
            return FAULT_DIVISION_BY_ZERO;
        }

        instance->registers[a] = instance->registers[b] / denominator;
    }
    break;

    case OPCODE_FREE:
    {
        // uint32_t segment = instance->registers[c];

        // if (segment >= instance->segmentCount)
        // {
        //     return FAULT_INVALID_SEGMENT;
        // }

        // finalize_segment(instance->segments + segment);
    }
    break;

    case OPCODE_GET:
    {
        // uint32_t segment = instance->registers[b];
        // uint32_t index = instance->registers[c];

        // if (segment >= instance->segmentCount)
        // {
        //     return FAULT_INVALID_SEGMENT;
        // }

        // if (index >= instance->segments[segment].count)
        // {
        //     return FAULT_INVALID_INDEX;
        // }

        // instance->registers[a] = instance->segments[segment].buffer[index];

        return FAULT_INVALID_INDEX;
    }
    break;

    case OPCODE_HALT: return FAULT_HALTED;

    case OPCODE_LOAD:
    {
        // uint32_t segment = instance->registers[b];
        // uint32_t index = instance->registers[c];

        // if (segment >= instance->segmentCount)
        // {
        //     return FAULT_INVALID_SEGMENT;
        // }

        // uint32_t count = instance->segments[segment].count;

        // if (index >= count)
        // {
        //     return FAULT_INVALID_INDEX;
        // }

        // if (segment)
        // {
        //     segment_ensure_capacity(instance->segments, count);
        //     memcpy(
        //         instance->segments[0].buffer,
        //         instance->segments[segment].buffer,
        //         count * sizeof * instance->segments[0].buffer);

        //     instance->segments[0].count = instance->segments[segment].count;
        // }

        // instance->instructionPointer = index;

        return FAULT_INVALID_INDEX;
    }
    return FAULT_NONE;

    case OPCODE_MULTIPLY:
    {
        uint32_t product = instance->registers[b] * instance->registers[c];

        instance->registers[a] = product;
    }
    break;

    case OPCODE_NAND:
    {
        uint32_t nand = ~(instance->registers[b] & instance->registers[c]);

        instance->registers[a] = nand;
    }
    break;

    case OPCODE_READ:
    {
        if (!instance->reader)
        {
            return FAULT_MISSING_READER;
        }

        instance->registers[c] = instance->reader();
    }
    break;

    case OPCODE_SET:
    {
        // uint32_t segment = instance->registers[a];
        // uint32_t index = instance->registers[b];

        // if (segment >= instance->segmentCount)
        // {
        //     return FAULT_INVALID_SEGMENT;
        // }

        // if (index >= instance->segments[segment].count)
        // {
        //     return FAULT_INVALID_INDEX;
        // }

        // instance->segments[segment].buffer[index] = instance->registers[c];

        return FAULT_INVALID_INDEX;
    }
    break;

    case OPCODE_WRITE:
    {
        if (instance->registers[c] > UINT8_MAX)
        {
            return FAULT_INVALID_BYTE;
        }

        if (!instance->writer)
        {
            return FAULT_MISSING_WRITER;
        }

        instance->writer(instance->registers[c]);
    }
    break;
    }

    instance->instructionPointer++;

    return FAULT_NONE;
}

static void machine_dump_many(FILE* output, uint32_t values[], uint32_t length)
{
    if (length > UM32_MACHINE_MAX_DUMP)
    {
        length = UM32_MACHINE_MAX_DUMP;
    }

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
    struct Segment program = instance->program;

    if (instance->instructionPointer < program.length)
    {
        uint32_t word = program.buffer[instance->instructionPointer];

        fprintf(output, "Instruction %08" PRIx32 ":\n",
            instance->instructionPointer);
        instruction_write_assembly(output, word);
        fprintf(output, "\n");
    }

    fprintf(output, "Registers:%17d word(s)\n", UM32_MACHINE_REGISTERS);
    machine_dump_many(output, instance->registers, UM32_MACHINE_REGISTERS);
    fprintf(output, "Program:%18d words(s)\n", program.length);
    machine_dump_many(output, program.buffer, program.length);

    // fprintf(output, "Heap:%20d object(s)\n\n", instance->segmentCount);

    // for (uint32_t i = 0; i < instance->segmentCount; i++)
    // {
    //     struct Segment segment = instance->segments[i];

    //     fprintf(
    //         output,
    //         "Segment %08" PRIx32 ":%10d word(s)\n", i, segment.count);

    //     uint64_t count = segment.count;

    //     if (count > UM32_MACHINE_MAX_SEGMENT_DUMP)
    //     {
    //         count = UM32_MACHINE_MAX_SEGMENT_DUMP;
    //     }

    //     machine_dump_many(output, segment.buffer, count);
    // }
}

void finalize_machine(Machine instance)
{
    finalize_segment(&instance->program);
    finalize_segment(&instance->heap);
}
