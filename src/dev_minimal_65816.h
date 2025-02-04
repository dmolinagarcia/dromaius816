// dev_minimal_65816.h - Johan Smet - BSD-3-Clause (see LICENSE)
// 
// dev_minimal_6502.h - Johan Smet - BSD-3-Clause (see LICENSE)
//
// Emulates a minimal MOS-65816 based system, with 32kb of RAM and a 16kb system ROM.

/* Address space:
   0x0000 - 0x7fff (32kb) : RAM
   0x8000 - 0x800f (16)   : PIA (6520 - Peripheral Interface Adapter)
		0x8000 = DDRA/Port-A	(Data Direction Register for port-A / port-A)
		0x8001 = CRA			(Control Register for port-A)
		0x8002 = DDRB/Port-B	(Data Direction Register for port-B / port-B)
		0x8003 = CRB			(Control Register for port-B)
   0x8010 - 0xbfff        : (free)
   0xc000 - 0xffff (16kb) : ROM
*/

#ifndef DROMAIUS_DEV_MINIMAL_65816_H
#define DROMAIUS_DEV_MINIMAL_65816_H

#include "device.h"
#include "signal_line.h"

#ifdef __cplusplus
extern "C" {
#endif

// types
enum DevMinimal65816SignalAssignment {

	// 16-bit address bus
	SIG_M65816_AB0 = 0,
	SIG_M65816_AB1,
	SIG_M65816_AB2,
	SIG_M65816_AB3,
	SIG_M65816_AB4,
	SIG_M65816_AB5,
	SIG_M65816_AB6,
	SIG_M65816_AB7,
	SIG_M65816_AB8,
	SIG_M65816_AB9,
	SIG_M65816_AB10,
	SIG_M65816_AB11,
	SIG_M65816_AB12,
	SIG_M65816_AB13,
	SIG_M65816_AB14,
	SIG_M65816_AB15,

	// 8-bit data bus
	SIG_M65816_DB0,
	SIG_M65816_DB1,
	SIG_M65816_DB2,
	SIG_M65816_DB3,
	SIG_M65816_DB4,
	SIG_M65816_DB5,
	SIG_M65816_DB6,
	SIG_M65816_DB7,

	SIG_M65816_RESET_BTN_B,

	SIG_M65816_RESET_B,
	SIG_M65816_CLOCK,

	SIG_M65816_CPU_RW,
	SIG_M65816_CPU_IRQ_B,
	SIG_M65816_CPU_NMI_B,
	SIG_M65816_CPU_SYNC,
	SIG_M65816_CPU_RDY,

	SIG_M65816_RAM_OE_B,
	SIG_M65816_RAM_WE_B,

	SIG_M65816_ROM_CE_B,

	SIG_M65816_PIA_CS2_B,

	SIG_M65816_LOW,
	SIG_M65816_HIGH,

	SIG_M65816_SIGNAL_COUNT
};

typedef Signal DevMinimal65816Signals[SIG_M65816_SIGNAL_COUNT];

typedef struct DevMinimal65816 {
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
	DevMinimal65816Signals	signals;

	SignalGroup				sg_address;
	SignalGroup				sg_data;
} DevMinimal65816;

// functions
DevMinimal65816 *dev_minimal_65816_create(const uint8_t *rom_data);
void dev_minimal_65816_destroy(DevMinimal65816 *device);
void dev_minimal_65816_reset(DevMinimal65816 *device);
void dev_minimal_65816_read_memory(DevMinimal65816 *device, size_t start_address, size_t size, uint8_t *output);
void dev_minimal_65816_write_memory(DevMinimal65816 *device, size_t start_address, size_t size, uint8_t *input);

void dev_minimal_65816_rom_from_file(DevMinimal65816 *device, const char *filename);
void dev_minimal_65816_ram_from_file(DevMinimal65816 *device, const char *filename);

#ifdef __cplusplus
}
#endif

#endif // DROMAIUS_DEV_MINIMAL_65816_H
