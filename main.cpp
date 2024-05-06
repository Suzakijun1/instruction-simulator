#include <fstream>
#include <iostream>
#include <vector>
#include "architecture/Parser.h"
#include "architecture/Architecture.h"
#include "architecture/Executor.h"
#include "nlohmann/json.hpp"

// high-level function to parse Executor State and present as JSON string
std::string prepJsonFromStruct(executorState state)
{
    // declare temp object for json response
    nlohmann::json jsonTemp;
    // set all Assembly instructions -
    jsonTemp["assemblyInstructions"] = state.all_original_instructions;
    // set all Binary instructions -
    jsonTemp["binaryInstructions"] = state.all_raw_binary_instructions;
    // set program counter
    jsonTemp["programCounter"] = state.programCounter;
    // set current assembly instruction
    jsonTemp["currentAssemblyInstruction"] = (state.programCounter == -1) ? "N/A" : state.all_original_instructions[state.programCounter];
    // set current binary instruction
    jsonTemp["currentBinaryInstruction"] = (state.programCounter == -1) ? "N/A" : state.all_raw_binary_instructions[state.programCounter];
    // set current Assembly OpCode
    jsonTemp["currentAssemblyOpCode"] = (state.programCounter == -1) ? "N/A" : state.current_assembly_instruction.opCode;
    // set current Binary OpCode
    jsonTemp["currentBinaryOpCode"] = (state.programCounter == -1) ? "N/A" : state.current_binary_instruction.opCode;

    // set current Assembly Destination Register
    jsonTemp["currentAssemblyDestReg"] = (state.programCounter == -1) ? "N/A" : state.current_assembly_instruction.destReg;
    // set current Binary Destination Register
    jsonTemp["currentBinaryDestReg"] = (state.programCounter == -1) ? "N/A" : state.current_binary_instruction.destReg;

    // set current Assembly firstReg Register
    jsonTemp["currentAssemblyFirstReg"] = (state.programCounter == -1) ? "N/A" : state.current_assembly_instruction.firstReg;
    // set current Binary Destination Register
    jsonTemp["currentBinaryFirstReg"] = (state.programCounter == -1) ? "N/A" : state.current_binary_instruction.firstReg;

    // set current Assembly secondReg Register
    jsonTemp["currentAssemblySecondReg"] = (state.programCounter == -1) ? "N/A" : state.current_assembly_instruction.secondReg;
    // set current Binary secondReg Register
    jsonTemp["currentBinarySecondReg"] = (state.programCounter == -1) ? "N/A" : state.current_binary_instruction.secondReg;

    // set current Assembly offset
    jsonTemp["currentAssemblyOffset"] = (state.programCounter == -1) ? -1 : state.current_assembly_instruction.offset;
    // set current Binary offset
    jsonTemp["currentBinaryOffset"] = (state.programCounter == -1) ? "N/A" : state.current_binary_instruction.offset;

    // set Register values
    jsonTemp["R0"] = state.R0;
    jsonTemp["R1"] = state.R1;
    jsonTemp["R2"] = state.R2;
    jsonTemp["R3"] = state.R3;
    jsonTemp["R4"] = state.R4;
    jsonTemp["R5"] = state.R5;
    jsonTemp["R6"] = state.R6;
    jsonTemp["R7"] = state.R7;
    jsonTemp["R8"] = state.R8;
    jsonTemp["R9"] = state.R9;
    jsonTemp["R10"] = state.R10;
    jsonTemp["R11"] = state.R11;
    jsonTemp["R12"] = state.R12;
    jsonTemp["R13"] = state.R13;
    jsonTemp["R14"] = state.R14;
    jsonTemp["R15"] = state.R15;

    // Capture the state
    jsonTemp["programStatus"] = state.programStatus;

    // return string version
    return jsonTemp.dump(4);
}

// high-level function to present Executor status as JSON payloads
std::string convertExecStatusToJSON(std::vector<executorState> OpState)
{
    // declare an Array string -
    std::string jsonArrayStr = "\"[";
    // process through the executor states
    for (size_t i = 0; i < OpState.size(); i++)
    {
        // Convert state struct info into JSON string in tmp variable
        std::string tmp = prepJsonFromStruct(OpState[i]);
        // Concatenate tmp with jsonArrayStr
        jsonArrayStr += tmp;
        if (i < OpState.size() - 1)
        {
            // add a trailing comma
            jsonArrayStr += ",";
        }
    }
    // Added a trailing ])
    jsonArrayStr += "]\"";
    return jsonArrayStr;
}

int main()
{
    // Use filestream to open a JSON native file
    std::fstream jsonFile;
    // Set the location
    std::string fileLocation = R"(C:\Users\Dylan\Documents\Syracuse\my-instruction-simulator\tests\instructions.json)";
    jsonFile.open(fileLocation.c_str(), std::ios::in);
    // Check if the file opened successfully
    if (!jsonFile.is_open())
    {
        // Handle error opening file
        return 1;
    }
    // parse the filestream as JSON using nlohmann library
    nlohmann::json jsonData = nlohmann::json::parse(jsonFile);
    // declare a vector all of original instructions;
    std::vector<std::string> allOriginalInstructions;
    // declare a vector of all assembly instructions!
    std::vector<assemblyPrep> allAssemblyInstructions;
    // declare a vector of all binary instructions!
    std::vector<binaryPrep> allBinaryInstructions;
    // declare a vector of all raw binary instructions!
    std::vector<std::string> allRawBinaryInstructions;

    // let's iterate over jsonData using an iterator container and load into the vector - acmeBagVector
    for (auto &it : jsonData)
    {
        // std::cout << it.at("instruction") <<  std::endl;
        Parser obj = Parser(it.at("instruction"));
        // Load Original instruction
        allOriginalInstructions.emplace_back(obj.getOrigInstruction());
        // Load Assembly instructions
        allAssemblyInstructions.emplace_back(obj.getAssemblyInstructions());
        // Load Binary instructions
        allBinaryInstructions.emplace_back(obj.getBinaryInstructions());
        // Load Raw Binary instructions
        allRawBinaryInstructions.emplace_back(obj.getRawBinaryInstructions());
    }

    // Close the file
    jsonFile.close();

    // let's invoke the Executor
    Executor operatingRISC = Executor(
        "RISC",
        allAssemblyInstructions,
        allBinaryInstructions,
        allRawBinaryInstructions,
        allOriginalInstructions);
    // let's run the entry method
    operatingRISC.executeAllInstructions();

    // prep an output
    std::string operatingRISCResults = convertExecStatusToJSON(operatingRISC.getOpExecutorStates());

    std::cout << operatingRISCResults << std::endl;

    return 0;
}
