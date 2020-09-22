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
