#include "Stack.h"

#include <stack>
#include <iostream>
#include <string>

// Default Constructor
StackWrapper::StackWrapper() {
    std::stack<int> temp;
    this->stackSize = 100;
    this->stackPointer = 0;
    this->StackImpl = temp;
}

// Constructor initialized with stack size
StackWrapper::StackWrapper(int stackSize) {
    stackSize = stackSize;
    stackPointer = 0;
}

// Getter method to return entire Stack
std::stack<int> StackWrapper::getStack() {
    return this->StackImpl;
}

// Getter method to return stack size
int StackWrapper::getStackSize() {
    return this->StackImpl.size();
}

// Setter method to push elements into the stack
void StackWrapper::pushStack(int element) {
    this->StackImpl.push(element);
}

// Print Stack
void StackWrapper::printStack() {
    // Create a temporary stack
    std::stack<int> temp = this->StackImpl;
    int tempSize = temp.size();
    // print zeros to show the elements
    while(tempSize > 0){
        std::cout << "0 ";
        tempSize--;
    }
    // print out elements along a line
    while(!temp.empty()){
        int item = temp.top();
        std::cout << item << " ";
        temp.pop();
    }
}