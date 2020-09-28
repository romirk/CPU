#include <iostream>
#include <string>
#include "CPU.h"

int main()
{
    void program();
    byte m[6] = { 0x41, 0x41, 0x41, 0x41, 0x41, 0x41 };
    uint64_t *p = (uint64_t*)&(m[3]);
    std::cout << */*(byte*)*/p << '\n' << m[3] << '\n';
    cpu.dumpFlags();
    cpu.load(program);                  // the program to be exeuted is passed as a param
    cpu.run();                          // and off we go!
}

void program() {
    
    std::string s = "Hello world!";
    for (char c : s) {                          //technically for loops don't exist in x64 but I'm lazy and dont want to copy/paste this 20 times
        cpu.write_instruction(MOV, c, 0);       //MOVQ  c, %rax
        cpu.write_instruction(PUTC);       //PUTCHAR
    }
    cpu.write_instruction(MOV, '\n', 0);
    cpu.write_instruction(PUTC);
    cpu.write_instruction(EXIT);
}
