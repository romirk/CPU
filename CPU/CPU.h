#pragma once
#include <string>
#include <bitset>
#include "definitions.h"
#include "exceptions.h"

#define bit(a, i)	(((a) >> (i)) & 1)

typedef unsigned char byte;

/**
* x64 CPU
*
* @author Romir Kulshrestha <https://github.com/romirk>
* @version 0.1
* @since 0.1
*/
class CPU
{


	byte memory[MEM_SIZE] = { 0 };
	void (*prog)();
	uint64_t registers[0x10] = { 0 };
	std::bitset<12> flags = 0;
	bool running = false;
	bool debug = false;

public:
	CPU() {
		//std::fill_n(registers, 0x10, 0);
		RSP = RBP = MEM_SIZE - 8;
		RIP = 0x10;
		CF = ZF = SF = AF = PF = 0;
		//std::fill_n(memory, MEM_SIZE, 0);

		Instruction::cpu = this;
	}

	// helper functions
	void load(void (*f)()) {
		prog = f;
	}

	void resetIP() {
		RIP = (uint64_t)0x10;
	}

	bool isReserved(uint64_t addr) {
		return addr < 0x10 || addr >= MAX_ADDRESSABLE - 8;
	}

	void dumpRegisters() {
		for (size_t i = 0; i < 0x10; i++)
		{
			std::cout << i << ": " << (long long)registers[i] << '\n';
		}
	}

	void dumpFlags() {
		std::cout << flags << '\n';
	}

	/**
	* convert byte to int
	*
	* @param b: byte
	* @return int
	*/
	static int btoi(byte b) {
		return ((int)b) & 0xff;
	}

	/**
	* reverse bits of a 64 bit number
	*
	* @param n: number to be reversed
	* @return bits of n in reverse order
	*/
	static uint64_t reverse(uint64_t n) {
		uint64_t r = 0;
		for (int i = 63; i >= 0; i--) {
			r |= (n & 1) << i;
			n >>= 1;
		}
		return r;
	}

	/**
	* perform binary addition on 2 64 bit numbers
	*
	* sets flags
	*
	* @param a: 64 bit number
	* @param b: 64 bit number
	* @return binary sum of a and b
	*/
	uint64_t binaryAdd(uint64_t a, uint64_t b) {
		//return (a + b) & 0xffffffffffffffff;
		uint64_t r = 0;
		byte c = 0, i = 0;
		while (i < 64) {
			if (a | b | c) {
				byte x = a & 1, y = b & 1;
				r |= x ^ y ^ c;
				c = ((x & y) | (x & c)) | (y & c);
				a >>= 1;
				b >>= 1;
			}

			r <<= 1;
			i++;
		}

		// set flags
		ZF = 0;								// clear ZF in case it was already set
		r = r ? reverse(r) : !(ZF = 1);		// don't waste cycles if r is 0. set ZF
		CF = c;								// carry flag
		OF = CF;							// there's an overflow if there's an unused carry?
		setParityFlag(r);					// set PF
		return r;
	}

	// sets PF
	void setParityFlag(uint64_t x) {
		byte b = x & 0xff;
		bool f = true;
		while (b) {
			if(b & 1)
				f = !f;
			b >>= 1;
		}
		PF = f;
	}

	// fetch methods
	uint64_t fetch64() {
		uint64_t r = fetch64at(RIP);
		RIP += 8;
		return r;
	}

	uint64_t fetch64at(uint64_t addr) {
		if (isReserved(addr))
			throw SegmentationFault();

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

	uint32_t fetch32at(uint64_t addr) {
		if (isReserved(addr))
			throw SegmentationFault();

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

	uint16_t fetch16at(uint64_t addr) {
		if (isReserved(addr))
			throw SegmentationFault();

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

	byte fetchat(uint64_t addr) {
		if (isReserved(addr))
			throw SegmentationFault();
		return memory[addr];
	}

	// write methods
	void write64at(uint64_t value, uint64_t addr) {
		if (isReserved(addr))
			throw SegmentationFault();

		int s = 0x38;
		uint64_t m = 0xff00000000000000;
		while (m) {
			memory[addr++] = (byte)((value & m) >> s);
			m >>= 8;
			s -= 8;
		}
	}

	void write32at(uint32_t value, uint64_t addr) {
		if (isReserved(addr))
			throw SegmentationFault();

		uint32_t m = 0xff000000;
		int s = 0x18;
		while (m) {
			memory[addr++] = (byte)((value & m) >> s);
			m >>= 8;
			s -= 8;
		}
	}

	void write16at(uint16_t value, uint64_t addr) {
		if (isReserved(addr))
			throw SegmentationFault();

		memory[addr++] = (byte)((value & 0xff00) >> 0x08);
		memory[addr] = (byte)(value & 0xff);
	}

	void write8at(byte value, uint64_t addr) {
		if (isReserved(addr))
			throw SegmentationFault();

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

	void write_instruction(byte i, uint32_t src = 0, uint32_t dst = 0) {
		uint64_t ins = (i & 0xffll) << 56;
		ins |= (src & 0xFFFFFFFll) << 28;
		ins |= dst & 0xFFFFFFFll;
		write64(ins);
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

		//instructions are fixed width: 8bit opcode + 48bit src + 8bit dst = 64bit instruction
		//registers are taken to be memory locations below 0x10

		uint64_t instruction = fetch64();
		byte opcode = (instruction >> 56) & 0xff;

		Instruction::src = (instruction >> 28) & 0xFFFFFFF;
		Instruction::dst = instruction & 0xFFFFFFF;
		
		switch (opcode)
		{
			// arithmetic
		case ADD: Instruction::add();
			break;

		case SUB: Instruction::sub();
			break;

		case MUL: Instruction::mul();
			break;

		/*case DIVL:
			src = fetch64();
			RDX = RAX % src;
			RAX /= src;
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
			break;*/

			//mov
		case MOV: Instruction::mov();
			break;

			//I/O
		case PUTC:
			putchar(RAX);
			break;

		default:
			std::cout << "Segmentation fault. (probably)\n"; 
		
		case EXIT:
			running = false;
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

	class Instruction {
	public:
		static uint64_t src, dst;

		//static byte instruction;
		static CPU* cpu;

		static void mov() {
			src = src < 0x10 ? cpu->registers[src] : src;
			if (dst < 0x10)
				cpu->registers[dst] = src;
			else
				cpu->write64at(src, dst);
		}
		
		static void add() {
			src = src < 0x10 ? cpu->registers[src] : src;
			cpu->registers[dst] = cpu->binaryAdd(cpu->registers[dst], src);
		}

		static void sub() {
			src = src < 0x10 ? cpu->registers[src] : src;
			cpu->registers[dst] -= src;
			cpu->SF = cpu->registers[dst] < 0;
			cpu->ZF = cpu->registers[dst] == 0;
			cpu->setParityFlag(cpu->registers[dst]);
		}

		static void mul() {
			src = src < 0x10 ? cpu->registers[src] : src;
			cpu->RAX *= src;
			cpu->ZF = cpu->RAX == 0;
			cpu->SF = cpu->RAX < 0;
			cpu->setParityFlag(cpu->RAX);
		}
	};
};

uint64_t CPU::Instruction::src = 0;
uint64_t CPU::Instruction::dst = 0;
CPU*	 CPU::Instruction::cpu = nullptr;

CPU cpu;