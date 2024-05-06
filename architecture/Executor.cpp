//
// Created by vinee on 5/1/2024.
//

#include <iostream>
#include "Executor.h"
#include "../nlohmann/json.hpp"

// initialize Constructor with Architecture name
Executor::Executor(std::string arch_name) {
    this->simpleRISC = ArchitectureWrapper(arch_name);
}

std::string Executor::getExecutorArchitectureName() {
    return this->simpleRISC.getArchName();
}

std::vector<RegisterWrapper> Executor::getExecutorRegisters() {
    return this->simpleRISC.getArchRegisters();
}

void Executor::setExecutorRegister(std::string registerName, int value){
    this->simpleRISC.selRegisterWithValue(registerName, value);
}

int Executor::getExecutorRegisterValue(std::string registerName) {
    return this->simpleRISC.getRegisterWithValue(registerName);
}

void Executor::setOpExecutorRegisterWithValue(std::string registerName, int value) {
    return this->simpleRISC.incRegisterWithValue(registerName, value);
}


// Setter - load all assembly instructions
void Executor::loadAllAssemblyInstructions(std::vector<assemblyPrep> assembly_instructions) {
    // set
    this->allAssemblyInstructions = assembly_instructions;
}

// Setter - load all binary instructions
void Executor::loadAllBinaryInstructions(std::vector<binaryPrep> binary_instructions) {
    // set
    this->allBinaryInstructions = binary_instructions;
}

// Setter - load all raw binary instructions
void Executor::loadAllRawBinaryInstructions(std::vector<std::string> raw_binary_instructions) {
    // set
    this->allRawBinaryInstructions = raw_binary_instructions;
}


// Setter - load all original instructions
void Executor::loadAllOriginalInstructions(std::vector<std::string> all_original_instructions) {
    // set
    this->allOriginalInstructions = all_original_instructions;

}


// Setter - load all available instructions in the object
void Executor::loadAllInstructions(std::vector<assemblyPrep> assembly_instructions,
                                  std::vector<binaryPrep> binary_instructions,
                                  std::vector<std::string> raw_binary_instructions,
                                  std::vector<std::string> original_instructions) {
    // set private members
    this->loadAllAssemblyInstructions(assembly_instructions);
    this->loadAllBinaryInstructions(binary_instructions);
    this->loadAllRawBinaryInstructions(raw_binary_instructions);
    this->loadAllOriginalInstructions(original_instructions);
}

// Setter - set current assembly instruction
void Executor::setCurrAssemblyInstruction(assemblyPrep assembly_instruction) {
    // set
    this->curAssemblyInstruction = assembly_instruction;
}

// Setter - set current binary instruction
void Executor::setCurrBinaryInstruction(binaryPrep binary_instruction) {
    // set
    this->curBinaryInstruction = binary_instruction;
}

// Setter - set current raw binary instruction
void Executor::setCurrRawBinaryInstruction(std::string raw_binary_instruction) {
    // set
    this->curRawBinaryInstruction = raw_binary_instruction;
}

// Setter - set current operating instruction
void Executor::setCurrInstructionAll(
        assemblyPrep assembly_instruction,
        binaryPrep binary_instruction,
        std::string raw_binary_instruction) {
    // set
    this->setCurrAssemblyInstruction(assembly_instruction);
    this->setCurrBinaryInstruction(binary_instruction);
    this->setCurrRawBinaryInstruction(raw_binary_instruction);
}

// Getter - get current assembly instruction
assemblyPrep Executor::getCurrAssemblyInstruction() {
    return this->curAssemblyInstruction;
}

// Getter - get current binary instruction
binaryPrep Executor::getCurrBinaryInstruction() {
    return this->curBinaryInstruction;
}

// Getter - get current program counter
int Executor::getCurrProgramCounter() {
    return this->programCounter;
}

// Getter - get Executor outputs
std::vector<executorState> Executor::getOpExecutorStates(){
    return this->executorOutputs;
}

// Getter = get program completion status
bool Executor::getCompletionStatus() {
    return this->programComplete;
}

// Constructor to set the Architecture and current Assembly Instruction
Executor::Executor(std::string arch_name, assemblyPrep current_assembly_instruction) {
    // set architecture
    this->simpleRISC = ArchitectureWrapper(arch_name);
    this->setCurrAssemblyInstruction(current_assembly_instruction);
    // initialize all program counters to -1 !
    this->programCounter = -1;
    this->programComplete = false;
}

// Constructor to set the Architecture and current Binary Instruction
Executor::Executor(std::string arch_name, binaryPrep current_binary_instruction) {
    // set architecture
    this->simpleRISC = ArchitectureWrapper(arch_name);
    this->setCurrBinaryInstruction(current_binary_instruction);
    // initialize all program counters to -1 !
    this->programCounter = -1;
    this->programComplete = false;
}

