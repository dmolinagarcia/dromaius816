// filter_65816_asm.c - Daniel Molina - BSD-3-Clause (see LICENSE)
// based on
// filter_6502_asm.c - Johan Smet - BSD-3-Clause (see LICENSE)
//
// Dissambler for 65816 assembly

#include "filt_65816_asm.h"

#include "stb/stb_ds.h"
#include "cpu_65816_opcodes.h"
#include "utils.h"

#include <assert.h>

///////////////////////////////////////////////////////////////////////////////
//
// lookup tables
//

// opcode => string
// Updated for 65816
static const char OPCODE_NAMES[256][4] = {
//	 00     01     02     03     04     05     06     07     08     09     0A     0B     0C     0D     0E     0F
	"BRK", "ORA", "COP", "ORA", "TSB", "ORA", "ASL", "ORA", "PHP", "ORA", "ASL", "PHD", "TSB", "ORA", "ASL", "ORA",
	"BPL", "ORA", "ORA", "ORA", "TRB", "ORA", "ASL", "ORA", "CLC", "ORA", "INC", "TCS", "TRB", "ORA", "ASL", "ORA",
	"JSR", "AND", "JSR", "AND", "BIT", "AND", "ROL", "AND", "PLP", "AND", "ROL", "PLD", "BIT", "AND", "ROL", "AND",
	"BMI", "AND", "AND", "AND", "BIT", "AND", "ROL", "AND", "SEC", "AND", "DEC", "TSC", "BIT", "AND", "ROL", "AND",
	"RTI", "EOR", "WDM", "EOR", "MVP", "EOR", "LSR", "EOR", "PHA", "EOR", "LSR", "PHK", "JMP", "EOR", "LSR", "EOR",
	"BVC", "EOR", "EOR", "EOR", "MVN", "EOR", "LSR", "EOR", "CLI", "EOR", "PHY", "TCD", "JMP", "EOR", "LSR", "EOR",
	"RTS", "ADC", "PER", "ADC", "STZ", "ADC", "ROR", "ADC", "PLA", "ADC", "ROR", "RTL", "JMP", "ADC", "ROR", "ADC",
	"BVS", "ADC", "ADC", "ADC", "STZ", "ADC", "ROR", "ADC", "SEI", "ADC", "PLY", "TDC", "JMP", "ADC", "ROR", "ADC",
	"BRA", "STA", "BRL", "STA", "STY", "STA", "STX", "STA", "DEY", "BIT", "TXA", "PHB", "STY", "STA", "STX", "STA",
	"BCC", "STA", "STA", "STA", "STY", "STA", "STX", "STA", "TYA", "STA", "TXS", "TXY", "STZ", "STA", "STZ", "STA",
	"LDY", "LDA", "LDX", "LDA", "LDY", "LDA", "LDX", "LDA", "TAY", "LDA", "TAX", "PLB", "LDY", "LDA", "LDX", "LDA",
	"BCS", "LDA", "LDA", "LDA", "LDY", "LDA", "LDX", "LDA", "CLV", "LDA", "TSX", "TYX", "LDY", "LDA", "LDX", "LDA",
	"CPY", "CMP", "REP", "CMP", "CPY", "CMP", "DEC", "CMP", "INY", "CMP", "DEX", "WAI", "CPY", "CMP", "DEC", "CMP",
	"BNE", "CMP", "CMP", "CMP", "PEI", "CMP", "DEC", "CMP", "CLD", "CMP", "PHX", "STP", "JML", "CMP", "DEC", "CMP",
	"CPX", "SBC", "SEP", "SBC", "CPX", "SBC", "INC", "SBC", "INX", "SBC", "NOP", "XBA", "CPX", "SBC", "INC", "SBC",
	"BEQ", "SBC", "SBC", "SBC", "PEA", "SBC", "INC", "SBC", "SED", "SBC", "PLX", "XCE", "JSR", "SBC", "INC", "SBC"
};

// addressing modes
//>TODO Update for 65816
typedef enum ADDR_MODE {
	NONE = 0,                     //> NONE, Placeholder     //      
	ABSA    ,                     //> Absolute   		    //    a       // 3.5.1  in WDC DataSheet
	IMPL    ,                     //> Implied               //    i       // 3.5.19 in WDC DataSheet

	                              //>         REMAINDER FROM 6502 IMM,
	                              //>         REMAINDER FROM 6502 REL,
	                              //>         REMAINDER FROM 6502 ZP,
	                              //>         REMAINDER FROM 6502 ZPX,
	                              //>         REMAINDER FROM 6502 ZPY,
	                              //>         REMAINDER FROM 6502 ABS,
	                              //>         REMAINDER FROM 6502 ABSX,
	                              //>         REMAINDER FROM 6502 ABSY,
	                              //>         REMAINDER FROM 6502 IND,
	                              //>         REMAINDER FROM 6502 XIND,
	                              //>         REMAINDER FROM 6502 INDY
} ADDR_MODE;

