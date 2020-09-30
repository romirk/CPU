#pragma once

#define		OPCODE_SIZE			8
#define		OPERAND_SIZE		27
#define		INSTRUCTION_SIZE	64

constexpr uint64_t MEM_SIZE = (uint64_t)1 << (OPERAND_SIZE + 1);
constexpr uint32_t MAX_ADDRESSABLE = 1 << OPERAND_SIZE;
constexpr uint32_t STACK_LIMIT = MAX_ADDRESSABLE + 8;

//bitshift drudgework done with these
#define bit(a, i)			(((a) >> (i)) & 1)
#define ones(n)				(~(uint64_t(0)) >> (64 - (n)))
#define extract(x, s, n)	(((x) >> ((s) - (n))) & ones(n))
#define encode(x, v, s, n)	(x) |= (ones(n) & v) << ((s) - (n))

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
#define		OF		flags[11]

// OPCODES
#define		ADD		0X00
#define		ADDL	0X01
#define		PUSH	0X04
#define		POP		0X05
#define		PUSHL	0X06
#define		POPL	0X07
#define		OR		0X08
#define		NOT		0X10
#define		AND		0X20
#define		SUB		0X28
#define		SUBL	0X29
#define		XOR		0X30

#define		INC		0X40
#define		DEC		0X48
#define		MUL		0X50
#define		DIV		0X60
#define		DIVL	0X61

#define		JZ		0X70
#define		JNZ		0X71
#define		JE		0X72
#define		JNE		0X73
#define		JL		0X74
#define		JLE		0X75
#define		JG		0X76
#define		JGE		0X77
#define		JMP		0X78

#define		LEA		0X8D
#define		NOP		0X90
#define		CALL	0X9A

#define		MOV		0XA0
#define		MOVL	0xA1

#define		INT		0XCC
#define		PUTC	0xD0
#define		GETC	0xD1

#define		EXIT	0xE0