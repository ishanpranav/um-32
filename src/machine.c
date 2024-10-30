// machine.c
// Copyright (c) 2024 Ishan Pranav
// Licensed under the MIT license.

// http://boundvariable.org

#include "instruction.h"
#include "machine.h"
#include "opcode.h"
#define UM32_MACHINE_CHUNK_SIZE 256

bool machine(Machine instance, Reader reader, Writer writer)
{
    if (!segment(&instance->program, 0))
    {
        return false;
    }

    if (!heap(&instance->heap))
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
        uint32_t capacity = instance->registers[c];
        uint32_t address = heap_allocate(&instance->heap, capacity);

        if (!address)
        {
            return FAULT_OUT_OF_MEMORY;
        }

        instance->registers[b] = address;
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
        if (!heap_free(&instance->heap, instance->registers[c]))
        {
            return FAULT_INVALID_FREE;
        }
        break;

    case OPCODE_GET:
    {
        uint32_t address = instance->registers[b];
        uint32_t offset = instance->registers[c];

        if (!address)
        {
            if (offset >= instance->program.length)
            {
                return FAULT_INVALID_ADDRESS;
            }

            instance->registers[a] = instance->program.buffer[offset];

            break;
        }

        uint32_t* index = heap_index(&instance->heap, address, offset, NULL);

        if (!index)
        {
            return FAULT_INVALID_ADDRESS;
        }

        instance->registers[a] = *index;
    }
    break;

    case OPCODE_HALT: return FAULT_HALTED;

    case OPCODE_LOAD:
    {
        uint32_t address = instance->registers[b];
        uint32_t offset = instance->registers[c];

        if (!address)
        {
            if (offset >= instance->program.length)
            {
                return FAULT_INVALID_INSTRUCTION_POINTER;
            }

            instance->instructionPointer = offset;

            return FAULT_NONE;
        }

        uint32_t length;
        uint32_t* index = heap_index(&instance->heap, address, 0, &length);

        if (!index)
        {
            return FAULT_INVALID_ADDRESS;
        }
    
        if (offset >= length)
        {
            return FAULT_INVALID_INSTRUCTION_POINTER;
        }
    
        segment_ensure_capacity(&instance->program, length);
        memcpy(instance->program.buffer, index, length * sizeof * index);

        instance->program.length = length;
        instance->instructionPointer = offset;
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
        uint32_t address = instance->registers[a];
        uint32_t offset = instance->registers[b];

        if (!address)
        {
            if (offset >= instance->program.length)
            {
                return FAULT_INVALID_ADDRESS;
            }

            instance->program.buffer[offset] = instance->registers[c];

            break;
        }

        uint32_t* index = heap_index(&instance->heap, address, offset, NULL);

        if (!index)
        {
            return FAULT_INVALID_ADDRESS;
        }

        *index = instance->registers[c];
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

void finalize_machine(Machine instance)
{
    finalize_segment(&instance->program);
    finalize_heap(&instance->heap);
}
