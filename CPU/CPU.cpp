#include <iostream>
#include <string>
#include "CPU.h"

int main()
{
    void program();
    //cpu.dumpFlags();
    cpu.load(program);                  // the program to be exeuted is passed as a param
    cpu.run();                          // and off we go!
}

/**
 aux function to write program into memory so I don't have to do it manually
*/
void program() {
    std::string s = "Hello world!";
    for (char c : s) {                                          //technically for loops don't exist in x64 but I'm lazy and dont want to copy/paste this 20 times
        cpu.write_instruction(MOVL, c, 0, false, true);         //MOVQ  c, %rax
        cpu.write_instruction(PUTC);       //PUTCHAR
    }
    cpu.push('\n');
    cpu.write_instruction(MOV, cpu.sp(), 0, false, true);
    cpu.write_instruction(PUTC);
    cpu.write_instruction(EXIT);
}
