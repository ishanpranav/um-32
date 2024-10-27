// fault.c
// Copyright (c) 2024 Ishan Pranav
// Licensed under the MIT license.

// http://boundvariable.org

#include "fault.h"

static const char* FAULTS_STRINGS[FAULTS_COUNT] =
{
    [FAULT_NONE] = "ok",
    [FAULT_TERMINATED] = "terminated",
    [FAULT_HALTED] = "stopped",
    [FAULT_DIVISION_BY_ZERO] = "division by zero",
    [FAULT_INVALID_BYTE] = "invalid byte",
    [FAULT_INVALID_INDEX] = "invalid index",
    [FAULT_INVALID_INSTRUCTION] = "invalid instruction",
    [FAULT_INVALID_SEGMENT] = "invalid segment",
    [FAULT_MISSING_READER] = "missing reader",
    [FAULT_MISSING_WRITER] = "missing writer",
    [FAULT_NO_OPERATION] = "no-op",
    [FAULT_OUT_OF_MEMORY] = "out of memory",
};

const char* fault_to_string(Fault value)
{
    if (value < 0 || value >= FAULTS_COUNT)
    {
        return "catastrophe";
    }

    return FAULTS_STRINGS[value];
}