// opcode => addressing mode
//>TODO Update for 65816
static const ADDR_MODE OPCODE_ADDRESS_MODES[256] = {
//	IMPL,	XIND,	NONE,	NONE,	NONE,	ZP,		ZP,		NONE,	IMPL,	IMM,	ACC,	NONE,	NONE,	ABS,	ABS,	NONE,
                                                                                                    ABSA,   ABSA,   ABSA,             // 0
//	REL,	INDY,	NONE,	NONE,	NONE,	ZPX,	ZPX,	NONE,	IMPL,	ABSY,	NONE,	NONE,	NONE,	ABSX,	ABSX,	NONE,
                                                                    IMPL,                   IMPL,   ABSA,                             // 1
//	ABS,	XIND,	NONE,	NONE,	ZP,		ZP,		ZP,		NONE,	IMPL,	IMM,	ACC,	NONE,	ABS,	ABS,	ABS,	NONE,
    ABSA,                                                                                           ABSA,   ABSA,   ABSA,             // 2
//	REL,	INDY,	NONE,	NONE,	NONE,	ZPX,	ZPX,	NONE,	IMPL,	ABSY,	NONE,	NONE,	NONE,	ABSX,	ABSX,	NONE,
                                                                    IMPL,                                                             // 3
//	IMPL,	XIND,	NONE,	NONE,	NONE,	ZP,		ZP,		NONE,	IMPL,	IMM,	ACC,	NONE,	ABS,	ABS,	ABS,	NONE,     
                                                                                                    ABSA,   ABSA,   ABSA,             // 4
//	REL,	INDY,	NONE,	NONE,	NONE,	ZPX,	ZPX,	NONE,	IMPL,	ABSY,	NONE,	NONE,	NONE,	ABSX,	ABSX,	NONE,
                                                                    IMPL,                   IMPL,                                     // 5
//	IMPL,	XIND,	NONE,	NONE,	NONE,	ZP,		ZP,		NONE,	IMPL,	IMM,	ACC,	NONE,	IND,	ABS,	ABS,	NONE,
                                                                                                            ABSA,   ABSA,             // 6
//	REL,	INDY,	NONE,	NONE,	NONE,	ZPX,	ZPX,	NONE,	IMPL,	ABSY,	NONE,	NONE,	NONE,	ABSX,	ABSX,	NONE,
                                                                    IMPL,                   IMPL,                                     // 7
//	NONE,	XIND,	NONE,	NONE,	ZP,		ZP,		ZP,		NONE,	IMPL,	NONE,	IMPL,	NONE,	ABS,	ABS,	ABS,	NONE,
                                                                    IMPL,                           ABSA,   ABSA,   ABSA,             // 8
//	REL,	INDY,	NONE,	NONE,	ZPX,	ZPX,	ZPY,	NONE,	IMPL,	ABSY,	IMPL,	NONE,	NONE,	ABSX,	NONE,	NONE,
                                                                                                    ABSA,                             // 9
//	IMM,	XIND,	IMM,	NONE,	ZP,		ZP,		ZP,		NONE,	IMPL,	IMM,	IMPL,	NONE,	ABS,	ABS,	ABS,	NONE,     
                                                                    IMPL,           IMPL,           ABSA,   ABSA,   ABSA,             // a
//	REL,	INDY,	NONE,	NONE,	ZPX,	ZPX,	ZPY,	NONE,	IMPL,	ABSY,	IMPL,	NONE,	ABSX,	ABSX,	ABSY,	NONE,
                                                                    IMPL,                                                             // b
//	IMM,	XIND,	NONE,	NONE,	ZP,		ZP,		ZP,		NONE,	IMPL,	IMM,	IMPL,	NONE,	ABS,	ABS,	ABS,	NONE,
                                                                    IMPL,           IMPL,           ABSA,   ABSA,   ABSA,             // c
//	REL,	INDY,	NONE,	NONE,	NONE,	ZPX,	ZPX,	NONE,	IMPL,	ABSY,	NONE,	NONE,	NONE,	ABSX,	ABSX,	NONE,
                                                                    IMPL,                   IMPL,                                     // D
//	IMM,	XIND,	NONE,	NONE,	ZP,		ZP,		ZP,		NONE,	IMPL,	IMM,	IMPL,	NONE,	ABS,	ABS,	ABS,	NONE,     
                                                                    IMPL,           IMPL,           ABSA,   ABSA,   ABSA,             // E
//	REL,	INDY,	NONE,	NONE,	NONE,	ZPX,	ZPX,	NONE,	IMPL,	ABSY,	NONE,	NONE,	NONE,	ABSX,	ABSX,	NONE
                                                                    IMPL,                                                             // F
};

