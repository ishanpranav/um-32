// fault.h
// Copyright (c) 2024 Ishan Pranav
// Licensed under the MIT license.

// http://boundvariable.org

#define um32_fault_is_stopped(fault) \
    ((fault) != FAULT_HALTED && (fault) != FAULT_TERMINATED)

enum Fault
{
    FAULT_NONE = 0,
    FAULT_TERMINATED = 1,
    FAULT_HALTED = 2,
    FAULT_DIVISION_BY_ZERO,
    FAULT_INVALID_ADDRESS,
    FAULT_INVALID_BYTE,
    FAULT_INVALID_FREE,
    FAULT_INVALID_INSTRUCTION,
    FAULT_INVALID_INSTRUCTION_POINTER,
    FAULT_MISSING_READER,
    FAULT_MISSING_WRITER,
    FAULT_NO_OPERATION,
    FAULT_OUT_OF_MEMORY,
    FAULTS_COUNT
};

typedef enum Fault Fault;

const char* fault_to_string(Fault value);
