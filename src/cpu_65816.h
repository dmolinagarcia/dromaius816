// cpu_65816.h - Daniel Molina - BSD-3-Clause (see LICENSE)
// Based on
// cpu_6502.h - Johan Smet - BSD-3-Clause (see LICENSE)
//
// Emulation of the MOS 6502

// WDC DataSheet: https://www.westerndesigncenter.com/wdc/documentation/w65c816s.pdf

#ifndef DROMAIUS_CPU_65816_H
#define DROMAIUS_CPU_65816_H

#include "cpu.h"
#include "signal_line.h"

#ifdef __cplusplus
extern "C" {
#endif

// types
enum Cpu65816SignalAssignments {
    PIN_65816_VPB   = CHIP_PIN_01, 
	PIN_65816_RDY   = CHIP_PIN_02,
	PIN_65816_ABORT = CHIP_PIN_03,
	PIN_65816_IRQ_B = CHIP_PIN_04,
	PIN_65816_MLB   = CHIP_PIN_05,
	PIN_65816_NMI_B = CHIP_PIN_06,
	PIN_65816_VPA   = CHIP_PIN_07,
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
	PIN_65816_RWB	= CHIP_PIN_34,
	PIN_65816_E     = CHIP_PIN_35,
	PIN_65816_BE    = CHIP_PIN_36,
	PIN_65816_PHI2  = CHIP_PIN_37,
	PIN_65816_MX	= CHIP_PIN_38,
	PIN_65816_VDA   = CHIP_PIN_39,
	PIN_65816_RES_B = CHIP_PIN_40
};

#define CHIP_65816_PIN_COUNT 40
typedef Signal Cpu65816Signals[CHIP_65816_PIN_COUNT];


//>TODO Flags need to be adapted to the 65c816
typedef enum Cpu65816Flags {
	FLAG_65816_CARRY			 = ((uint32_t) 0b00000000100000001),
	FLAG_65816_ZERO_RESULT		 = ((uint32_t) 0b00000001000000010),
	FLAG_65816_INTERRUPT_DISABLE = ((uint32_t) 0b00000010000000100),
	FLAG_65816_DECIMAL_MODE		 = ((uint32_t) 0b00000100000001000),
	FLAG_65816_BREAK_COMMAND	 = ((uint32_t) 0b00000000000010000),
	FLAG_65816_INDEX             = ((uint32_t) 0b00001000000000000),
	FLAG_65816_UNUSED			 = ((uint32_t) 0b00000000000100000),
	FLAG_65816_MEMORY            = ((uint32_t) 0b00010000000000000),
	FLAG_65816_OVERFLOW			 = ((uint32_t) 0b00100000001000000),
	FLAG_65816_NEGATIVE_RESULT	 = ((uint32_t) 0b01000000010000000),
	FLAG_65816_EMULATION         = ((uint32_t) 0b10000000000000000),

	// abbreviations
	FLAG_65816_C = FLAG_65816_CARRY,
	FLAG_65816_Z = FLAG_65816_ZERO_RESULT,
	FLAG_65816_I = FLAG_65816_INTERRUPT_DISABLE,
	FLAG_65816_D = FLAG_65816_DECIMAL_MODE,
	FLAG_65816_B = FLAG_65816_BREAK_COMMAND,
	FLAG_65816_U = FLAG_65816_UNUSED,
	FLAG_65816_V = FLAG_65816_OVERFLOW,
	FLAG_65816_N = FLAG_65816_NEGATIVE_RESULT,
	FLAG_65816_M = FLAG_65816_MEMORY,
	FLAG_65816_X = FLAG_65816_INDEX,
	FLAG_65816_E = FLAG_65816_EMULATION
} Cpu65816Flags;



typedef struct Cpu65816 {

	CPU_DECLARE_FUNCTIONS

	// interface
	SignalPool *	signal_pool;
	Cpu65816Signals	signals;

	SignalGroup		sg_address;
	SignalGroup		sg_data;

	// registers
    uint8_t     reg_dbr;			// Data Bank Register
    uint8_t     reg_pbr;			// Program Bank Register
	uint16_t	reg_a;				// accumulator
	uint16_t	reg_x;				// x-index
	uint16_t	reg_y;				// y-index
	uint16_t	reg_sp;				// stack-register
	uint8_t		reg_ir;				// instruction register
	uint16_t	reg_pc;				// program counter
	uint32_t	reg_p;				// processor status register
} Cpu65816;

// functions
Cpu65816 *cpu_65816_create(struct Simulator *sim, Cpu65816Signals signals);

#ifdef __cplusplus
}
#endif

#endif // DROMAIUS_CPU_65816_H