// addressing mode => number of arguments
//> TODO Modify based on MX,E
static const unsigned int ADDRESS_MODE_LEN[14] = {
	0, // NONE 
	2, // ABSA
	0  // IMPL
	//>    6502 REMAINING, 0,	// ACC
	//>    6502 REMAINING, 1,	// IMM
	//>    6502 REMAINING, 1,	// REL
	//>    6502 REMAINING, 1,	// ZP
	//>    6502 REMAINING, 1,	// ZPX
	//>    6502 REMAINING, 1,	// ZPY
	//>    6502 REMAINING, 2,	// ABSX
	//>    6502 REMAINING, 2,	// ABSY
	//>    6502 REMAINING, 2,	// IND
	//>    6502 REMAINING, 1,	// XIND
	//>    6502 REMAINING, 1	// INDY
};//>    6502 REMAINING, 

///////////////////////////////////////////////////////////////////////////////
//
// interface functions
//

size_t filt_65816_asm_line(const uint8_t *binary, size_t bin_size, size_t bin_index, size_t bin_offset, char **line) {
	assert(binary);
	assert(bin_index < bin_size);

	uint8_t op = binary[bin_index];

	// address + opcode
	arr_printf(*line, "%.4x: %s", bin_index + bin_offset, OPCODE_NAMES[op]);

	// argument
	unsigned arg_size = ADDRESS_MODE_LEN[OPCODE_ADDRESS_MODES[op]];

	if (bin_index + arg_size >= bin_size) {
		arr_printf(*line, " (opcode incomplete)");
		return 1 + arg_size;
	}

	switch (OPCODE_ADDRESS_MODES[op]) {
		case ABSA:
			arr_printf(*line, " $%.2x%.2x", binary[bin_index+2], binary[bin_index+1]);
			break;
//>		case IMM:	
//>			arr_printf(*line, " #$%.2x", binary[bin_index+1]);
//>			break;
//>		case REL:
//>		case ZP:
//>			arr_printf(*line, " $%.2x", binary[bin_index+1]);
//>			break;
//>		case ZPX:
//>			arr_printf(*line, " $%.2x,x", binary[bin_index+1]);
//>			break;
//>		case ZPY:
//>			arr_printf(*line, " $%.2x,y", binary[bin_index+1]);
//>			break;
//>		case ABSX:
//>			arr_printf(*line, " $%.2x%.2x,x", binary[bin_index+2], binary[bin_index+1]);
//>			break;
//>		case ABSY:
//>			arr_printf(*line, " $%.2x%.2x,y", binary[bin_index+2], binary[bin_index+1]);
//>			break;
//>		case IND:
//>			arr_printf(*line, " ($%.2x%.2x)", binary[bin_index+2], binary[bin_index+1]);
//>			break;
//>		case XIND:
//>			arr_printf(*line, " ($%.2x,x)", binary[bin_index+1]);
//>			break;
//>		case INDY:
//>			arr_printf(*line, " ($%.2x),y", binary[bin_index+1]);
//>			break;
		default:
			break;
	}

	return 1 + arg_size;
}

size_t filt_65816_asm_count_instruction(const uint8_t *binary, size_t bin_size, size_t from, size_t until) {
	assert(binary);
	assert(from < bin_size);
	assert(until <= bin_size);

	if (until > bin_size) {
		until = bin_size;
	}

	size_t count = 0;

	for (size_t i = from; i < until; i += 1) {
		unsigned arg_size = ADDRESS_MODE_LEN[OPCODE_ADDRESS_MODES[binary[i]]];
		i += arg_size;
		++count;
	}

	return count;
}

uint8_t filt_65816_asm_instruction_length(uint8_t opcode, uint32_t reg_p) {

	return 1;
}

const char* filt_65816_get_opcode(uint8_t opcode) {
    return OPCODE_NAMES[opcode];
}
