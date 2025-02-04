// cpu_65816.h - Daniel - BSD-3-Clause (see LICENSE)
// Based on
// cpu_6502.h - Johan Smet - BSD-3-Clause (see LICENSE)
//
// Emulation of the MOS 6502

#ifndef DROMAIUS_CPU_65816_H
#define DROMAIUS_CPU_65816_H

#include "cpu.h"
#include "signal_line.h"

#ifdef __cplusplus
extern "C" {
#endif

// types
enum Cpu65816SignalAssignments {
	PIN_65816_RDY   = CHIP_PIN_02,		// ready signal - cpu only runs when asserted
	PIN_65816_PHI1O = CHIP_PIN_03,		// phase-1 clock output (NOT IMPLEMENTED)
	PIN_65816_IRQ_B = CHIP_PIN_04,		// interrupt request line
	PIN_65816_NMI_B = CHIP_PIN_06,		// non-maskable interrupt line
	PIN_65816_SYNC  = CHIP_PIN_07,		// indicates opcode fetch cycle (output)
	PIN_65816_AB0   = CHIP_PIN_09,
	PIN_65816_AB1   = CHIP_PIN_10,
	PIN_65816_AB2   = CHIP_PIN_11,
	PIN_65816_AB3   = CHIP_PIN_12,
	PIN_65816_AB4   = CHIP_PIN_13,
	PIN_65816_AB5   = CHIP_PIN_14,
	PIN_65816_AB6   = CHIP_PIN_15,
	PIN_65816_AB7   = CHIP_PIN_16,
	PIN_65816_AB8   = CHIP_PIN_17,
	PIN_65816_AB9   = CHIP_PIN_18,
	PIN_65816_AB10  = CHIP_PIN_19,
	PIN_65816_AB11  = CHIP_PIN_20,
	PIN_65816_AB12  = CHIP_PIN_22,
	PIN_65816_AB13  = CHIP_PIN_23,
	PIN_65816_AB14  = CHIP_PIN_24,
	PIN_65816_AB15  = CHIP_PIN_25,
	PIN_65816_DB7   = CHIP_PIN_26,
	PIN_65816_DB6   = CHIP_PIN_27,
	PIN_65816_DB5   = CHIP_PIN_28,
	PIN_65816_DB4   = CHIP_PIN_29,
	PIN_65816_DB3   = CHIP_PIN_30,
	PIN_65816_DB2   = CHIP_PIN_31,
	PIN_65816_DB1   = CHIP_PIN_32,
	PIN_65816_DB0   = CHIP_PIN_33,
	PIN_65816_RW	   = CHIP_PIN_34,		// read-write line (high == reading, low == writing)
	PIN_65816_CLK   = CHIP_PIN_37,		// phase-2 clock/system clock input
	PIN_65816_SO	   = CHIP_PIN_38,		// set overflow flag (NOT IMPLEMENTED)
	PIN_65816_PHI2O = CHIP_PIN_39,		// phase-1 clock output (NOT IMPLEMENTED)
	PIN_65816_RES_B = CHIP_PIN_40		// reset line (input)
};

#define CHIP_65816_PIN_COUNT 40
typedef Signal Cpu65816Signals[CHIP_65816_PIN_COUNT];

typedef enum Cpu65816Flags {
	FLAG_65816_CARRY				= 0b00000001,
	FLAG_65816_ZERO_RESULT		= 0b00000010,
	FLAG_65816_INTERRUPT_DISABLE = 0b00000100,
	FLAG_65816_DECIMAL_MODE		= 0b00001000,
	FLAG_65816_BREAK_COMMAND		= 0b00010000,
	FLAG_65816_EXPANSION			= 0b00100000,
	FLAG_65816_OVERFLOW			= 0b01000000,
	FLAG_65816_NEGATIVE_RESULT	= 0b10000000,

	// abbreviations
	FLAG_65816_C = FLAG_65816_CARRY,
	FLAG_65816_Z = FLAG_65816_ZERO_RESULT,
	FLAG_65816_I = FLAG_65816_INTERRUPT_DISABLE,
	FLAG_65816_D = FLAG_65816_DECIMAL_MODE,
	FLAG_65816_B = FLAG_65816_BREAK_COMMAND,
	FLAG_65816_E = FLAG_65816_EXPANSION,
	FLAG_65816_V = FLAG_65816_OVERFLOW,
	FLAG_65816_N = FLAG_65816_NEGATIVE_RESULT
} Cpu65816Flags;

typedef struct Cpu65816 {

	CPU_DECLARE_FUNCTIONS

	// interface
	SignalPool *	signal_pool;
	Cpu65816Signals	signals;

	SignalGroup		sg_address;
	SignalGroup		sg_data;

	// registers
	uint8_t		reg_a;				// accumulator
	uint8_t		reg_x;				// x-index
	uint8_t		reg_y;				// y-index
	uint8_t		reg_sp;				// stack-pointer
	uint8_t		reg_ir;				// instruction register
	uint16_t	reg_pc;				// program counter
	uint8_t		reg_p;				// processor status register
} Cpu65816;

// functions
Cpu65816 *cpu_65816_create(struct Simulator *sim, Cpu65816Signals signals);

#ifdef __cplusplus
}
#endif

#endif // DROMAIUS_CPU_65816_H
