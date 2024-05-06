#include "Parser.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <bitset>

// inner function to remove hash
std::string removeHash(const std::string& input) {
    // Check if the input string starts with '#'
    if (input.size() > 0 && input[0] == '#') {
        // If yes, return the substring starting from the second character
        return input.substr(1);
    } else {
        // If not, return the original string
        return input;
    }
}


// default
Parser::Parser() = default;


std::string Parser::getOrigInstruction(){
    return this->origInstruction;
};

// Tokenize
void Parser::tokenize(){
    // reference the original instruction
    std::string instruction = this->getOrigInstruction();
    // create a vector of strings to host tokenized output
    std::vector<std::string> tokens;
    // read the instruction as a stream to perform string manipulations
    std::stringstream ss(instruction);
    // host each token created by the comma delimiter
    std::string token;
    while(std::getline(ss, token, ' ')){
        // Remove any leading whitespaces from the token
        token.erase(0, token.find_first_not_of(", \t\n\r"));
        // Remove any trailing whitespaces from the token
        token.erase(token.find_last_not_of(", \t\n\r") + 1);
        // Add it to the tokens vector
        tokens.push_back(token);
    }
    // save it in private variable
    this->tokenInstruction = tokens;
}

// Getter - retrieve the vector of tokens
std::vector<std::string> Parser::getTokenInstructions() {
    return this->tokenInstruction;
}

// Setter - converts token to Binary
void Parser::tokenToBinary() {
    // reserve temp
    binaryPrep tempBinary;
    assemblyPrep tempAssembly;
    // parse the tokens
    for(int i = 0; i < this->getTokenInstructions().size(); i++){
        if(i==0){
            // first token will always be an instruction
            if(opCodeMap.count(this->getTokenInstructions()[i]) > 0) {
                tempBinary.opCode = opCodeMap[this->getTokenInstructions()[i]];
                tempAssembly.opCode = this->getTokenInstructions()[i];
            }
        } else{
            // remaining tokens will either registers or numbers
            if(regCodeMap.count(this->getTokenInstructions()[i]) > 0) {
                // if 1st register
                if(i==1){
                    tempBinary.destReg = regCodeMap[this->getTokenInstructions()[i]];
                    tempAssembly.destReg = this->getTokenInstructions()[i];
                } else if(i==2){
                    tempBinary.firstReg = regCodeMap[this->getTokenInstructions()[i]];
                    tempAssembly.firstReg = this->getTokenInstructions()[i];
                } else if(i==3){
                    tempBinary.secondReg = regCodeMap[this->getTokenInstructions()[i]];
                    tempAssembly.secondReg = this->getTokenInstructions()[i];
                } else {
                    // set nothing
                }
            } else {

                std::string foo = this->getTokenInstructions()[i];
                int fooClean = stoi(removeHash(foo));
                // save offset
                tempBinary.offset = std::bitset<8>(fooClean).to_string();
                tempAssembly.offset = fooClean;
            }
        }
    }
    // sanity checks -
    if(tempBinary.opCode == "1111") {
        tempBinary.destReg = "1111";
        tempBinary.firstReg = "1111";
        tempBinary.secondReg = "1111";
        tempBinary.offset = "1111";
    } else {
        if(tempBinary.firstReg.empty()){
            tempBinary.firstReg = "0000";
        }
        if(tempBinary.secondReg.empty()){
            tempBinary.secondReg = "0000";
        }
        if(tempBinary.offset.empty()){
            tempBinary.offset = "00000000";
        }
    }
    // save it
    this->binaryInstruction = tempBinary;
    // save the rawBinaryInstruction
    this->rawBinaryInstruction = tempBinary.opCode + tempBinary.destReg +
            tempBinary.firstReg + tempBinary.secondReg + tempBinary.unused + tempBinary.offset;
    this->assemblyInstruction = tempAssembly;
}

binaryPrep Parser::getBinaryInstructions() {
    return this->binaryInstruction;
}

std::string Parser::getRawBinaryInstructions() {
    return this->rawBinaryInstruction;
}

assemblyPrep Parser::getAssemblyInstructions() {
    return this->assemblyInstruction;
}

// complete Constructor
Parser::Parser(std::string wholeInstruction) {
    // save the original instruction
    this->origInstruction = wholeInstruction;
    // save the tokens as a vector
    this->tokenize();
    // convert tokens to binary
    this->tokenToBinary();
}