// Constructor to set the Architecture and current Raw Binary Instruction
Executor::Executor(std::string arch_name, std::string current_raw_binary_instruction) {
    this->simpleRISC = ArchitectureWrapper(arch_name);
    this->setCurrRawBinaryInstruction(current_raw_binary_instruction);
    // initialize all program counters to -1 !
    this->programCounter = -1;
    this->programComplete = false;
}

// Constructor to set the Architecture, current Assembly Instruction, current Binary Instruction and  current Raw Binary Instruction
// Used for executing single set of instructions
Executor::Executor(std::string arch_name,
                   assemblyPrep current_assembly_instruction,
                   binaryPrep current_binary_instruction,
                   std::string current_raw_binary_instruction){
    this->simpleRISC = ArchitectureWrapper(arch_name);
    this->setCurrInstructionAll(current_assembly_instruction,
                                current_binary_instruction,
                                current_raw_binary_instruction
                                );
    // initialize all program counters to -1 !
    this->programCounter = -1;
    this->programComplete = false;
}

// Constructor to set the Architecture, all Assembly Instructions, all Binary Instructions and all Raw Binary Instructions
// used to load all instructions and execute them in a sequence
Executor::Executor(std::string arch_name,
                   std::vector<assemblyPrep> all_assembly_instructions,
                   std::vector<binaryPrep> all_binary_instructions,
                   std::vector<std::string> all_raw_binary_instructions,
                   std::vector<std::string> all_original_instructions) {
    // set values
    this->simpleRISC = ArchitectureWrapper(arch_name);
    this->loadAllInstructions(
            all_assembly_instructions,
            all_binary_instructions,
            all_raw_binary_instructions,
            all_original_instructions
            );
    // initialize all program counters to -1 !
    this->programCounter = -1;
    this->programComplete = false;
}


// Base instruction methods!
// Load a register with a value
void Executor::baseLoadR(assemblyPrep instruction) {
    // This instruction is used to load a register with a value
    this->setExecutorRegister(instruction.destReg, instruction.offset);
    // increment the program counter
    this->programCounter++;
}

// Add a register with pre-existing value, with a new value - used for ADDRI
void Executor::baseAddRI(assemblyPrep instruction) {
    // This instruction is used to increment existing register with a value
    this->setOpExecutorRegisterWithValue(instruction.destReg, instruction.offset);
    // increment the program counter
    this->programCounter++;
}

// Add values from two registers and save it in a 3rd register - used for ADDRR
void Executor::baseAddRR(assemblyPrep instruction) {
    // Retrieve values from first register
    int tmp1 = this->getExecutorRegisterValue(instruction.firstReg);
    // Retrieve values from second register
    int tmp2 = this->getExecutorRegisterValue(instruction.secondReg);
    // Load the sum to Destination Register
    this->setExecutorRegister(instruction.destReg, tmp1+tmp2);
    // increment the program counter
    this->programCounter++;
}

// Compare values between two registers, if equal save 1 in a 3rd register, else 0 - used for EQUAL
void Executor::baseEqualR(assemblyPrep instruction) {
    // Retrieve values from first register
    int tmp1 = this->getExecutorRegisterValue(instruction.firstReg);
    // Retrieve values from second register
    int tmp2 = this->getExecutorRegisterValue(instruction.secondReg);
    if(tmp1==tmp2){
        // Both values are same - set to 1
        this->setExecutorRegister(instruction.destReg, 1);
    } else {
        // Both values are not same - set to 0
        this->setExecutorRegister(instruction.destReg, 0);
    }
    // increment the program counter
    this->programCounter++;
}

// Compare values between two registers, if not equal save 1 in a 3rd register, else 0 - used for NQUAL
void Executor::baseNqualR(assemblyPrep instruction) {
    // Retrieve values from first register
    int tmp1 = this->getExecutorRegisterValue(instruction.firstReg);
    // Retrieve values from second register
    int tmp2 = this->getExecutorRegisterValue(instruction.secondReg);
    if(tmp1!=tmp2){
        // Both values are not same - set to 1
        this->setExecutorRegister(instruction.destReg, 1);
    } else {
        // Both values are same - set to 0
        this->setExecutorRegister(instruction.destReg, 0);
    }
    // increment the program counter
    this->programCounter++;
}

// Entry method for executing a single instruction
void Executor::executeInstruction(assemblyPrep instruction) {
    // inspect the instruction
    if (instruction.opCode == "LOADI"){
        // This instruction is used to load a register with a value
        this->baseLoadR(instruction);
    } else if(instruction.opCode == "ADDRI"){
        // This instruction is used to increment existing register with a value
        this->baseAddRI(instruction);
    } else if(instruction.opCode == "ADDRR"){
        // This instruction is used to add values from two registers and save it in a 3rd register
        this->baseAddRR(instruction);
    } else if(instruction.opCode == "EQUAL"){
        // This instruction is used to compare values between two registers, if equal save 1 in a 3rd register, else 0
        this->baseEqualR(instruction);
    } else if(instruction.opCode == "NQUAL"){
        // This instruction is used to compare values between two registers, if not equal save 1 in a 3rd register, else 0
        this->baseNqualR(instruction);
    } else if(instruction.opCode == "BRNCH"){
        // This instruction is used to branch to another location in execution "memory" a.k.a allAssemblyInstructions
        // this->baseBrnchR(instruction);
    } else {
        // do nothing!
    }
}

