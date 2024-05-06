#ifndef HW2_PARSER_H
#define HW2_PARSER_H

#include <map>
#include <vector>
#include <string>
#include <sstream>


// Struct for composing instruction properties
struct binaryPrep{
    // 1st octet
    std::string opCode;
    // 2nd octet
    std::string destReg;
    // 3rd octet
    std::string firstReg;
    // 4th octet
    std::string secondReg;
    // 5th octet ** unused **
    std::string unused="0000";
    // 6th octet
    std::string offset;
};

// Struct for composing instruction properties in plain english
struct assemblyPrep{
    // 1st octet
    std::string opCode;
    // 2nd octet
    std::string destReg;
    // 3rd octet
    std::string firstReg;
    // 4th octet
    std::string secondReg;
    // 5th octet ** unused **
    std::string unused="N/A";
    // 6th octet
    int offset;
};

class Parser{

private:
    // Original instruction
    std::string origInstruction;
    // Convert original instruction to tokens
    std::vector<std::string> tokenInstruction;
    // Capture binary numbers
    std::string rawBinaryInstruction;
    // Capture binary instruction in struct
    binaryPrep binaryInstruction;
    // Capture assembly instruction in struct
    assemblyPrep assemblyInstruction;
    // Detect a branch instruction
    // bool jumpInstruction;

public:
    // Map to contain opcodes for instructions
    std::map<std::string, std::string> opCodeMap = {
            {"LOADI", "0001"}, // LOADI R0, #10
            {"ADDRR", "0010"}, // ADDRR R1, R2, R3
            {"ADDRI", "0011"}, // ADDRI R1, #10
            {"BRNCH", "0100"}, // BRNCH <INSTRUCTION>
            {"EQUAL", "0101"}, // EQUAL R4, R5, R6
            {"NQUAL", "0110"}, // NQUAL R4, R5, R6
            {"CLOSE", "1111"}, // CLOSE
    };

    // Map to contain opcodes for registers
    std::map<std::string, std::string> regCodeMap = {
            {"R0", "0000"},
            {"R1", "0001"},
            {"R2", "0010"},
            {"R3", "0011"},
            {"R4", "0100"},
            {"R5", "0101"},
            {"R6", "0110"},
            {"R7", "0111"},
            {"R8", "1000"},
            {"R9", "1001"},
            {"R10", "1010"},
            {"R11", "1011"},
            {"R12", "1100"},
            {"R13", "1101"},
            {"R14", "1110"},
            {"R15", "1111"}
    };

    // Default Constructor
    Parser();
    Parser(std::string wholeInstruction);
    // Getter - return original string instruction
    std::string getOrigInstruction();
    // Setter - Tokenize the original instructions
    void tokenize();
    // Getter = return tokenized instruction
    std::vector<std::string> getTokenInstructions();
    // Setter - convert tokens to binary
    void tokenToBinary();
    // Getter - return struct
    binaryPrep getBinaryInstructions();
    // Getter - return raw binary in string
    std::string getRawBinaryInstructions();
    // Getter - return assembly in string
    assemblyPrep getAssemblyInstructions();

};


#endif //HW2_PARSER_H
