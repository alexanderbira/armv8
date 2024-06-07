///
/// process.c
/// Midway point to get [IRExecutor]s and [BinaryParser]s.
///
/// Created by Billy Highley and Alexander Biraben-Renard on 29/05/2024.
///

#include "emulatorDelegate.h"

/// Get the corresponding [IRExecutor] for this [irObject].
/// @param ir The intermediate representation of the instruction
/// @returns The corresponding [IRExecutor].
Executor getExecuteFunction(IR *irObject) {
    switch (irObject->type) {
        case IMMEDIATE:
            return executeImmediate;

        case REGISTER:
            return executeRegister;

        case LOAD_STORE:
            return executeLoadStore;

        case BRANCH:
            return executeBranch;

        default:
            throwFatal("[getExecuteFunction] Invalid IR!");
    }
}

/// Get the corresponding [BinaryParser] for this [instruction].
/// @param ir The intermediate representation of the instruction
/// @returns The corresponding [BinaryParser].
Decoder getDecodeFunction(const Instruction instruction) {
    Component op0 = decompose(instruction, OP0_M);
    if ((op0 & OP0_IMMEDIATE_M) == OP0_IMMEDIATE_C) {
        return decodeImmediate;
    } else if ((op0 & OP0_REGISTER_M) == OP0_REGISTER_C) {
        return decodeRegister;
    } else if ((op0 & OP0_LOAD_STORE_M) == OP0_LOAD_STORE_C) {
        return decodeLoadStore;
    } else if ((op0 & OP0_BRANCH_M) == OP0_BRANCH_C) {
        return decodeBranch;
    }

    throwFatal("Invalid binary instruction!");
}
