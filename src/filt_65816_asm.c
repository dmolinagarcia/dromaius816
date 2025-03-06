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

#define b3  binary[bin_index+3]
#define b2  binary[bin_index+2]
#define b1  binary[bin_index+1]
#define b0  binary[bin_index+0]

// Calculate relative offsets for branch instructions
#define GET_ADDR_8(pc, offset) ((size_t)(((ptrdiff_t)(pc)) + 2 + (int8_t)(offset)))
#define GET_ADDR_16(pc, b1, b2) \
    ((uint16_t)((((ptrdiff_t)(pc)) + 2 + (int16_t)(((uint16_t)(b2) << 8) | ((uint16_t)(b1)))) & 0xFFFF))



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
	NONE = 0,                     //> NONE, Placeholder                 //            // Non-existant. Just a placeholder
	ABSA    ,                     //> Absolute   		                //    a       // 3.5.1  in WDC DataSheet
    JAII    ,                     //> Absolute Indexed Indirect Jump    //    (a,x)   // 3.5.2  in WDC DataSheet
	ABIX    ,                     //> Absolute Indexed with X           //    a,x     // 3.5.3  in WDC DataSheet
    ABIY    ,                     //> Absolute Indexed with Y           //    a,y     // 3.5.4  in WDC DataSheet	
	ABSI    ,                     //> Absolute Indirect                 //    (a)     // 3.5.5  in WDC DataSheet
	ALIX    ,                     //> Absolute Long Indexes with X      //    al,x    // 3.5.6  in WDC DataSheet
	ABSL    ,                     //> Absolute Long                     //    al      // 3.5.7  in WDC DataSheet
	ACCU    ,                     //> Accumulator                       //    A       // 3.5.8  in WDC DataSheet
	BLKM    ,                     //> Block Move                        //    xyc     // 3.5.9  in WDC DataSheet
	DIIX    ,                     //> Direct Indexed Indirect           //    (d,x)   // 3.5.10 in WDC DataSheet
	DINX    ,                     //> Direct Indexed with X             //    d,x     // 3.5.11 in WDC DataSheet
	DINY    ,                     //> Direct Indexed with Y             //    d,y     // 3.5.12 in WDC DataSheet
	DIIN    ,                     //> Direct Indirect Indexed           //    (d),y   // 3.5.13 in WDC DataSheet
	DILI    ,                     //> Direct Indirect Long Indexed      //    [d],y   // 3.5.14 in WDC DataSheet
	DILO    ,                     //> Direct Indirect Long              //    [d]     // 3.5.15 in WDC DataSheet
	DIRI    ,                     //> Direct Indirect                   //    (d)     // 3.5.16 in WDC DataSheet
	DIRE    ,                     //> Direct                            //    d       // 3.5.17 in WDC DataSheet
	IMME    ,                     //> Immediate                         //    #       // 3.5.18 in WDC DataSheet  2 bytes or 3 depending on MX....
	                                                                                  //                          0000 1001 3 bytes if M = 0 else 2
																					  //						  0010 1001
																					  //                          0100 1001
																					  //                          0110 1001
																					  //                          1000 1001
																					  //                          1010 1001
																					  //                          1100 1001
																					  //                          1110 1001
	                                                                                  //                          1010 0000 3 bytes if X = 0 else 2
																					  //                          1010 0010
																					  //                          1100 0000
																					  //                          1110 0000
																					  //                          1100 0010 2 bytes always
																					  //                          1110 0010
	IMPL    ,                     //> Implied                           //    i       // 3.5.19 in WDC DataSheet
	PCRL    ,                     //> Program Counter Relative Long     //    rl      // 3.5.20 in WDC DataSheet
	PCRE    ,                     //> Program Counter Relative          //    r       // 3.5.21 in WDC DataSheet
	STCK    ,                     //> Stack                             //    s       // 3.5.22 in WDC DataSheet
	SREL    ,                     //> Stack Relative                    //    d,s     // 3.5.23 in WDC DataSheet
	SRII    ,                     //> Stack Relative Indirect Indexed   //    (d,s),y // 3.5.24 in WDC DataSheet
} ADDR_MODE;