// Entry method for executing all instructions
void Executor::executeAllInstructions() {
    // declare a temp variable of struct executorState type to capture state
    executorState tmp;
    // save all inputs
    tmp.all_assembly_instructions = this->allAssemblyInstructions;
    tmp.all_binary_instructions = this->allBinaryInstructions;
    tmp.all_raw_binary_instructions = this->allRawBinaryInstructions;
    tmp.all_original_instructions = this->allOriginalInstructions;
    // save current state of registers
    tmp.R0 = this->getExecutorRegisterValue("R0");
    tmp.R1 = this->getExecutorRegisterValue("R1");
    tmp.R2 = this->getExecutorRegisterValue("R2");
    tmp.R3 = this->getExecutorRegisterValue("R3");
    tmp.R4 = this->getExecutorRegisterValue("R4");
    tmp.R5 = this->getExecutorRegisterValue("R5");
    tmp.R6 = this->getExecutorRegisterValue("R6");
    tmp.R7 = this->getExecutorRegisterValue("R7");
    tmp.R8 = this->getExecutorRegisterValue("R8");
    tmp.R9 = this->getExecutorRegisterValue("R9");
    tmp.R10 = this->getExecutorRegisterValue("R10");
    tmp.R11 = this->getExecutorRegisterValue("R11");
    tmp.R12 = this->getExecutorRegisterValue("R12");
    tmp.R13 = this->getExecutorRegisterValue("R13");
    tmp.R14 = this->getExecutorRegisterValue("R14");
    tmp.R15 = this->getExecutorRegisterValue("R15");
    // set program counter in the struct
    tmp.programCounter = this->programCounter;
    // declare a vector of executorState structs
    std::vector<executorState> allStates;
    // add tmp to vector
    allStates.emplace_back(tmp);
    // the "memory" of instructions would be - allAssemblyInstructions
    // we are iterating it over it and executing them as we go!
    for(int i=0; i < this->allAssemblyInstructions.size(); i++){
        // set the current operating assembly instruction
        this->curAssemblyInstruction = this->allAssemblyInstructions[i];
        // set the current operating binary instruction
        this->curBinaryInstruction = this->allBinaryInstructions[i];
        // set the current operating raw binary instruction
        this->curRawBinaryInstruction = this->allRawBinaryInstructions[i];
        // execute the instruction
        this->executeInstruction(this->getCurrAssemblyInstruction());
        // capture assembly instruction
        tmp.current_assembly_instruction = this->allAssemblyInstructions[i];
        // capture binary instruction
        tmp.current_binary_instruction = this->allBinaryInstructions[i];
        // capture raw binary instruction
        tmp.current_raw_binary_instruction = this->allRawBinaryInstructions[i];
        // save current state of registers
        tmp.R0 = this->getExecutorRegisterValue("R0");
        tmp.R1 = this->getExecutorRegisterValue("R1");
        tmp.R2 = this->getExecutorRegisterValue("R2");
        tmp.R3 = this->getExecutorRegisterValue("R3");
        tmp.R4 = this->getExecutorRegisterValue("R4");
        tmp.R5 = this->getExecutorRegisterValue("R5");
        tmp.R6 = this->getExecutorRegisterValue("R6");
        tmp.R7 = this->getExecutorRegisterValue("R7");
        tmp.R8 = this->getExecutorRegisterValue("R8");
        tmp.R9 = this->getExecutorRegisterValue("R9");
        tmp.R10 = this->getExecutorRegisterValue("R10");
        tmp.R11 = this->getExecutorRegisterValue("R11");
        tmp.R12 = this->getExecutorRegisterValue("R12");
        tmp.R13 = this->getExecutorRegisterValue("R13");
        tmp.R14 = this->getExecutorRegisterValue("R14");
        tmp.R15 = this->getExecutorRegisterValue("R15");
        // set program counter in the struct
        tmp.programCounter = this->programCounter;
        if(i == this->allAssemblyInstructions.size()-1){
            // if last instruction - set program status in state to true
            tmp.programStatus = true;
            // Set the program complete flag to true
            this->programComplete = true;
        } else {
            tmp.programStatus = false;
        }
        // add tmp to vector
        allStates.emplace_back(tmp);
    }
    // Save the atomic state changes to the object
    this->executorOutputs = allStates;
}

