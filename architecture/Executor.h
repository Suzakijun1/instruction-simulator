#ifndef HW2_INSTRUCTION_H
#define HW2_INSTRUCTION_H

#include "../nlohmann/json.hpp"
#include "Registers.h"
#include "Stack.h"
#include "Parser.h"
#include "Architecture.h"
#include <vector>
#include <map>
#include <stack>
#include <string>

// set a struct to capture state
struct executorState{
    // all original instructions
    std::vector<std::string> all_original_instructions;
    // all assembly inputs
    std::vector<assemblyPrep> all_assembly_instructions;
    // all binary inputs
    std::vector<binaryPrep> all_binary_instructions;
    // all raw binary outputs
    std::vector<std::string> all_raw_binary_instructions;
    // current assembly input
    assemblyPrep current_assembly_instruction;
    // current binary input
    binaryPrep current_binary_instruction;
    // current raw binary output
    std::string current_raw_binary_instruction;
    // current program counter
    int programCounter;
    // register values
    int R0;
    int R1;
    int R2;
    int R3;
    int R4;
    int R5;
    int R6;
    int R7;
    int R8;
    int R9;
    int R10;
    int R11;
    int R12;
    int R13;
    int R14;
    int R15;
    // current program status
    bool programStatus = false;
};


class Executor {
private:
    // declare Architecture Wrapper
    ArchitectureWrapper simpleRISC;
    // current raw binary Instruction
    std::string curRawBinaryInstruction;
    // binary Wrapper
    binaryPrep curBinaryInstruction;
    // assembly Wrapper
    assemblyPrep curAssemblyInstruction;
    // all raw binary Instructions
    std::vector<std::string> allRawBinaryInstructions;
    // all wrapper binary instructions
    std::vector<binaryPrep> allBinaryInstructions;
    // all wrapper assembly instructions
    std::vector<assemblyPrep> allAssemblyInstructions;
    // all original instructions
    std::vector<std::string> allOriginalInstructions;
    // program counter
    int programCounter;
    // program completion
    bool programComplete=false;
    // save states of each run
    std::vector<executorState> executorOutputs;

public:
    // Constructor to set the Architecture
    Executor(std::string arch_name);

    // Constructor to set the Architecture and current Assembly Instruction
    Executor(std::string arch_name, assemblyPrep current_assembly_instruction);

    // Constructor to set the Architecture and current Binary Instruction
    Executor(std::string arch_name, binaryPrep current_binary_instruction);

    // Constructor to set the Architecture and current Raw Binary Instruction
    Executor(std::string arch_name, std::string current_raw_binary_instruction);

    // Constructor to set the Architecture, current Assembly Instruction, current Binary Instruction and  current Raw Binary Instruction
    // used to run individual instructions
    Executor(
            std::string arch_name,
            assemblyPrep current_assembly_instruction,
            binaryPrep current_binary_instruction,
            std::string current_raw_binary_instruction);

    // Constructor to set the Architecture, all Assembly Instructions, all Binary Instructions and all Raw Binary Instructions
    // used to load all instructions and execute them in a sequence
    Executor(
            std::string arch_name,
            std::vector<assemblyPrep> all_assembly_instructions,
            std::vector<binaryPrep> all_binary_instructions,
            std::vector<std::string> all_raw_binary_instructions,
            std::vector<std::string> all_original_instructions
            );

    // Architecture supporting methods!

    // Getter - return the architecture name
    std::string getExecutorArchitectureName();
    // Getter - return all the corresponding registers
    std::vector<RegisterWrapper> getExecutorRegisters();
    // Setter - set a value on a particular Register used for LOADI
    void setExecutorRegister(std::string registerName, int value);
    // Getter - return a value for a particular Register
    int getExecutorRegisterValue(std::string registerName);
    // Setter - increment the value on a particular Register -- used to implement ADDR
    void setOpExecutorRegisterWithValue(std::string registerName, int value);

    // Instruction supporting methods!

    // Capture instructions
    // Setter - load all assembly instructions
    void loadAllAssemblyInstructions(std::vector<assemblyPrep> assembly_instructions);
    // Setter - load all binary instructions
    void loadAllBinaryInstructions(std::vector<binaryPrep> binary_instructions);
    // Setter - load all raw binary instructions
    void loadAllRawBinaryInstructions(std::vector<std::string> raw_binary_instructions);
    // Setter - load all original instructions
    void loadAllOriginalInstructions(std::vector<std::string> all_original_instructions);
    // Setter - load all available instructions in the object
    void loadAllInstructions(
            std::vector<assemblyPrep> assembly_instructions,
            std::vector<binaryPrep> binary_instructions,
            std::vector<std::string> raw_binary_instructions,
            std::vector<std::string> original_instructions
            );
    // Setter - set current assembly instruction
    void setCurrAssemblyInstruction(assemblyPrep assembly_instruction);
    // Setter - set current binary instruction
    void setCurrBinaryInstruction(binaryPrep binary_instruction);
    // Setter - set current raw binary instruction
    void setCurrRawBinaryInstruction(std::string raw_binary_instruction);
    // Getter - get current assembly instruction
    assemblyPrep getCurrAssemblyInstruction();
    // Getter - get current binary instruction
    binaryPrep getCurrBinaryInstruction();
    // Getter - get current raw binary instruction
    std::string getCurrRawBinaryInstruction();
    // Getter - get current program counter
    int getCurrProgramCounter();
    // Getter - get all executor states
    std::vector<executorState> getOpExecutorStates();
    // Getter = get program completion status
    bool getCompletionStatus();
    // Setter - set all current operating instruction
    void setCurrInstructionAll(
            assemblyPrep assembly_instruction,
            binaryPrep binary_instruction,
            std::string raw_binary_instruction);

    // Run instructions
    // Load a register with a value - used for LOADI
    void baseLoadR(assemblyPrep instruction);
    // Add a register with pre-existing value, with a new value - used for ADDRI
    void baseAddRI(assemblyPrep instruction);
    // Add values from two registers and save it in a 3rd register - used for ADDRR
    void baseAddRR(assemblyPrep instruction);
    // Compare values between two registers, if equal save 1 in a 3rd register, else 0 - used for EQUAL
    void baseEqualR(assemblyPrep instruction);
    // Compare values between two registers, if not equal save 1 in a 3rd register, else 0 - used for NQUAL
    void baseNqualR(assemblyPrep instruction);
    // Entry method for executing a single instruction
    void executeInstruction(assemblyPrep instruction);
    // PRIMARY Entry method for executing all instructions
    void executeAllInstructions();


};


#endif //HW2_INSTRUCTION_H
