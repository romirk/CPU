// CPU.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include "CPU.h"

int main()
{
    void program();
    InstructionSet::setCPU(&cpu);       // for aux functions to write to correct memory
    cpu.load(program);                  // the program to be exeuted is passed as a param
    cpu.run();                          // and off we go!
}

void program() {
    // instructions go here
    using namespace InstructionSet;     //contains aux functions that write hex to memory
    std::string s = "Hello world!";
    for (char c : s) {  //technically for loops don't exist in x64 but I'm lazy and dont want to copy/paste this 20 times
        movl(c, 0);     //MOVQ  c, %rax
        putc();         //PUTCHAR
    }

    exit();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
