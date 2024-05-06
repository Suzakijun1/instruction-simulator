//
// Created by vinee on 4/30/2024.
//

#include "Architecture.h"
#include "Registers.h"
#include "Stack.h"
#include <vector>
#include <iostream>
#include <string>

// Default Constructor
ArchitectureWrapper:: ArchitectureWrapper()=default;

// Constructor
ArchitectureWrapper::ArchitectureWrapper(std::string name) {
    // save the name
    this->ArchName = name;
    // Set the stack
    this->createStack();
    // Set the vector of registers
    this->createRegisters();
}

// Initialize a stack
void ArchitectureWrapper::createStack() {
    this->ArchStack = StackWrapper();
}

// Initialize registers
void ArchitectureWrapper::createRegisters() {
    // Reserve space to avoid reallocation
    this->ArchRegisters.reserve(16); // Assuming 16 registers in total
    // Lookup
    std::vector<std::string> register_lkp = {
            "R0",  "R1", "R2", "R3",
            "R4", "R5", "R6", "R7",
            "R8", "R9", "R10", "R11",
            "R12", "R13", "R14", "R15"
    };
    // Iterate
    for(const auto& reg_name: register_lkp){
        this->ArchRegisters.emplace_back(reg_name);
    }
}

// Getter - return Architecture name
std::string ArchitectureWrapper::getArchName() {
    return this->ArchName;
}

// Getter - return the vector of Registers
std::vector<RegisterWrapper>& ArchitectureWrapper::getArchRegisters(){
    return this->ArchRegisters;
}

StackWrapper& ArchitectureWrapper::getArchStack(){
    return this->ArchStack;
}

// Display all registers
void ArchitectureWrapper::displayRegisters() {
    std::cout << std::endl;
    std::vector<RegisterWrapper> tmp = this->getArchRegisters();
    for(int i = 0; i < tmp.size(); i++){
       std::cout << "Register name: " <<  tmp[i].getRegisterName() << "  Register Contents: " << tmp[i].getRegisterValue() << std::endl;
    }
    std::cout << std::endl;
}

// Setter - Select a register and load a value
void ArchitectureWrapper::selRegisterWithValue(std::string register_name, int value){
    // iterate over the registers and set the value if there was a hit for the register name
    for(int i=0; i < this->getArchRegisters().size(); i++){
        if(register_name == this->getArchRegisters()[i].getRegisterName()){
            this->getArchRegisters()[i].setRegisterValue(value);
        }
    }
}

// Getter - Select a register and get a value
int ArchitectureWrapper::getRegisterWithValue(std::string register_name){
    int tmp = -9999;
    // iterate over the registers and set the value if there was a hit for the register name
    for(int i=0; i < this->getArchRegisters().size(); i++){
        if(register_name == this->getArchRegisters()[i].getRegisterName()){
            tmp = this->getArchRegisters()[i].getRegisterValue();
        }
    }
    // return valid value else -9999
    return tmp;
}

// Display stack
void ArchitectureWrapper::displayStack() {
    std::cout << "STACK INFORMATION" << std::endl;
    this->getArchStack().printStack();
}

// Display entire Architecture
void ArchitectureWrapper::displayArch() {
    this->displayRegisters();
    this->displayStack();
}

void ArchitectureWrapper::incRegisterWithValue(std::string register_name, int value) {
    // iterate over the registers and set the value if there was a hit for the register name
    for(int i=0; i < this->getArchRegisters().size(); i++){
        if(register_name == this->getArchRegisters()[i].getRegisterName()){
            int tmp = this->getArchRegisters()[i].getRegisterValue() + value;
            this->getArchRegisters()[i].setRegisterValue(tmp);
        }
    }
}
