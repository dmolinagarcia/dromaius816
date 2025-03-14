// signal_history_profiles.c - Daniel Molina - BSD-3-Clause (see LICENSE)
//
// extends
//
// signal_history_profiles.c - Johan Smet - BSD-3-Clause (see LICENSE)

#include "signal_history_profiles.h"
#include "signal_history.h"

#include "dev_commodore_pet.h"
#include "dev_minimal_65816.h"

#include "cpu_6502.h"
//> COUT_DMG #include "chip_6520.h"
//> COUT_DMG #include "chip_6522.h"
//> COUT_DMG #include "chip_rom.h"
//> COUT_DMG 
//> COUT_DMG #include "perif_datassette_1530.h"
//> COUT_DMG #include "perif_disk_2031.h"

void dev_minimal_65816_history_profiles(struct DevMinimal65816 *device, const char *chip_name, struct SignalHistory *history) {

	uint32_t prof_data = signal_history_profile_create(history, chip_name, "Data Bus");
	for (size_t i = 0; i < arrlenu(device->sg_data); ++i) {
		signal_history_profile_add_signal(history, prof_data, *device->sg_data[i], NULL);
	}

	uint32_t prof_address = signal_history_profile_create(history, chip_name, "Address Bus");
	for (size_t i = 0; i < arrlenu(device->sg_address); ++i) {
		signal_history_profile_add_signal(history, prof_address, *device->sg_address[i], NULL);
	}

	uint32_t prof_ram = signal_history_profile_create(history, chip_name, "RAM");
	signal_history_profile_add_signal(history, prof_ram, device->signals[SIG_M65816_RAM_CE_B], NULL);
	signal_history_profile_add_signal(history, prof_ram, device->signals[SIG_M65816_RAM_OE_B], NULL);
	signal_history_profile_add_signal(history, prof_ram, device->signals[SIG_M65816_RAM_WE_B], NULL);

}

void dev_commodore_pet_history_profiles(struct DevCommodorePet *pet, const char *chip_name, struct SignalHistory *history) {

	uint32_t prof_data = signal_history_profile_create(history, chip_name, "Data Bus");
	for (size_t i = 0; i < arrlenu(pet->sg_cpu_data); ++i) {
		signal_history_profile_add_signal(history, prof_data, *pet->sg_cpu_data[i], NULL);
	}

	uint32_t prof_address = signal_history_profile_create(history, chip_name, "Address Bus");
	for (size_t i = 0; i < arrlenu(pet->sg_cpu_address); ++i) {
		signal_history_profile_add_signal(history, prof_address, *pet->sg_cpu_address[i], NULL);
	}

	uint32_t prof_video = signal_history_profile_create(history, chip_name, "Video");
	signal_history_profile_add_signal(history, prof_video, pet->signals[SIG_P2001N_HORZ_DISP_ON], NULL);
	signal_history_profile_add_signal(history, prof_video, pet->signals[SIG_P2001N_HORZ_DISP_OFF], NULL);
	signal_history_profile_add_signal(history, prof_video, pet->signals[SIG_P2001N_HORZ_DRIVE], NULL);
	signal_history_profile_add_signal(history, prof_video, pet->signals[SIG_P2001N_VIDEO_LATCH], NULL);
	signal_history_profile_add_signal(history, prof_video, pet->signals[SIG_P2001N_VERT_DRIVE], NULL);
	signal_history_profile_add_signal(history, prof_video, pet->signals[SIG_P2001N_VIDEO_ON], NULL);
	signal_history_profile_add_signal(history, prof_video, pet->signals[SIG_P2001N_VIDEO], NULL);

	uint32_t prof_phases = signal_history_profile_create(history, chip_name, "Clk1 Phases");
	signal_history_profile_add_signal(history, prof_phases, pet->signals[SIG_P2001N_BPHI2A], NULL);
	signal_history_profile_add_signal(history, prof_phases, pet->signals[SIG_P2001N_BPHI2B], NULL);
	signal_history_profile_add_signal(history, prof_phases, pet->signals[SIG_P2001N_BPHI2C], NULL);
	signal_history_profile_add_signal(history, prof_phases, pet->signals[SIG_P2001N_BPHI2D], NULL);
	signal_history_profile_add_signal(history, prof_phases, pet->signals[SIG_P2001N_BPHI2E], NULL);
	signal_history_profile_add_signal(history, prof_phases, pet->signals[SIG_P2001N_BPHI2F], NULL);
	signal_history_profile_add_signal(history, prof_phases, pet->signals[SIG_P2001N_BPHI2G], NULL);
	signal_history_profile_add_signal(history, prof_phases, pet->signals[SIG_P2001N_BPHI2H], NULL);
}

