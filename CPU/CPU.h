#pragma once
#include <string>
#include <bitset>
#include "InstructionSet.h"

#define		MEM_SIZE	0x100000
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

public:
	byte memory[MEM_SIZE] = { 0x0 };
	void (*prog)();
	uint64_t registers[0x10];
	std::bitset<64> flags = 0;
	bool running = false;

	bool debug = false;

//public:
	CPU() {
		std::fill_n(registers, 0x10, 0);
		RSP = RBP = 0xffff8;
		CF = ZF = SF = AF = PF = 0;
		std::fill_n(memory, MEM_SIZE, 0);
	}

	// helper functions
	void dumpRegisters() {
		for (size_t i = 0; i < 0x10; i++)
		{
			std::cout << i << ": " << (long long) registers[i] << '\n';
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
			m >>= 8;
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
			m >>= 8;
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
			m >>= 8;
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
		int s = 0x38;
		uint64_t m = 0xff00000000000000;
		while (m) {
			memory[addr++] = (byte)((value & m) >> s);
			m >>= 8;
			s -= 8;
		}
	}

	void write32at(uint32_t value, int addr) {
		uint32_t m = 0xff000000;
		int s = 0x18;
		while (m) {
			memory[addr++] = (byte)((value & m) >> s);
			m >>= 8;
			s -= 8;
		}
	}

	void write16at(uint16_t value, int addr) {
		memory[addr++] = (byte)((value & 0xff00) >> 0x08);
		memory[addr] = (byte)(value & 0xff);
	}

	void write8at(byte value, int addr) {
		memory[addr] = (byte)(value & 0xff);
	}

	void write64(uint64_t val) {
		write64at(val, RIP);
		RIP += 8;
	}

	void write32(uint32_t val) {
		write32at(val, RIP);
		RIP += 4;
	}

	void write16(uint16_t val) {
		write16at(val, RIP);
		RIP += 2;
	}

	void write8(byte val) {
		write8at(val, RIP);
		RIP++;
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

	void load(void (*f)()) {
		prog = f;
	}

	void resetIP() {
		RIP = (uint64_t)0;
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

		case INC:
			dst = fetch();
			registers[dst]++;
			break;

		case DEC:
			dst = fetch();
			registers[dst]--;
			break;

			//mov
		case MOV_LR:
			lit = fetch64();
			dst = fetch();
			registers[dst] = lit;
			break;

		case MOV_RR:
			src = fetch();
			dst = fetch();
			registers[dst] = registers[src];
			break;

		//I/O
		case PUTC:
			src = RAX;
			putchar(src);
			break;

		case EXIT:
			running = false;
			break;

		default:
			std::cout << "Segmentation fault. (probably)\n";
			break;
		}
	}

	void run() {
		running = true;
		prog();
		resetIP();
		while (running) {
			if (debug) dumpRegisters();
			step();
		}
	}
} cpu;

