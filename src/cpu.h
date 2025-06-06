// cpu.h - Daniel Molina - BSD-3-Clause (see LICENSE)
//
// extends
//
// cpu.h - Johan Smet - BSD-3-Clause (see LICENSE)
//
// Common 'base' for cpu implementations

#ifndef DROMAIUS_CPU_H
#define DROMAIUS_CPU_H

#include "chip.h"

typedef void (*CPU_OVERRIDE_NEXT_INSTRUCTION_ADDRESS)(void *cpu, uint16_t addr);
typedef bool (*CPU_IS_AT_START_OF_INSTRUCTION)(void *cpu);
typedef bool (*CPU_IRQ_IS_ASSERTED)(void *cpu);
typedef int64_t  (*CPU_PROGRAM_COUNTER)(void *cpu);

// Function to retrieve model number
typedef uint32_t (*CPU_MODEL_NUMBER)(void *cpu);

// Get current cycle
typedef uint32_t (*CPU_GET_CYCLES)(void *cpu);

#define CPU_DECLARE_FUNCTIONS		\
	CHIP_DECLARE_BASE														\
	CPU_OVERRIDE_NEXT_INSTRUCTION_ADDRESS override_next_instruction_address;	\
	CPU_IS_AT_START_OF_INSTRUCTION is_at_start_of_instruction;					\
	CPU_IRQ_IS_ASSERTED irq_is_asserted;										\
	CPU_PROGRAM_COUNTER program_counter;                                        \
	CPU_MODEL_NUMBER model_number;												\
	CPU_GET_CYCLES get_cycles;

typedef struct Cpu {
	CPU_DECLARE_FUNCTIONS
} Cpu;

#endif // DROMAIUS_CPU_H
