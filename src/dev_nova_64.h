// dev_nova_64.h - Daniel Molina - BSD-3-Clause (see LICENSE)
// extending
// dev_minimal_65816.h - Daniel Molina - BSD-3-Clause (see LICENSE)
// based on
// dev_minimal_6502.h - Johan Smet - BSD-3-Clause (see LICENSE)
//
// Emulates a minimal 65816 based system

/* Address space:
   // TBD
*/

#ifndef DROMAIUS_DEV_NOVA_64_H
#define DROMAIUS_DEV_NOVA_64_H

#include "device.h"
#include "signal_line.h"

#ifdef __cplusplus
extern "C" {
#endif

// types
enum DevNova64SignalAssignment {

	// 16-bit address bus
	SIG_NOVA64_AB0 = 0,
	SIG_NOVA64_AB1,
	SIG_NOVA64_AB2,
	SIG_NOVA64_AB3,
	SIG_NOVA64_AB4,
	SIG_NOVA64_AB5,
	SIG_NOVA64_AB6,
	SIG_NOVA64_AB7,
	SIG_NOVA64_AB8,
	SIG_NOVA64_AB9,
	SIG_NOVA64_AB10,
	SIG_NOVA64_AB11,
	SIG_NOVA64_AB12,
	SIG_NOVA64_AB13,
	SIG_NOVA64_AB14,
	SIG_NOVA64_AB15,

	// 8-bit data bus
	SIG_NOVA64_DB0,
	SIG_NOVA64_DB1,
	SIG_NOVA64_DB2,
	SIG_NOVA64_DB3,
	SIG_NOVA64_DB4,
	SIG_NOVA64_DB5,
	SIG_NOVA64_DB6,
	SIG_NOVA64_DB7,

	SIG_NOVA64_RESET_BTN_B,

	SIG_NOVA64_RESET_B,
	SIG_NOVA64_CLOCK,

	SIG_NOVA64_CPU_RW,
	SIG_NOVA64_CPU_IRQ_B,
	SIG_NOVA64_CPU_NMI_B,
	SIG_NOVA64_CPU_SYNC,
	SIG_NOVA64_CPU_RDY,

	SIG_NOVA64_RAM_OE_B,
	SIG_NOVA64_RAM_WE_B,

	SIG_NOVA64_ROM_CE_B,

	SIG_NOVA64_PIA_CS2_B,

	SIG_NOVA64_LOW,
	SIG_NOVA64_HIGH,

	SIG_NOVA64_SIGNAL_COUNT
};

typedef Signal DevNova64Signals[SIG_NOVA64_SIGNAL_COUNT];

typedef struct DevNova64 {
	DEVICE_DECLARE_FUNCTIONS

	// components
	struct Cpu65816 *	cpu;
	struct Ram8d16a *	ram;
	struct Rom8d16a *	rom;
	struct Chip6520 *	pia;
	struct ChipHd44780 *lcd;
	struct InputKeypad *keypad;
	struct Oscillator *	oscillator;

	bool			in_reset;

	// signals
	SignalPool *			signal_pool;
	DevNova64Signals	signals;

	SignalGroup				sg_address;
	SignalGroup				sg_data;
} DevNova64;

// functions
DevNova64 *dev_nova_64_create(const uint8_t *rom_data);
void dev_nova_64_destroy(DevNova64 *device);
void dev_nova_64_reset(DevNova64 *device);
void dev_nova_64_read_memory(DevNova64 *device, size_t start_address, size_t size, uint8_t *output);
void dev_nova_64_write_memory(DevNova64 *device, size_t start_address, size_t size, uint8_t *input);

void dev_nova_64_rom_from_file(DevNova64 *device, const char *filename);
void dev_nova_64_ram_from_file(DevNova64 *device, const char *filename);

#ifdef __cplusplus
}
#endif

#endif // DROMAIUS_DEV_NOVA_64_H