void cpu_6502_signal_history_profiles(struct Cpu6502 *cpu, const char *chip_name, struct SignalHistory *history) {

	uint32_t prof_ctrl = signal_history_profile_create(history, chip_name, "Control Signals");
	signal_history_profile_add_signal(history, prof_ctrl, cpu->signals[PIN_6502_RES_B], "/RES");
	signal_history_profile_add_signal(history, prof_ctrl, cpu->signals[PIN_6502_CLK],	"PHI0");
	signal_history_profile_add_signal(history, prof_ctrl, cpu->signals[PIN_6502_RDY],	"RDY");
	signal_history_profile_add_signal(history, prof_ctrl, cpu->signals[PIN_6502_SYNC],	"SYNC");
	signal_history_profile_add_signal(history, prof_ctrl, cpu->signals[PIN_6502_RW],	"RW");
	signal_history_profile_add_signal(history, prof_ctrl, cpu->signals[PIN_6502_IRQ_B],	"/IRQ");
	signal_history_profile_add_signal(history, prof_ctrl, cpu->signals[PIN_6502_NMI_B], "/NMI");
}

//> COUT_DMG void chip_6520_signal_history_profiles(struct Chip6520 *pia, const char *chip_name, struct SignalHistory *history) {
//> COUT_DMG 
//> COUT_DMG 	uint32_t prof_ctrl = signal_history_profile_create(history, chip_name, "Control Signals");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_ctrl, pia->signals[CHIP_6520_RESET_B],	"/RESET");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_ctrl, pia->signals[CHIP_6520_PHI2],		"PHI2");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_ctrl, pia->signals[CHIP_6520_CA1],		"CA1");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_ctrl, pia->signals[CHIP_6520_CA2],		"CA2");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_ctrl, pia->signals[CHIP_6520_CB1],		"CB1");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_ctrl, pia->signals[CHIP_6520_CB2],		"CB2");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_ctrl, pia->signals[CHIP_6520_IRQA_B],	"/IRQA");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_ctrl, pia->signals[CHIP_6520_IRQB_B],	"/IRQB");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_ctrl, pia->signals[CHIP_6520_RW],		"RW");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_ctrl, pia->signals[CHIP_6520_RS0],		"RS0");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_ctrl, pia->signals[CHIP_6520_RS1],		"RS1");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_ctrl, pia->signals[CHIP_6520_CS0],		"CS0");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_ctrl, pia->signals[CHIP_6520_CS1],		"CS1");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_ctrl, pia->signals[CHIP_6520_CS2_B],	"/CS2");
//> COUT_DMG 
//> COUT_DMG 	uint32_t prof_porta = signal_history_profile_create(history, chip_name, "Port-A");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_porta, pia->signals[CHIP_6520_PA0],		"PA0");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_porta, pia->signals[CHIP_6520_PA1],		"PA1");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_porta, pia->signals[CHIP_6520_PA2],		"PA2");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_porta, pia->signals[CHIP_6520_PA3],		"PA3");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_porta, pia->signals[CHIP_6520_PA4],		"PA4");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_porta, pia->signals[CHIP_6520_PA5],		"PA5");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_porta, pia->signals[CHIP_6520_PA6],		"PA6");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_porta, pia->signals[CHIP_6520_PA7],		"PA7");
//> COUT_DMG 
//> COUT_DMG 	uint32_t prof_portb = signal_history_profile_create(history, chip_name, "Port-B");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_portb, pia->signals[CHIP_6520_PB0],		"PB0");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_portb, pia->signals[CHIP_6520_PB1],		"PB1");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_portb, pia->signals[CHIP_6520_PB2],		"PB2");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_portb, pia->signals[CHIP_6520_PB3],		"PB3");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_portb, pia->signals[CHIP_6520_PB4],		"PB4");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_portb, pia->signals[CHIP_6520_PB5],		"PB5");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_portb, pia->signals[CHIP_6520_PB6],		"PB6");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_portb, pia->signals[CHIP_6520_PB7],		"PB7");
//> COUT_DMG }
//> COUT_DMG 
//> COUT_DMG void chip_6522_signal_history_profiles(struct Chip6522 *via, const char *chip_name, struct SignalHistory *history) {
//> COUT_DMG 
//> COUT_DMG 	uint32_t prof_ctrl = signal_history_profile_create(history, chip_name, "Control Signals");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_ctrl, via->signals[CHIP_6522_RESET_B],	"/RESET");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_ctrl, via->signals[CHIP_6522_PHI2],		"PHI2");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_ctrl, via->signals[CHIP_6522_CA1],		"CA1");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_ctrl, via->signals[CHIP_6522_CA2],		"CA2");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_ctrl, via->signals[CHIP_6522_CB1],		"CB1");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_ctrl, via->signals[CHIP_6522_CB2],		"CB2");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_ctrl, via->signals[CHIP_6522_IRQ_B],	"/IRQ");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_ctrl, via->signals[CHIP_6522_RW],		"RW");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_ctrl, via->signals[CHIP_6522_RS0],		"RS0");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_ctrl, via->signals[CHIP_6522_RS1],		"RS1");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_ctrl, via->signals[CHIP_6522_RS2],		"RS2");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_ctrl, via->signals[CHIP_6522_RS3],		"RS3");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_ctrl, via->signals[CHIP_6522_CS1],		"CS1");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_ctrl, via->signals[CHIP_6522_CS2_B],	"/CS2");
//> COUT_DMG 
//> COUT_DMG 	uint32_t prof_porta = signal_history_profile_create(history, chip_name, "Port-A");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_porta, via->signals[CHIP_6522_PA0],		"PA0");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_porta, via->signals[CHIP_6522_PA1],		"PA1");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_porta, via->signals[CHIP_6522_PA2],		"PA2");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_porta, via->signals[CHIP_6522_PA3],		"PA3");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_porta, via->signals[CHIP_6522_PA4],		"PA4");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_porta, via->signals[CHIP_6522_PA5],		"PA5");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_porta, via->signals[CHIP_6522_PA6],		"PA6");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_porta, via->signals[CHIP_6522_PA7],		"PA7");
//> COUT_DMG 
//> COUT_DMG 	uint32_t prof_portb = signal_history_profile_create(history, chip_name, "Port-B");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_portb, via->signals[CHIP_6522_PB0],		"PB0");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_portb, via->signals[CHIP_6522_PB1],		"PB1");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_portb, via->signals[CHIP_6522_PB2],		"PB2");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_portb, via->signals[CHIP_6522_PB3],		"PB3");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_portb, via->signals[CHIP_6522_PB4],		"PB4");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_portb, via->signals[CHIP_6522_PB5],		"PB5");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_portb, via->signals[CHIP_6522_PB6],		"PB6");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_portb, via->signals[CHIP_6522_PB7],		"PB7");
//> COUT_DMG }
//> COUT_DMG 
//> COUT_DMG void chip_63xx_signal_history_profiles(struct Chip63xxRom *rom, const char *chip_name, struct SignalHistory *history) {
//> COUT_DMG 
//> COUT_DMG 	uint32_t prof_ctrl = signal_history_profile_create(history, chip_name, "Control Signals");
//> COUT_DMG 	if (rom->data_size == ROM_6316_DATA_SIZE) {
//> COUT_DMG 		signal_history_profile_add_signal(history, prof_ctrl, rom->signals[CHIP_6316_CS1_B],	"/CS1");
//> COUT_DMG 		signal_history_profile_add_signal(history, prof_ctrl, rom->signals[CHIP_6316_CS2_B],	"/CS2");
//> COUT_DMG 		signal_history_profile_add_signal(history, prof_ctrl, rom->signals[CHIP_6316_CS3],		"CS3");
//> COUT_DMG 	} else {
//> COUT_DMG 		signal_history_profile_add_signal(history, prof_ctrl, rom->signals[CHIP_6332_CS1_B],	"/CS1");
//> COUT_DMG 		signal_history_profile_add_signal(history, prof_ctrl, rom->signals[CHIP_6332_CS3],		"CS3");
//> COUT_DMG 	}
//> COUT_DMG }
//> COUT_DMG
//> COUT_DMG void perif_datassette_signal_history_profiles(struct PerifDatassette *datassette, const char *chip_name, struct SignalHistory *history) {
//> COUT_DMG 
//> COUT_DMG 	uint32_t prof_ctrl = signal_history_profile_create(history, chip_name, "Signals");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_ctrl, datassette->signals[PIN_DS1530_MOTOR],		"MOTOR");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_ctrl, datassette->signals[PIN_DS1530_SENSE],		"SENSE");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_ctrl, datassette->signals[PIN_DS1530_DATA_FROM_DS],	"TO-DS");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_ctrl, datassette->signals[PIN_DS1530_DATA_TO_DS],	"FROM-DS");
//> COUT_DMG }
//> COUT_DMG 
//> COUT_DMG void perif_disk2031_signal_history_profiles(struct PerifDisk2031 *fd2031, const char *chip_name, struct SignalHistory *history) {
//> COUT_DMG 
//> COUT_DMG 	uint32_t prof_ctrl = signal_history_profile_create(history, chip_name, "Signals");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_ctrl, fd2031->signals[PERIF_FD2031_EOI_B],	"/EOI");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_ctrl, fd2031->signals[PERIF_FD2031_DAV_B],	"/DAV");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_ctrl, fd2031->signals[PERIF_FD2031_NRFD_B],	"/NRFD");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_ctrl, fd2031->signals[PERIF_FD2031_NDAC_B],	"/NDAC");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_ctrl, fd2031->signals[PERIF_FD2031_ATN_B],	"/ATN");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_ctrl, fd2031->signals[PERIF_FD2031_SRQ_B],	"/SRQ");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_ctrl, fd2031->signals[PERIF_FD2031_IFC_B],	"/IFC");
//> COUT_DMG 
//> COUT_DMG 	uint32_t prof_io = signal_history_profile_create(history, chip_name, "I/O");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_io, fd2031->signals[PERIF_FD2031_DIO0],	"DIO0");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_io, fd2031->signals[PERIF_FD2031_DIO1],	"DIO1");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_io, fd2031->signals[PERIF_FD2031_DIO2],	"DIO2");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_io, fd2031->signals[PERIF_FD2031_DIO3],	"DIO3");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_io, fd2031->signals[PERIF_FD2031_DIO4],	"DIO4");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_io, fd2031->signals[PERIF_FD2031_DIO5],	"DIO5");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_io, fd2031->signals[PERIF_FD2031_DIO6],	"DIO6");
//> COUT_DMG 	signal_history_profile_add_signal(history, prof_io, fd2031->signals[PERIF_FD2031_DIO7],	"DIO7");
//> COUT_DMG }