// opcode => addressing mode
//>TODO Update for 65816
static const ADDR_MODE OPCODE_ADDRESS_MODES[256] = {
//    0        1      2       3       4      5       6       7       8       9       A       B      C       D        E        F
    STCK,   DIIX,   STCK,   SREL,   DIRE,   DIRE,   DIRE,   DILO,   STCK,   IMME,   ACCU,   STCK,   ABSA,   ABSA,   ABSA,   ABSL,     // 0
    PCRE,   DIIN,   DIRI,   SRII,   DIRE,   DINX,   DINX,   DILI,   IMPL,   ABIY,   ACCU,   IMPL,   ABSA,   ABIX,   ABIX,   ALIX,     // 1
    ABSA,   DIIX,   ABSL,   SREL,   DIRE,   DIRE,   DIRE,   DILO,   STCK,   IMME,   ACCU,   STCK,   ABSA,   ABSA,   ABSA,   ABSL,     // 2
    PCRE,   DIIN,   DIRI,   SRII,   DINX,   DINX,   DINX,   DILI,   IMPL,   ABIY,   ACCU,   IMPL,   ABIX,   ABIX,   ABIX,   ALIX,     // 3
    STCK,   DIIX,   IMME,   SREL,   BLKM,   DIRE,   DIRE,   DILO,   STCK,   IMME,   ACCU,   STCK,   ABSA,   ABSA,   ABSA,   ABSL,     // 4
    PCRE,   DIIN,   DIRI,   SRII,   BLKM,   DINX,   DINX,   DILI,   IMPL,   ABIY,   STCK,   IMPL,   ABSL,   ABIX,   ABIX,   ALIX,     // 5
    STCK,   DIIX,   ABSA,   SREL,   DIRE,   DIRE,   DIRE,   DILO,   STCK,   IMME,   ACCU,   STCK,   ABSI,   ABSA,   ABSA,   ABSL,     // 6
    PCRE,   DIIN,   DIRI,   SRII,   DINX,   DINX,   DINX,   DILI,   IMPL,   ABIY,   STCK,   IMPL,   JAII,   ABIX,   ABIX,   ALIX,     // 7
    PCRE,   DIIX,   PCRL,   SREL,   DIRE,   DIRE,   DIRE,   DILO,   IMPL,   IMME,   IMPL,   STCK,   ABSA,   ABSA,   ABSA,   ABSL,     // 8
    PCRE,   DIIN,   DIRI,   SRII,   DINX,   DINX,   DINY,   DILI,   IMPL,   ABIY,   IMPL,   IMPL,   ABSA,   ABIX,   ABIX,   ALIX,     // 9
    IMME,   DIIX,   IMME,   SREL,   DIRE,   DIRE,   DIRE,   DILO,   IMPL,   IMME,   IMPL,   STCK,   ABSA,   ABSA,   ABSA,   ABSL,     // a
    PCRE,   DIIN,   DIRI,   SRII,   DINX,   DINX,   DINY,   DILI,   IMPL,   ABIY,   IMPL,   IMPL,   ABIX,   ABIX,   ABIY,   ALIX,     // b
    IMME,   DIIX,   IMME,   SREL,   DIRE,   DIRE,   DIRE,   DILO,   IMPL,   IMME,   IMPL,   IMPL,   ABSA,   ABSA,   ABSA,   ABSL,     // c
    PCRE,   DIIN,   DIRI,   SRII,   DIRE,   DINX,   DINX,   DILI,   IMPL,   ABIY,   STCK,   IMPL,   ABSI,   ABIX,   ABIX,   ALIX,     // D
    IMME,   DIIX,   IMME,   SREL,   DIRE,   DIRE,   DIRE,   DILO,   IMPL,   IMME,   IMPL,   IMPL,   ABSA,   ABSA,   ABSA,   ABSL,     // E
    PCRE,   DIIN,   DIRI,   SRII,   IMME,   DINX,   DINX,   DILI,   IMPL,   ABIY,   STCK,   IMPL,   JAII,   ABIX,   ABIX,   ALIX      // F
};

