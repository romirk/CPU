#pragma once
#include <string>
#include <bitset>
#include "InstructionSet.h"

//REGISTERS
#define		RAX		registers[0x00]
#define		RBX		registers[0x01]
#define		RCX		registers[0x02]
#define		RDX		registers[0x03]
#define		RSI		registers[0x04]
#define		RDI		registers[0x05]
#define		RSP		registers[0x06]
#define		RBP		registers[0x07]
#define		R8		registers[0x08]
#define		R9		registers[0x09]
#define		R10		registers[0x0A]
#define		R11		registers[0x0B]
#define		R12		registers[0x0C]
#define		R13		registers[0x0D]
#define		R14		registers[0x0E]
#define		RIP		registers[0x0F]

//Flags
#define		CF		flags[0]
#define		PF		flags[2]
#define		AF		flags[4]
#define		ZF		flags[6]
#define		SF		flags[7]

typedef unsigned char byte;

class CPU
{

	byte memory[0x100000] = { 0x0 };
	uint64_t registers[0x10];
	std::bitset<64> flags = 0;
	bool running = false;

public:
	CPU() {
		RSP = RBP = 0xffff8;
	}

	// helper functions
	/*std::string padLeftZeros(std::string s, unsigned int l) {
		if (s.length() >= l)
			return s;
		std::string r = "";
		while (r.length() < l - s.length())
			r += "0";
		return r + s;
	}*/

	void printMemory(int startingLocation) {
		printMemory(startingLocation, 100);
	}

	void printMemory(int startingLocation, int bytes) {
		for (int i = startingLocation; i < startingLocation + bytes; i++) {
			printf("0x%02X", memory[i]);
		}
	}

	int btoi(byte b) {
		return ((int)b) & 0xff;
	}

	uint64_t binaryAdd(uint64_t a, uint64_t b) {
		return (a + b) & 0xffffffffffffffff;
	}

	// fetch methods
	uint64_t fetch64() {
		uint64_t r = fetch64at(RIP);
		RIP += 8;
		return r;
	}

	uint64_t fetch64at(int addr) {
		uint64_t r = 0, m = 0x100000000000000;
		while (m) {
			r += btoi(memory[addr++]) * m;
			m >>= 2;
		}
		return r;	
	}
	
	uint32_t fetch32() {
		uint32_t r = fetch32at(RIP);
		RIP += 4;
		return r;
	}

	uint32_t fetch32at(int addr) {
		uint32_t r = 0, m = 0x1000000;
		while (m) {
			r += btoi(memory[addr++]) * m;
			m >>= 2;
		}
		return r;
	}

	uint16_t fetch16() {
		uint16_t r = fetch16at(RIP);
		RIP += 2;
		return r;
	}

	uint16_t fetch16at(int addr) {
		uint16_t r = 0, m = 0x100;
		while (m) {
			r += btoi(memory[addr++]) * m;
			m >>= 2;
		}
		return r;
	}

	byte fetch() {
		return fetchat(RIP++);
	}

	byte fetchat(int addr) {
		return memory[addr];
	}

	// write methods
	void write64at(uint64_t value, int addr) {
		memory[addr++] = (byte)((value & 0xff00000000000000) >> 0x38);
		memory[addr++] = (byte)((value & 0xff000000000000) >> 0x30);
		memory[addr++] = (byte)((value & 0xff0000000000) >> 0x28);
		memory[addr++] = (byte)((value & 0xff00000000) >> 0x20);
		memory[addr++] = (byte)((value & 0xff000000) >> 0x18);
		memory[addr++] = (byte)((value & 0xff0000) >> 0x10);
		memory[addr++] = (byte)((value & 0xff00) >> 0x08);
		memory[addr] = (byte)(value & 0xff);
	}

	void write32at(uint32_t value, int addr) {
		memory[addr++] = (byte)((value & 0xff000000) >> 0x18);
		memory[addr++] = (byte)((value & 0xff0000) >> 0x10);
		memory[addr++] = (byte)((value & 0xff00) >> 0x08);
		memory[addr] = (byte)(value & 0xff);
	}

	void write16at(uint16_t value, int addr) {
		memory[addr++] = (byte)((value & 0xff00) >> 0x08);
		memory[addr] = (byte)(value & 0xff);
	}

	void write8at(byte value, int addr) {
		memory[addr] = (byte)(value & 0xff);
	}

	//stack operations
	void push(uint64_t v) {
		RSP -= 8;
		write64at(v, (int)RSP);
	}

	uint64_t pop() {
		uint64_t r = fetch64at((int)RSP);
		RSP += 8;
		return r;
	}

	// execution methods
	void step() {
		byte instruction = fetch();
		uint64_t lit;
		int src, dst;
		switch (instruction)
		{
			// arithmetic
		case ADDL:
			lit = fetch64();
			dst = fetch();
			if (registers[dst] + lit > 0x100000000l)
				registers[dst] = binaryAdd(registers[dst], lit);
			break;

		case ADDR:
			src = fetch();
			dst = fetch();
			registers[dst] = binaryAdd(registers[src], registers[dst]);
			break;

		case SUBL:
			lit = fetch64();
			dst = fetch();
			registers[dst] -= lit;
			CF = registers[dst] < 0;
			ZF = registers[dst] == 0;
			break;

		case SUBR:
			src = fetch();
			dst = fetch();
			registers[dst] -= registers[src];
			CF = registers[dst] < 0;
			ZF = registers[dst] == 0;
			break;

		case MULL:
			lit = fetch64();
			RAX *= lit;
			break;

		case MULR:
			src = fetch();
			RAX *= registers[src];
			break;

		case DIVL:
			lit = fetch64();
			RDX = RAX % lit;
			RAX /= lit;
			break;

		case DIVR:
			src = fetch();
			RDX = RAX % registers[src];
			RAX /= registers[src];
			break;

		default:
			std::cout << "Segmentation fault. (probably)\n";
			break;
		}
	}

	void run() {
		running = true;
		while (running)
			step();
	}
};

