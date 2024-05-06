#ifndef HW2_REGISTERS_H
#define HW2_REGISTERS_H

#include <vector>
#include <string>

class RegisterWrapper{
    // Private data members
private:
    // Register name
    std::string registerName;
    // Register implementation
    int registerValue;

    // Public methods
public:
    // Default Constructor
    RegisterWrapper();
    // Constructor to set register name
    RegisterWrapper(std::string name);
    // Constructor to set register value
    RegisterWrapper(std::string registerName, int value);
    // Getter Register name
    std::string getRegisterName();
    // Getter Register Value
    int getRegisterValue();
    // Setter Register Value
    void setRegisterValue(int value);

};


#endif //HW2_REGISTERS_H
