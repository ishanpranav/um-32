// fault.h
// Copyright (c) 2024 Ishan Pranav
// Licensed under the MIT license.

// http://boundvariable.org

enum Fault
{
    FAULT_NONE = 0,
    FAULT_HALTED = 1,
    FAULT_DIVISION_BY_ZERO,
    FAULT_INVALID_INSTRUCTION,
    FAULT_INVALID_SEGMENT,
    FAULT_INVALID_INDEX,
    FAULTS_COUNT
};

typedef enum Fault Fault;

const char* fault_to_string(Fault value);
