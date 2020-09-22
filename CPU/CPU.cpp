// CPU.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "CPU.h"

int main()
{
    void program();
    cpu.load(program);
    cpu.run();
}

void program() {
    // instructions go here
    cpu.write8(MOV_LR); cpu.write64('H'); cpu.write8(0x0);      //MOVQ  $72, %rax
    cpu.write8(PUTC);

    cpu.write8(MOV_LR);
    cpu.write64('e');
    cpu.write8(0x0);
    cpu.write8(PUTC);
    cpu.write8(MOV_LR);
    cpu.write64('l');
    cpu.write8(0x0);
    cpu.write8(PUTC);
    cpu.write8(MOV_LR);
    cpu.write64('l');
    cpu.write8(0x0);
    cpu.write8(PUTC);
    cpu.write8(MOV_LR);
    cpu.write64('o');
    cpu.write8(0x0);
    cpu.write8(PUTC);
    cpu.write8(MOV_LR);
    cpu.write64(' ');
    cpu.write8(0x0);
    cpu.write8(PUTC);
    cpu.write8(MOV_LR);
    cpu.write64('w');
    cpu.write8(0x0);
    cpu.write8(PUTC);
    cpu.write8(MOV_LR);
    cpu.write64('o');
    cpu.write8(0x0);
    cpu.write8(PUTC);
    cpu.write8(MOV_LR);
    cpu.write64('r');
    cpu.write8(0x0);
    cpu.write8(PUTC);
    cpu.write8(MOV_LR);
    cpu.write64('l');
    cpu.write8(0x0);
    cpu.write8(PUTC);
    cpu.write8(MOV_LR);
    cpu.write64('d');
    cpu.write8(0x0);
    cpu.write8(PUTC);
    cpu.write8(EXIT);
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
