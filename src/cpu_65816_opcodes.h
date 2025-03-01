// cpu_65816_opcodes.h - Daniel Molina - BSD-3-Clause (see LICENCE)
// based on
// cpu_6502_opcodes.h - Johan Smet - BSD-3-Clause (see LICENSE)
//
// list all support 65816 opcodes

#ifndef DROMAIUS_CPU_65816_OPCODES_H
#define DROMAIUS_CPU_65816_OPCODES_H

#include "types.h"
// opcodes
typedef enum OPCODES_65816_ {
	OP_65816_BRK			= 0x00,
	OP_65816_NOP			= 0xea,
	OP_65816_XCE			= 0xfb
} OPCODES_65816;

/*
// opcodes
typedef enum OPCODES_65816_ {

	OP_65816_ADC_IMM		= 0x69,
	OP_65816_ADC_ZP		= 0x65,
	OP_65816_ADC_ZPX		= 0x75,
	OP_65816_ADC_ABS		= 0x6d,
	OP_65816_ADC_ABSX	= 0x7d,
	OP_65816_ADC_ABSY	= 0x79,
	OP_65816_ADC_INDX	= 0x61,
	OP_65816_ADC_INDY	= 0x71,

	OP_65816_AND_IMM		= 0x29,
	OP_65816_AND_ZP		= 0x25,
	OP_65816_AND_ZPX		= 0x35,
	OP_65816_AND_ABS		= 0x2d,
	OP_65816_AND_ABSX	= 0x3d,
	OP_65816_AND_ABSY	= 0x39,
	OP_65816_AND_INDX	= 0x21,
	OP_65816_AND_INDY	= 0x31,

	OP_65816_ASL_ACC		= 0x0a,
	OP_65816_ASL_ZP		= 0x06,
	OP_65816_ASL_ZPX		= 0x16,
	OP_65816_ASL_ABS		= 0x0e,
	OP_65816_ASL_ABSX	= 0x1e,
	
	OP_65816_BCC			= 0x90,		// branch on carry clear (C = 0)
	OP_65816_BCS			= 0xb0,		// branch on carry set (C = 1)
	OP_65816_BIT_ZP		= 0x24,		
	OP_65816_BIT_ABS		= 0x2c,
	OP_65816_BEQ			= 0xf0,		// branch on result zero (Z = 1)
	
	OP_65816_BMI			= 0x30,		// branch on result minus (N = 1)
	OP_65816_BNE			= 0xd0,		// branch on result not zero (Z = 0)
	OP_65816_BPL			= 0x10,		// branch on result plus (N = 0)
	OP_65816_BVC			= 0x50,		// branch on overflow clear (V = 0)
	OP_65816_BVS			= 0x70,		// branch on overflow set (V = 1)

	OP_65816_CLC			= 0x18,		// clear carry  flag
	OP_65816_CLD			= 0xd8,		// clear decimal mode
	OP_65816_CLI			= 0x58,		// clear interrupt disable bit
	OP_65816_CLV			= 0xb8,		// clear overflow flag

	OP_65816_CMP_IMM		= 0xc9,
	OP_65816_CMP_ZP		= 0xc5,
	OP_65816_CMP_ZPX		= 0xd5,
	OP_65816_CMP_ABS		= 0xcd,
	OP_65816_CMP_ABSX	= 0xdd,
	OP_65816_CMP_ABSY	= 0xd9,
	OP_65816_CMP_INDX	= 0xc1,
	OP_65816_CMP_INDY	= 0xd1,

	OP_65816_CPX_IMM		= 0xe0,
	OP_65816_CPX_ZP		= 0xe4,
	OP_65816_CPX_ABS		= 0xec,

	OP_65816_CPY_IMM		= 0xc0,
	OP_65816_CPY_ZP		= 0xc4,
	OP_65816_CPY_ABS		= 0xcc,

	OP_65816_DEC_ZP		= 0xc6,
	OP_65816_DEC_ZPX		= 0xd6,
	OP_65816_DEC_ABS		= 0xce,
	OP_65816_DEC_ABSX	= 0xde,

	OP_65816_DEX			= 0xca,
	OP_65816_DEY			= 0x88,

	OP_65816_EOR_IMM		= 0x49,
	OP_65816_EOR_ZP		= 0x45,
	OP_65816_EOR_ZPX		= 0x55,
	OP_65816_EOR_ABS		= 0x4d,
	OP_65816_EOR_ABSX	= 0x5d,
	OP_65816_EOR_ABSY	= 0x59,
	OP_65816_EOR_INDX	= 0x41,
	OP_65816_EOR_INDY	= 0x51,

	OP_65816_INC_ZP		= 0xe6,
	OP_65816_INC_ZPX		= 0xf6,
	OP_65816_INC_ABS		= 0xee,
	OP_65816_INC_ABSX	= 0xfe,

	OP_65816_INX			= 0xe8,
	OP_65816_INY			= 0xc8,

	OP_65816_JMP_ABS		= 0x4c,
	OP_65816_JMP_IND		= 0x6c,

	OP_65816_JSR			= 0x20,

	OP_65816_LDA_IMM		= 0xa9,
	OP_65816_LDA_ZP		= 0xa5,
	OP_65816_LDA_ZPX		= 0xb5,
	OP_65816_LDA_ABS		= 0xad,
	OP_65816_LDA_ABSX	= 0xbd,
	OP_65816_LDA_ABSY	= 0xb9,
	OP_65816_LDA_INDX	= 0xa1,
	OP_65816_LDA_INDY	= 0xb1,

	OP_65816_LDX_IMM		= 0xa2,
	OP_65816_LDX_ZP		= 0xa6,
	OP_65816_LDX_ZPY		= 0xb6,
	OP_65816_LDX_ABS		= 0xae,
	OP_65816_LDX_ABSY	= 0xbe,

	OP_65816_LDY_IMM		= 0xa0,
	OP_65816_LDY_ZP		= 0xa4,
	OP_65816_LDY_ZPX		= 0xb4,
	OP_65816_LDY_ABS		= 0xac,
	OP_65816_LDY_ABSX	= 0xbc,

	OP_65816_LSR_ACC		= 0x4a,
	OP_65816_LSR_ZP		= 0x46,
	OP_65816_LSR_ZPX		= 0x56,
	OP_65816_LSR_ABS		= 0x4e,
	OP_65816_LSR_ABSX	= 0x5e,


	OP_65816_ORA_IMM		= 0x09,
	OP_65816_ORA_ZP		= 0x05,
	OP_65816_ORA_ZPX		= 0x15,
	OP_65816_ORA_ABS		= 0x0d,
	OP_65816_ORA_ABSX	= 0x1d,
	OP_65816_ORA_ABSY	= 0x19,
	OP_65816_ORA_INDX	= 0x01,
	OP_65816_ORA_INDY	= 0x11,

	OP_65816_PHA			= 0x48,		// push accumulator on stack
	OP_65816_PHP			= 0x08,		// push processor status on stack
	OP_65816_PLA			= 0x68,		// pull accumulator from stack
	OP_65816_PLP			= 0x28,		// pull processor status from stack

	OP_65816_ROL_ACC		= 0x2a,
	OP_65816_ROL_ZP		= 0x26,
	OP_65816_ROL_ZPX		= 0x36,
	OP_65816_ROL_ABS		= 0x2e,
	OP_65816_ROL_ABSX	= 0x3e,

	OP_65816_ROR_ACC		= 0x6a,
	OP_65816_ROR_ZP		= 0x66,
	OP_65816_ROR_ZPX		= 0x76,
	OP_65816_ROR_ABS		= 0x6e,
	OP_65816_ROR_ABSX	= 0x7e,

	OP_65816_RTI			= 0x40,
	OP_65816_RTS			= 0x60,

	OP_65816_SBC_IMM		= 0xe9,
	OP_65816_SBC_ZP		= 0xe5,
	OP_65816_SBC_ZPX		= 0xf5,
	OP_65816_SBC_ABS		= 0xed,
	OP_65816_SBC_ABSX	= 0xfd,
	OP_65816_SBC_ABSY	= 0xf9,
	OP_65816_SBC_INDX	= 0xe1,
	OP_65816_SBC_INDY	= 0xf1,

	OP_65816_SEC			= 0x38,		// set cary flag
	OP_65816_SED			= 0xf8,		// set decimal mode
	OP_65816_SEI			= 0x78,		// set interrupt disable status

	OP_65816_STA_ZP		= 0x85,
	OP_65816_STA_ZPX		= 0x95,
	OP_65816_STA_ABS		= 0x8D,
	OP_65816_STA_ABSX	= 0x9D,
	OP_65816_STA_ABSY	= 0x99,
	OP_65816_STA_INDX	= 0x81,
	OP_65816_STA_INDY	= 0x91,

	OP_65816_STX_ZP		= 0x86,
	OP_65816_STX_ZPY		= 0x96,
	OP_65816_STX_ABS		= 0x8e,

	OP_65816_STY_ZP		= 0x84,
	OP_65816_STY_ZPX		= 0x94,
	OP_65816_STY_ABS		= 0x8c,

	OP_65816_TAX			= 0xaa,
	OP_65816_TAY			= 0xa8,
	OP_65816_TSX			= 0xba,
	OP_65816_TXA			= 0x8a,
	OP_65816_TXS			= 0x9a,
	OP_65816_TYA			= 0x98,
} OPCODES_65816;

  The 65816 instruction set is laid out according to a pattern "aaabbbcc".
   Typically a and c denote the instruction type and b specifies the addressing mode.

//                      aaabbbcc
#define AC_65816_MASK  0b11100011
#define AC_65816_ADC	  0b01100001
#define AC_65816_AND	  0b00100001
#define AC_65816_ASL	  0b00000010
#define AC_65816_CMP	  0b11000001
#define AC_65816_DEC   0b11000010
#define AC_65816_EOR	  0b01000001
#define AC_65816_INC   0b11100010
#define AC_65816_LDA   0b10100001
#define AC_65816_LDX   0b10100010
#define AC_65816_LDY   0b10100000
#define AC_65816_LSR	  0b01000010
#define AC_65816_ORA	  0b00000001
#define AC_65816_ROL	  0b00100010
#define AC_65816_ROR	  0b01100010
#define AC_65816_SBC	  0b11100001
#define AC_65816_STA	  0b10000001

//                          aaabbbcc
#define ADDR_65816_MASK    0b00011100

#define EXTRACT_65816_ADRESSING_MODE(op)	\
	(((op) & ADDR_65816_MASK) >> 2)

typedef enum ADDRESSING_MODE_65816_ {
	
	// the values of these first 8 modes correspond to the bbb-part of the opcodes for the "Group 1 instructions"
	AM_65816_INDIRECT_X	= 0b0000,
	AM_65816_ZEROPAGE	= 0b0001,
	AM_65816_IMMEDIATE	= 0b0010,
	AM_65816_ABSOLUTE	= 0b0011,
	AM_65816_INDIRECT_Y	= 0b0100,
	AM_65816_ZEROPAGE_X	= 0b0101,
	AM_65816_ABSOLUTE_Y	= 0b0110,
	AM_65816_ABSOLUTE_X	= 0b0111,

	// other addressing modes not used for group-1 instructions
	AM_65816_ZEROPAGE_Y	= 0b1000,
	AM_65816_INDIRECT	= 0b1001,

	AM_65816_UNDEFINED	= 0b1111

} ADDRESSING_MODE_65816;

*/


#endif // DROMAIUS_CPU_65816_OPCODES_H
