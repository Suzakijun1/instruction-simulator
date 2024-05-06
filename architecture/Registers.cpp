#include "Registers.h"

// Default constructor
RegisterWrapper::RegisterWrapper() {
    // nothing
}

// Constructor to set register name
RegisterWrapper::RegisterWrapper(std::string name) {
    this->registerName = name;
    this->registerValue = 0;
}

// Getter Register name
std::string RegisterWrapper::getRegisterName(){
    return this->registerName;
}

// Getter Register value
int RegisterWrapper::getRegisterValue(){
    return this->registerValue;
}

// Setter Register Value
void RegisterWrapper::setRegisterValue(int value){
    this->registerValue = value;
}

// Constructor to set register value
RegisterWrapper::RegisterWrapper(std::string name, int value) {
    this->registerName = name;
    this->setRegisterValue(value);
}
