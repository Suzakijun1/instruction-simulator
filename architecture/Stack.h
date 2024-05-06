#ifndef HW2_STACK_H
#define HW2_STACK_H

#include <stack>
#include <string>

class StackWrapper{
    // Private data members
private:
    // pointer to stack
    int stackPointer;
    // elements in stack
    int stackSize;
    // C++ implementation of Stack
    std::stack<int> StackImpl;

    // Public methods
public:
    // Default Constructor
    StackWrapper();
    // Initialize Constructor
    StackWrapper(int stackSize);
    // Getter - returns C++ implementation of Stack
    std::stack<int> getStack();
    // Getter - returns stack size
    int getStackSize();
    // Setter - pushes an element to Stack
    void pushStack(int element);
    // Display Stack details
    void printStack();
};


#endif //HW2_STACK_H
