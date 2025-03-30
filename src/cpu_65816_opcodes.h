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
	OP_65816_RTI			= 0x40,
	OP_65816_JMP_ABSO       = 0x4c,
	OP_65816_LDY_IMME       = 0xA0,
	OP_65816_LDX_IMME       = 0xA2,
	OP_65816_LDA_IMME       = 0xA9,
	OP_65816_REP            = 0xc2,
	OP_65816_SEP            = 0xe2,
	OP_65816_NOP			= 0xea,
	OP_65816_XCE			= 0xfb
} OPCODES_65816;

// addressing modes list

typedef enum ADDR_MODES_65816_ {
	____ = 0,                     //> ____, Placeholder                 //            // Non-existant. Just a placeholder
	abso    ,                     //> Absolute   		                //    a       // 3.5.1  in WDC DataSheet
	impl    ,                     //> Implied                           //    i       // 3.5.19 in WDC DataSheet
	imme    ,                     //> Immediate                         //    #       // 3.5.18 in WDC DataSheet  
																					  // 2 bytes or 3 depending on MX....
																					  // Up to the opcode decoder to choose
//  JAII    ,                     //> Absolute Indexed Indirect Jump    //    (a,x)   // 3.5.2  in WDC DataSheet
//	ABIX    ,                     //> Absolute Indexed with X           //    a,x     // 3.5.3  in WDC DataSheet
//  ABIY    ,                     //> Absolute Indexed with Y           //    a,y     // 3.5.4  in WDC DataSheet	
//	ABSI    ,                     //> Absolute Indirect                 //    (a)     // 3.5.5  in WDC DataSheet
//	ALIX    ,                     //> Absolute Long Indexes with X      //    al,x    // 3.5.6  in WDC DataSheet
//	ABSL    ,                     //> Absolute Long                     //    al      // 3.5.7  in WDC DataSheet
//	ACCU    ,                     //> Accumulator                       //    A       // 3.5.8  in WDC DataSheet
//	BLKM    ,                     //> Block Move                        //    xyc     // 3.5.9  in WDC DataSheet
//	DIIX    ,                     //> Direct Indexed Indirect           //    (d,x)   // 3.5.10 in WDC DataSheet
//	DINX    ,                     //> Direct Indexed with X             //    d,x     // 3.5.11 in WDC DataSheet
//	DINY    ,                     //> Direct Indexed with Y             //    d,y     // 3.5.12 in WDC DataSheet
//	DIIN    ,                     //> Direct Indirect Indexed           //    (d),y   // 3.5.13 in WDC DataSheet
//	DILI    ,                     //> Direct Indirect Long Indexed      //    [d],y   // 3.5.14 in WDC DataSheet
//	DILO    ,                     //> Direct Indirect Long              //    [d]     // 3.5.15 in WDC DataSheet
//	DIRI    ,                     //> Direct Indirect                   //    (d)     // 3.5.16 in WDC DataSheet
//	DIRE    ,                     //> Direct                            //    d       // 3.5.17 in WDC DataSheet
//	PCRL    ,                     //> Program Counter Relative Long     //    rl      // 3.5.20 in WDC DataSheet
//	PCRE    ,                     //> Program Counter Relative          //    r       // 3.5.21 in WDC DataSheet
//	STCK    ,                     //> Stack                             //    s       // 3.5.22 in WDC DataSheet
//	SREL    ,                     //> Stack Relative                    //    d,s     // 3.5.23 in WDC DataSheet
//	SRII    ,                     //> Stack Relative Indirect Indexed   //    (d,s),y // 3.5.24 in WDC DataSheet
} ADDR_MODES_65816;

static const ADDR_MODES_65816 ADDRESS_MODES_MATRIX_65816[256] = {
	//    0        1      2       3       4      5       6       7       8       9       A       B      C       D        E        F
		impl,   ____,   ____,   ____,   impl,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,     // 0
		____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,     // 1
		____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,     // 2
		____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,     // 3
		____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   abso,   ____,   ____,   ____,     // 4
		____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,     // 5
		____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,     // 6
		____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,     // 7
		____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,     // 8
		____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,     // 9
		imme,   ____,   imme,   ____,   ____,   ____,   ____,   ____,   ____,   imme,   ____,   ____,   ____,   ____,   ____,   ____,     // a
		____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,     // b
		____,   ____,   imme,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,     // c
		____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,     // D
		____,   ____,   imme,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   impl,   ____,   ____,   ____,   ____,   ____,     // E
		____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   ____,   impl,   ____,   ____,   ____,   ____      // F
	};
	


#endif // DROMAIUS_CPU_65816_OPCODES_H