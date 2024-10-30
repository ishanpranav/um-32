// main.c
// Copyright (c) 2024 Ishan Pranav
// Licensed under the MIT license.

// http://boundvariable.org

#include <errno.h>
#include <inttypes.h>
#include <signal.h>
#include "instruction.h"
#include "machine.h"
#define UM32_VM_MAX_DUMP 16
#define UM32_VM_DEBUG

struct Machine um;

static uint8_t vm_read()
{
    int result = getchar();

    if (result == EOF)
    {
        return -1;
    }

    return result;
}

static void vm_write(uint8_t value)
{
    putchar(value);
}

static void vm_dump_raw(FILE* output, uint32_t values[], uint32_t length)
{
    if (length > UM32_VM_MAX_DUMP)
    {
        length = UM32_VM_MAX_DUMP;
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

void vm_dump_heap(FILE* output, Heap heap)
{
    struct HeapBlock it;
    uint32_t allocated = 0;
    uint32_t freed = 0;
    uint32_t allocatedSize = 0;
    uint32_t freedSize = 0;

    for (heap_first(&it); heap_next(&it, heap); )
    {
        if (it.allocated) 
        {
            allocated++;
            allocatedSize += it.capacity;
        }
        else 
        {
            freed++;
            freedSize += it.capacity;
        }
    }
    
    fprintf(output, 
        "Heap:%22" PRIu32 " word(s)\n"
        " %8" PRId32 " alloc'ed, %8" PRId32 " free'd\n"
        " %19.2lf%% fragmentation\n", 
        heap->segment.capacity,
        allocated, freed,
        (double)freedSize * 100.0 / (allocatedSize + freedSize));
    vm_dump_raw(output, heap->segment.buffer, heap->segment.length);
}

void vm_dump_machine(FILE* output, Machine machine)
{
    struct Segment program = machine->program;

    if (machine->instructionPointer < program.length)
    {
        uint32_t word = program.buffer[machine->instructionPointer];

        fprintf(output, "Instruction %08" PRIx32 ":\n",
            machine->instructionPointer);
        instruction_write_assembly(output, word);
        fprintf(output, "\n");
    }

    fprintf(output, "Registers:%17d word(s)\n", UM32_MACHINE_REGISTERS);
    vm_dump_raw(output, machine->registers, UM32_MACHINE_REGISTERS);
    fprintf(output, "Program:%18" PRIu32 " words(s)\n", program.length);
    vm_dump_raw(output, program.buffer, program.length);
    vm_dump_heap(output, &machine->heap);
}

static void vm_handle_interrupt()
{
    printf("\nProcess terminating with signal %d (SIGINT)\n", SIGINT);
    vm_dump_machine(stderr, &um);
    finalize_machine(&um);
    exit(128 + SIGINT);
}

int main(int count, char* args[])
{
    signal(SIGINT, vm_handle_interrupt);

    char* app = args[0];

    if (count < 2)
    {
        fprintf(stderr, "Usage: %s FILE\n", app);

        return EXIT_FAILURE;
    }

    if (!machine(&um, vm_read, vm_write))
    {
        fprintf(stderr, "%s: %s\n", app, strerror(errno));

        return EXIT_FAILURE;
    }

    char* path = args[1];
    FILE* input = fopen(path, "rb");

    if (!input)
    {
        finalize_machine(&um);
        fprintf(stderr, "%s: %s: %s\n", app, path, strerror(errno));

        return EXIT_FAILURE;
    }

    if (!machine_read_program(&um, input))
    {
        fclose(input);
        finalize_machine(&um);
        fprintf(stderr, "%s: %s\n", app, strerror(errno));

        return EXIT_FAILURE;
    }

    if (fclose(input) != 0)
    {
        finalize_machine(&um);
        fprintf(stderr, "%s: %s: %s\n", app, path, strerror(errno));

        return EXIT_FAILURE;
    }

    Fault fault;

    do
    {
        fault = machine_execute(&um);

        if (fault && um32_fault_is_stopped(fault))
        {
            fprintf(stderr, "%s: %s\n", path, fault_to_string(fault));
            vm_dump_machine(stderr, &um);
            finalize_machine(&um);

            return EXIT_FAILURE;
        }
    } 
    while (um32_fault_is_stopped(fault));

#ifdef UM32_VM_DEBUG
        printf("%s: %s\n", path, fault_to_string(fault));
        vm_dump_machine(stdout, &um);
#endif
    
    finalize_machine(&um);

    return EXIT_SUCCESS;
}