// addressing mode => number of arguments
//> TODO Modify based on MX,E
static const unsigned int ADDRESS_MODE_LEN[25] = {
	0, // NONE   0
	2, // ABSA   1
	2, // JAII   2
	2, // ABIX   3
	2, // ABIY   4
	2, // ABSI   5
	3, // ALIX   6
	3, // ABSL   7
	0, // ACCU   8
	2, // BLKM   9
	1, // DIIX   10
	1, // DINX   11
	1, // DINY   12
	1, // DIIN   13
	1, // DILI   14
	1, // DILO   15
	1, // DIRI   16
	1, // DIRE   17
	1, // IMME   18
	0, // IMPL   19
	2, // PCRL   20
	1, // PCRE   21
	0, // STCK   22
	1, // SREL   23
	1  // SRII   24
};

///////////////////////////////////////////////////////////////////////////////
//
// interface functions
//

size_t filt_65816_asm_line(const uint8_t *binary, size_t bin_size, size_t bin_index, size_t bin_offset, char **line, bool m, bool x) {
	assert(binary);
	assert(bin_index < bin_size);

	uint8_t op = binary[bin_index];

	// argument
	unsigned arg_size = ADDRESS_MODE_LEN[OPCODE_ADDRESS_MODES[op]];

		//>TODO Test this!
	if (OPCODE_ADDRESS_MODES[op] == IMME ) {
		// Addresing mode is Immediate. We have to choose between if operand is 8 or 16 bits
		if ((op & 0x0F) == 0x09) 
			arg_size = m ? 1 : 2; 					// 09 29 49 69 89 A9 C9 E9
		if ((op & 0x0F) == 0x00 || op == 0xA2) 
			arg_size = x ? 1 : 2;					// A0 C0 E0 A2
		if (op == 0xF4) 
			arg_size = 2;							// F4 PER
		// 42 wdm, c2 rep AND e2 sep always 2 bytes
	}

	if (bin_index + arg_size >= bin_size) {
		arr_printf(*line, " (opcode incomplete)");
		return 1 + arg_size;
	}

	switch (OPCODE_ADDRESS_MODES[op]) {
		case ABSA: arr_printf(*line,   "%.4x: %.2x %.2x %.2x    %s $%.2x%.2x",			bin_index + bin_offset, b0, b1, b2    , OPCODE_NAMES[op],	b2, b1      );			break;
		case JAII: arr_printf(*line,   "%.4x: %.2x %.2x %.2x    %s ($%.2x%.2x,x)",		bin_index + bin_offset, b0, b1, b2    ,	OPCODE_NAMES[op],	b2, b1      ); 			break;
		case ABIX: arr_printf(*line,   "%.4x: %.2x %.2x %.2x    %s $%.2x%.2x,x", 	 	bin_index + bin_offset, b0, b1, b2    ,	OPCODE_NAMES[op], 	b2, b1      );			break;
		case ABIY: arr_printf(*line,   "%.4x: %.2x %.2x %.2x    %s $%.2x%.2x,y", 	 	bin_index + bin_offset, b0, b1, b2    ,	OPCODE_NAMES[op],	b2, b1      );			break;
		case ABSI: arr_printf(*line,   "%.4x: %.2x %.2x %.2x    %s ($%.2x%.2x)", 	 	bin_index + bin_offset, b0, b1, b2    ,	OPCODE_NAMES[op], 	b2, b1      ); 			break;
		case ALIX: arr_printf(*line, "%.4x: %.2x %.2x %.2x %.2x %s $%.2x%.2x%.2x,x", 	bin_index + bin_offset, b0, b1, b2, b3, OPCODE_NAMES[op], 	b3, b2, b1	);			break;
		case ABSL: arr_printf(*line, "%.4x: %.2x %.2x %.2x %.2x %s $%.2x%.2x%.2x", 		bin_index + bin_offset, b0, b1, b2, b3,	OPCODE_NAMES[op], 	b3, b2, b1	);	 		break;
		case BLKM: arr_printf(*line,   "%.4x: %.2x %.2x %.2x    %s $%.2x,%.2x", 	 	bin_index + bin_offset, b0, b1, b2    ,	OPCODE_NAMES[op], 	b2, b1      ); 			break;
		case DIIX: arr_printf(*line,     "%.4x: %.2x %.2x       %s ($%.2x,x)", 			bin_index + bin_offset, b0, b1        ,	OPCODE_NAMES[op], 	b1          );			break;
		case DINX: arr_printf(*line,     "%.4x: %.2x %.2x       %s $%.2x,x",        	bin_index + bin_offset, b0, b1        , OPCODE_NAMES[op],   b1			);			break;
		case DINY: arr_printf(*line,     "%.4x: %.2x %.2x       %s $%.2x,Y",         	bin_index + bin_offset, b0, b1        , OPCODE_NAMES[op],   b1			);			break;
		case DIIN: arr_printf(*line,     "%.4x: %.2x %.2x       %s ($%.2x),Y",       	bin_index + bin_offset, b0, b1        , OPCODE_NAMES[op],   b1			);			break;
		case DILI: arr_printf(*line,     "%.4x: %.2x %.2x       %s [$%.2x],Y",       	bin_index + bin_offset, b0, b1        , OPCODE_NAMES[op],   b1			);			break;
		case DILO: arr_printf(*line,     "%.4x: %.2x %.2x       %s [$%.2x]",	       	bin_index + bin_offset, b0, b1        , OPCODE_NAMES[op],   b1			);			break;
		case DIRI: arr_printf(*line,     "%.4x: %.2x %.2x       %s ($%.2x)",	       	bin_index + bin_offset, b0, b1        , OPCODE_NAMES[op],   b1			);			break;
		case DIRE: arr_printf(*line,     "%.4x: %.2x %.2x       %s $%.2x",	       		bin_index + bin_offset, b0, b1        , OPCODE_NAMES[op],   b1			);			break;
		case IMME: 
			switch (arg_size) {		
				case 1: arr_printf(*line,     "%.4x: %.2x %.2x       %s #$%.2x",	    bin_index + bin_offset, b0, b1        , OPCODE_NAMES[op],   b1			);			break;
				case 2: arr_printf(*line,   "%.4x: %.2x %.2x %.2x    %s #$%.2x%.2x",	bin_index + bin_offset, b0, b1, b2    , OPCODE_NAMES[op],   b2, b1		);			break;
				default: break;
			}
			break;
		case PCRL: arr_printf(*line,   "%.4x: %.2x %.2x %.2x    %s $%.4x",  bin_index + bin_offset, b0, b1, b2    ,	OPCODE_NAMES[op], 	GET_ADDR_16(bin_index + bin_offset, b1, b2 )     ); 			break;
		case PCRE: arr_printf(*line,     "%.4x: %.2x %.2x       %s $%.4x",	bin_index + bin_offset, b0, b1        , OPCODE_NAMES[op],   GET_ADDR_8(bin_index + bin_offset, b1)		);			break;
		case SREL: arr_printf(*line,     "%.4x: %.2x %.2x       %s $%.2x,s",	       	bin_index + bin_offset, b0, b1        , OPCODE_NAMES[op],   b1			);			break;
		case SRII: arr_printf(*line,     "%.4x: %.2x %.2x       %s ($%.2x,s),Y",       	bin_index + bin_offset, b0, b1        , OPCODE_NAMES[op],   b1			);			break;
		default:   arr_printf(*line,       "%.4x: %.2x          %s ",			       	bin_index + bin_offset, b0,             OPCODE_NAMES[op]                );			break;
	}

	//>todo rel INDICATES IT IS A BRANCH WITH RELATIVE ADDRSING. I HAVE TO CALCULATE THIS
	//>TODO rel is bin_index + bin_offset + 2 + int8_t (offset which is b1)
	//>  A394   D0 21    BNE $A3b7

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

//>TODO What about variable length instructions?

const char* filt_65816_get_opcode(uint8_t opcode) {
    return OPCODE_NAMES[opcode];
}
