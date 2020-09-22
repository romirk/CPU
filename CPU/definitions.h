#pragma once

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

// OPCODES
#define		ADDL	0X00
#define		ADDR	0X01
#define		PUSH	0X06
#define		POP		0X07
#define		OR		0X08
#define		NOT		0X10
#define		AND		0X20
#define		SUBL	0X28
#define		SUBR	0X29
#define		XOR		0X30

#define		INC		0X40
#define		DEC		0X48
#define		MULL	0X50
#define		MULR	0X51
#define		DIVL	0X60
#define		DIVR	0X61

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

#define		MOV_RR	0XA0
#define		MOV_LR	0XA1

#define		INT		0XCC
#define		PUTC	0xD0
#define		GETC	0xD1

#define		EXIT	0xE0
