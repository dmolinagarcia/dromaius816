// dev_commodore_pet.h - Johan Smet - BSD-3-Clause (see LICENSE)
//
// Emulates a Commodore PET 2001N

#include "dev_commodore_pet.h"

#include "crt.h"
#include "utils.h"

#include "chip.h"
//> COUT_DMG #include "chip_6520.h"
//> COUT_DMG #include "chip_6522.h"
//> COUT_DMG #include "chip_74xxx.h"
//> COUT_DMG #include "chip_mc3446a.h"
#include "chip_oscillator.h"
#include "chip_poweronreset.h"
//> COUT_DMG #include "chip_ram_static.h"
//> COUT_DMG #include "chip_ram_dynamic.h"
//> COUT_DMG #include "chip_rom.h"
#include "cpu_6502.h"
//> COUT_DMG #include "input_keypad.h"
//> COUT_DMG #include "perif_pet_crt.h"
//> COUT_DMG #include "perif_datassette_1530.h"
//> COUT_DMG #include "perif_disk_2031.h"
#include "stb/stb_ds.h"

#include "signal_history_profiles.h"

//> COUT_DMG #include "ram_8d_16a.h"


///////////////////////////////////////////////////////////////////////////////
//
// internal - glue logic
//

#define CHIP_GLUE_LOGIC_MAX_PIN_COUNT 40

typedef struct ChipGlueLogic {
	CHIP_DECLARE_BASE

	DevCommodorePet *device;
	SignalPool *	signal_pool;
	Signal			signals[CHIP_GLUE_LOGIC_MAX_PIN_COUNT];
} ChipGlueLogic;

//> COUT_DMG static uint8_t ChipGlueLogic01_PinTypes[CHIP_GLUE_LOGIC_MAX_PIN_COUNT] = {0};
//> COUT_DMG static uint8_t ChipGlueLogic03_PinTypes[CHIP_GLUE_LOGIC_MAX_PIN_COUNT] = {0};
//> COUT_DMG static uint8_t ChipGlueLogic05_PinTypes[CHIP_GLUE_LOGIC_MAX_PIN_COUNT] = {0};
//> COUT_DMG static uint8_t ChipGlueLogic06_PinTypes[CHIP_GLUE_LOGIC_MAX_PIN_COUNT] = {0};
//> COUT_DMG static uint8_t ChipGlueLogic06P_PinTypes[CHIP_GLUE_LOGIC_MAX_PIN_COUNT] = {0};
//> COUT_DMG static uint8_t ChipGlueLogic07_PinTypes[CHIP_GLUE_LOGIC_MAX_PIN_COUNT] = {0};
//> COUT_DMG static uint8_t ChipGlueLogic08_PinTypes[CHIP_GLUE_LOGIC_MAX_PIN_COUNT] = {0};
//> COUT_DMG static uint8_t ChipGlueLogic17_PinTypes[CHIP_GLUE_LOGIC_MAX_PIN_COUNT] = {0};

//> COUT_DMG static ChipGlueLogic *glue_logic_create_01(DevCommodorePet *device);
//> COUT_DMG static void glue_logic_process_01(ChipGlueLogic *chip);
//> COUT_DMG static ChipGlueLogic *glue_logic_create_03(DevCommodorePet *device);
//> COUT_DMG static void glue_logic_process_03(ChipGlueLogic *chip);
//> COUT_DMG static ChipGlueLogic *glue_logic_create_05(DevCommodorePet *device);
//> COUT_DMG static void glue_logic_process_05(ChipGlueLogic *chip);
//> COUT_DMG static ChipGlueLogic *glue_logic_create_06(DevCommodorePet *device);
//> COUT_DMG static void glue_logic_process_06(ChipGlueLogic *chip);
//> COUT_DMG static ChipGlueLogic *glue_logic_create_06_phases(DevCommodorePet *device);
//> COUT_DMG static void glue_logic_process_06_phases(ChipGlueLogic *chip);
//> COUT_DMG static ChipGlueLogic *glue_logic_create_07(DevCommodorePet *device);
//> COUT_DMG static void glue_logic_process_07(ChipGlueLogic *chip);
//> COUT_DMG static ChipGlueLogic *glue_logic_create_08(DevCommodorePet *device);
//> COUT_DMG static void glue_logic_process_08(ChipGlueLogic *chip);

//> COUT_DMG static ChipGlueLogic *glue_logic_create_07_lite(DevCommodorePet *device);
//> COUT_DMG static void glue_logic_process_07_lite(ChipGlueLogic *chip);

static inline ChipGlueLogic *glue_logic_create(DevCommodorePet *device) {
	ChipGlueLogic *chip = (ChipGlueLogic *) dms_calloc(1, sizeof(ChipGlueLogic));
	chip->device = device;
	chip->signal_pool = device->signal_pool;
	return chip;
}

//> COUT_DMG static void glue_logic_destroy(ChipGlueLogic *chip) {
//> COUT_DMG 	assert(chip);
//> COUT_DMG 	dms_free(chip);
//> COUT_DMG }

#define DEVICE_SIGNAL(sig)				\
	chip->device->signals[SIGNAL_CONCAT(SIG_P2001N_, sig)]

#define GLUE_PIN(s,t)									\
	chip->signals[SIGNAL_ENUM(s)] = DEVICE_SIGNAL(s);	\
	chip->pin_types[SIGNAL_ENUM(s)] = (t);

// glue-logic: micro-processor / memory expansion
#define SIGNAL_PREFIX		CHIP_GLUE_01_
#define SIGNAL_OWNER		chip

enum ChipGlueLogic01SignalAssignment {
	CHIP_GLUE_01_BA6 = CHIP_PIN_01,
	CHIP_GLUE_01_BA8,
	CHIP_GLUE_01_BA9,
	CHIP_GLUE_01_BA10,
	CHIP_GLUE_01_BA11,
	CHIP_GLUE_01_BA15,
	CHIP_GLUE_01_SEL8_B,
	CHIP_GLUE_01_RW,
	CHIP_GLUE_01_CLK1,
	CHIP_GLUE_01_RESET_BTN_B,
	CHIP_GLUE_01_RESET_B,
	CHIP_GLUE_01_RESET,
	CHIP_GLUE_01_INIT_B,
	CHIP_GLUE_01_INIT,
	CHIP_GLUE_01_IFC_B,
	CHIP_GLUE_01_SEL8,
	CHIP_GLUE_01_BA11_B,
	CHIP_GLUE_01_X8XX,
	CHIP_GLUE_01_88XX_B,
	CHIP_GLUE_01_ROMA_B,
	CHIP_GLUE_01_RAMR_B,
	CHIP_GLUE_01_RAMW_B,
	CHIP_GLUE_01_BRW,
	CHIP_GLUE_01_BRW_B,
	CHIP_GLUE_01_RAMRW,
	CHIP_GLUE_01_PHI2,
	CHIP_GLUE_01_BPHI2,
	CHIP_GLUE_01_CPHI2,
	CHIP_GLUE_01_CS1,
	CHIP_GLUE_01_DIAG,

	CHIP_GLUE_01_PIN_COUNT
};

static_assert(CHIP_GLUE_01_PIN_COUNT <= CHIP_GLUE_LOGIC_MAX_PIN_COUNT, "Too many signals in GlueLogic01");

//> COUT_DMG static ChipGlueLogic *glue_logic_create_01(DevCommodorePet *device) {
//> COUT_DMG 	assert(device);
//> COUT_DMG 
//> COUT_DMG 	ChipGlueLogic *chip = glue_logic_create(device);
//> COUT_DMG 	CHIP_SET_FUNCTIONS(chip, glue_logic_process_01, glue_logic_destroy);
//> COUT_DMG 	CHIP_SET_VARIABLES(chip, chip->device->simulator, chip->signals, ChipGlueLogic01_PinTypes, CHIP_GLUE_01_PIN_COUNT);
//> COUT_DMG 
//> COUT_DMG 	GLUE_PIN(BA6,			CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(BA8,			CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(BA9,			CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(BA10,			CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(BA11,			CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(BA15,			CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(SEL8_B,		CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(RW,			CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(CLK1,			CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(RESET_BTN_B,	CHIP_PIN_OUTPUT);
//> COUT_DMG 	GLUE_PIN(RESET_B,		CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(RESET,			CHIP_PIN_OUTPUT);
//> COUT_DMG 	GLUE_PIN(INIT_B,		CHIP_PIN_OUTPUT);
//> COUT_DMG 	GLUE_PIN(INIT,			CHIP_PIN_OUTPUT);
//> COUT_DMG 	GLUE_PIN(IFC_B,			CHIP_PIN_OUTPUT);
//> COUT_DMG 	GLUE_PIN(SEL8,			CHIP_PIN_OUTPUT);
//> COUT_DMG 	GLUE_PIN(BA11_B,		CHIP_PIN_OUTPUT);
//> COUT_DMG 	GLUE_PIN(X8XX,			CHIP_PIN_OUTPUT);
//> COUT_DMG 	GLUE_PIN(88XX_B,		CHIP_PIN_OUTPUT);
//> COUT_DMG 	GLUE_PIN(ROMA_B,		CHIP_PIN_OUTPUT);
//> COUT_DMG 	GLUE_PIN(RAMR_B,		CHIP_PIN_OUTPUT);
//> COUT_DMG 	GLUE_PIN(RAMW_B,		CHIP_PIN_OUTPUT);
//> COUT_DMG 	GLUE_PIN(BRW,			CHIP_PIN_OUTPUT);
//> COUT_DMG 	GLUE_PIN(BRW_B,			CHIP_PIN_OUTPUT);
//> COUT_DMG 	GLUE_PIN(RAMRW,			CHIP_PIN_OUTPUT);
//> COUT_DMG 	GLUE_PIN(PHI2,			CHIP_PIN_OUTPUT);
//> COUT_DMG 	GLUE_PIN(BPHI2,			CHIP_PIN_OUTPUT);
//> COUT_DMG 	GLUE_PIN(CPHI2,			CHIP_PIN_OUTPUT);
//> COUT_DMG 	GLUE_PIN(CS1,			CHIP_PIN_OUTPUT);
//> COUT_DMG 	GLUE_PIN(DIAG,			CHIP_PIN_OUTPUT);
//> COUT_DMG 
//> COUT_DMG 	return chip;
//> COUT_DMG }

//> COUT_DMG static void glue_logic_process_01(ChipGlueLogic *chip) {
//> COUT_DMG 	assert(chip);
//> COUT_DMG 
//> COUT_DMG 	bool ba8 = SIGNAL_READ(BA8);
//> COUT_DMG 	bool ba9 = SIGNAL_READ(BA9);
//> COUT_DMG 	bool ba10 = SIGNAL_READ(BA10);
//> COUT_DMG 	bool ba15 = SIGNAL_READ(BA15);
//> COUT_DMG 	bool sel8_b = SIGNAL_READ(SEL8_B);
//> COUT_DMG 	bool rw   = SIGNAL_READ(RW);
//> COUT_DMG 
//> COUT_DMG 	SIGNAL_WRITE(RESET_BTN_B, !chip->device->in_reset);
//> COUT_DMG 	chip->device->in_reset = false;
//> COUT_DMG 
//> COUT_DMG 	if (chip->device->diag_toggled) {
//> COUT_DMG 		SIGNAL_WRITE(DIAG, !chip->device->diag_mode);
//> COUT_DMG 		chip->device->diag_toggled = false;
//> COUT_DMG 	}
//> COUT_DMG 
//> COUT_DMG 	// A3 (1, 2)
//> COUT_DMG 	bool reset_b = SIGNAL_READ(RESET_B);
//> COUT_DMG 	SIGNAL_WRITE(RESET, !reset_b);
//> COUT_DMG 
//> COUT_DMG 	SIGNAL_WRITE(INIT_B, reset_b);
//> COUT_DMG 	SIGNAL_WRITE(INIT, !reset_b);
//> COUT_DMG 
//> COUT_DMG 	// A3 (11, 10)
//> COUT_DMG 	bool sel8 = !sel8_b;
//> COUT_DMG 	SIGNAL_WRITE(SEL8, sel8);
//> COUT_DMG 
//> COUT_DMG 	// A3 (3, 4)
//> COUT_DMG 	bool ba11_b = !SIGNAL_READ(BA11);
//> COUT_DMG 	SIGNAL_WRITE(BA11_B, ba11_b);
//> COUT_DMG 
//> COUT_DMG 	// B2 (1,2,4,5,6)
//> COUT_DMG 	bool x8xx = !(ba8 | ba9 | ba10 | ba11_b);
//> COUT_DMG 	SIGNAL_WRITE(X8XX, x8xx);
//> COUT_DMG 
//> COUT_DMG 	// A4 (4,5,6)
//> COUT_DMG 	bool addr_88xx_b = !(sel8 && x8xx);
//> COUT_DMG 	SIGNAL_WRITE(88XX_B, addr_88xx_b);
//> COUT_DMG 
//> COUT_DMG 	// A4 (1,2,3)
//> COUT_DMG 	bool rom_addr_b = !(ba15 && sel8_b);
//> COUT_DMG 	SIGNAL_WRITE(ROMA_B, rom_addr_b);
//> COUT_DMG 
//> COUT_DMG 	// A5 (3,4,5,6)
//> COUT_DMG 	bool ram_read_b = !(rom_addr_b && addr_88xx_b && rw);
//> COUT_DMG 	SIGNAL_WRITE(RAMR_B, ram_read_b);
//> COUT_DMG 	SIGNAL_WRITE(RAMW_B, !ram_read_b);
//> COUT_DMG 
//> COUT_DMG 	// A10 (3,4)
//> COUT_DMG 	SIGNAL_WRITE(BRW, rw);
//> COUT_DMG 
//> COUT_DMG 	// A3 (12,13)
//> COUT_DMG 	SIGNAL_WRITE(BRW_B, !rw);
//> COUT_DMG 
//> COUT_DMG 	// A3 (9,8)
//> COUT_DMG 	SIGNAL_WRITE(RAMRW, rw);
//> COUT_DMG 
//> COUT_DMG 	// FIXME: cpu doesn't output phi2 clock signal
//> COUT_DMG 	bool phi2 = SIGNAL_READ(CLK1);
//> COUT_DMG 	SIGNAL_WRITE(PHI2, phi2);
//> COUT_DMG 	SIGNAL_WRITE(BPHI2, phi2);
//> COUT_DMG 	SIGNAL_WRITE(CPHI2, phi2);
//> COUT_DMG 
//> COUT_DMG 	// IEEE-488: F1 (11,12,13)
//> COUT_DMG 	bool cs1 = x8xx && SIGNAL_READ(BA6);
//> COUT_DMG 	SIGNAL_WRITE(CS1, cs1);
//> COUT_DMG 
//> COUT_DMG 	// IEEE-488: A10 (10,11): open collector buffer
//> COUT_DMG 	if (!SIGNAL_READ(RESET_B)) {
//> COUT_DMG 		SIGNAL_WRITE(IFC_B, false);
//> COUT_DMG 	} else {
//> COUT_DMG 		SIGNAL_NO_WRITE(IFC_B);
//> COUT_DMG 	}
//> COUT_DMG }

// glue-logic: cassette & keyboard
#undef  SIGNAL_PREFIX
#define SIGNAL_PREFIX		CHIP_GLUE_03_
#undef  SIGNAL_OWNER
#define SIGNAL_OWNER		chip

enum ChipGlueLogic03SignalAssignment {
	CHIP_GLUE_03_CASS_MOTOR_1 = CHIP_PIN_01,
	CHIP_GLUE_03_CASS_MOTOR_1_B,
	CHIP_GLUE_03_CASS_MOTOR_2,
	CHIP_GLUE_03_CASS_MOTOR_2_B,

	CHIP_GLUE_03_EOI_OUT_B,
	CHIP_GLUE_03_EOI_IN_B,
	CHIP_GLUE_03_EOI_B,

	CHIP_GLUE_03_PIN_COUNT
};
static_assert(CHIP_GLUE_03_PIN_COUNT <= CHIP_GLUE_LOGIC_MAX_PIN_COUNT, "Too many signals in GlueLogic03");

//> COUT_DMG static ChipGlueLogic *glue_logic_create_03(DevCommodorePet *device) {
//> COUT_DMG 	assert(device);
//> COUT_DMG 
//> COUT_DMG 	ChipGlueLogic *chip = glue_logic_create(device);
//> COUT_DMG 	CHIP_SET_FUNCTIONS(chip, glue_logic_process_03, glue_logic_destroy);
//> COUT_DMG 	CHIP_SET_VARIABLES(chip, chip->device->simulator, chip->signals, ChipGlueLogic03_PinTypes, CHIP_GLUE_03_PIN_COUNT);
//> COUT_DMG 
//> COUT_DMG 	GLUE_PIN(CASS_MOTOR_1,		CHIP_PIN_OUTPUT);
//> COUT_DMG 	GLUE_PIN(CASS_MOTOR_1_B,	CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(CASS_MOTOR_2,		CHIP_PIN_OUTPUT);
//> COUT_DMG 	GLUE_PIN(CASS_MOTOR_2_B,	CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 
//> COUT_DMG 	GLUE_PIN(EOI_OUT_B,			CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(EOI_IN_B,			CHIP_PIN_OUTPUT);
//> COUT_DMG 	GLUE_PIN(EOI_B,				CHIP_PIN_INPUT | CHIP_PIN_OUTPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 
//> COUT_DMG 	return chip;
//> COUT_DMG }

//> COUT_DMG static void glue_logic_process_03(ChipGlueLogic *chip) {
//> COUT_DMG 	assert(chip);
//> COUT_DMG 
//> COUT_DMG 	SIGNAL_WRITE(CASS_MOTOR_1, !SIGNAL_READ(CASS_MOTOR_1_B));
//> COUT_DMG 	SIGNAL_WRITE(CASS_MOTOR_2, !SIGNAL_READ(CASS_MOTOR_2_B));
//> COUT_DMG 
//> COUT_DMG 	// IEEE-488: A10 (5,6): open collector buffer
//> COUT_DMG 	if (!SIGNAL_READ(EOI_OUT_B)) {
//> COUT_DMG 		SIGNAL_WRITE(EOI_B, false);
//> COUT_DMG 		SIGNAL_WRITE(EOI_IN_B, false);
//> COUT_DMG 	} else {
//> COUT_DMG 		SIGNAL_NO_WRITE(EOI_B);
//> COUT_DMG 		SIGNAL_WRITE(EOI_IN_B, SIGNAL_READ(EOI_B));
//> COUT_DMG 	}
//> COUT_DMG }

// glue-logic: rams
#undef  SIGNAL_PREFIX
#define SIGNAL_PREFIX		CHIP_GLUE_05_
#undef  SIGNAL_OWNER
#define SIGNAL_OWNER		chip

enum ChipGlueLogic05SignalAssignment {
	CHIP_GLUE_05_BA15 = CHIP_PIN_01,
	CHIP_GLUE_05_BANKSEL,
	CHIP_GLUE_05_BRW,
	CHIP_GLUE_05_G7_8,

	CHIP_GLUE_05_PIN_COUNT
};
static_assert(CHIP_GLUE_05_PIN_COUNT <= CHIP_GLUE_LOGIC_MAX_PIN_COUNT, "Too many signals in GlueLogic05");

//> COUT_DMG static ChipGlueLogic *glue_logic_create_05(DevCommodorePet *device) {
//> COUT_DMG 	assert(device);
//> COUT_DMG 
//> COUT_DMG 	ChipGlueLogic *chip = glue_logic_create(device);
//> COUT_DMG 	CHIP_SET_FUNCTIONS(chip, glue_logic_process_05, glue_logic_destroy);
//> COUT_DMG 	CHIP_SET_VARIABLES(chip, chip->device->simulator, chip->signals, ChipGlueLogic05_PinTypes, CHIP_GLUE_05_PIN_COUNT);
//> COUT_DMG 
//> COUT_DMG 	GLUE_PIN(BA15,		CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(BANKSEL,	CHIP_PIN_OUTPUT);
//> COUT_DMG 	GLUE_PIN(BRW,		CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(G7_8,		CHIP_PIN_OUTPUT);
//> COUT_DMG 
//> COUT_DMG 	return chip;
//> COUT_DMG }

//> COUT_DMG static void glue_logic_process_05(ChipGlueLogic *chip) {
//> COUT_DMG 	assert(chip);
//> COUT_DMG 
//> COUT_DMG 	// B2: 8, 9, 10, 12, 13 (4-Input NOR-gate with strobe)
//> COUT_DMG 	bool banksel = !(true && (false || false || false || SIGNAL_READ(BA15)));
//> COUT_DMG 	SIGNAL_WRITE(BANKSEL, banksel);
//> COUT_DMG 
//> COUT_DMG 	// G7: 8, 9, 10, 11
//> COUT_DMG 	bool g78 = !(true && banksel && SIGNAL_READ(BRW));
//> COUT_DMG 	SIGNAL_WRITE(G7_8, g78);
//> COUT_DMG }

// glue-logic: master timing
#undef  SIGNAL_PREFIX
#define SIGNAL_PREFIX		CHIP_GLUE_06_
#undef  SIGNAL_OWNER
#define SIGNAL_OWNER		chip

enum ChipGlueLogic06SignalAssignment {
	CHIP_GLUE_06_RA1AND3,
	CHIP_GLUE_06_RA1,
	CHIP_GLUE_06_RA3,
	CHIP_GLUE_06_RA4AND6,
	CHIP_GLUE_06_RA4,
	CHIP_GLUE_06_RA6,
	CHIP_GLUE_06_RA5AND6_B,
	CHIP_GLUE_06_RA5,
	CHIP_GLUE_06_RA6_B,
	CHIP_GLUE_06_H8Q,
	CHIP_GLUE_06_H8Q2,
	CHIP_GLUE_06_VIDEO_ON,
	CHIP_GLUE_06_H8Q2_B,
	CHIP_GLUE_06_H8Q_B,
	CHIP_GLUE_06_VERT_DRIVE,
	CHIP_GLUE_06_BANKSEL,
	CHIP_GLUE_06_BPHI2,
	CHIP_GLUE_06_H53,
	CHIP_GLUE_06_H1Q1_B,
	CHIP_GLUE_06_H1Q2_B,
	CHIP_GLUE_06_RAS0_B,
	CHIP_GLUE_06_H4Y4,
	CHIP_GLUE_06_CAS1_B,
	CHIP_GLUE_06_BA14,
	CHIP_GLUE_06_BA14_B,
	CHIP_GLUE_06_CAS0_B,

	CHIP_GLUE_06_PIN_COUNT
};
static_assert(CHIP_GLUE_06_PIN_COUNT <= CHIP_GLUE_LOGIC_MAX_PIN_COUNT, "Too many signals in GlueLogic06");

//> COUT_DMG static ChipGlueLogic *glue_logic_create_06(DevCommodorePet *device) {
//> COUT_DMG 	assert(device);
//> COUT_DMG 
//> COUT_DMG 	ChipGlueLogic *chip = glue_logic_create(device);
//> COUT_DMG 	CHIP_SET_FUNCTIONS(chip, glue_logic_process_06, glue_logic_destroy);
//> COUT_DMG 	CHIP_SET_VARIABLES(chip, chip->device->simulator, chip->signals, ChipGlueLogic06_PinTypes, CHIP_GLUE_06_PIN_COUNT);
//> COUT_DMG 
//> COUT_DMG 	GLUE_PIN(RA1AND3,		CHIP_PIN_OUTPUT);
//> COUT_DMG 	GLUE_PIN(RA1,			CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(RA3,			CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(RA4AND6,		CHIP_PIN_OUTPUT);
//> COUT_DMG 	GLUE_PIN(RA4,			CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(RA6,			CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(RA5AND6_B,		CHIP_PIN_OUTPUT);
//> COUT_DMG 	GLUE_PIN(RA5,			CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(RA6_B,			CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(H8Q,			CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(H8Q2,			CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(VIDEO_ON,		CHIP_PIN_OUTPUT);
//> COUT_DMG 	GLUE_PIN(H8Q2_B,		CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(H8Q_B,			CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(VERT_DRIVE,	CHIP_PIN_OUTPUT);
//> COUT_DMG 	GLUE_PIN(BANKSEL,		CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(BPHI2,			CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(H53,			CHIP_PIN_OUTPUT);
//> COUT_DMG 	GLUE_PIN(H1Q1_B,		CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(H1Q2_B,		CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(RAS0_B,		CHIP_PIN_OUTPUT);
//> COUT_DMG 	GLUE_PIN(H4Y4,			CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(CAS1_B,		CHIP_PIN_OUTPUT);
//> COUT_DMG 	GLUE_PIN(BA14,			CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(BA14_B,		CHIP_PIN_OUTPUT);
//> COUT_DMG 	GLUE_PIN(CAS0_B,		CHIP_PIN_OUTPUT);
//> COUT_DMG 
//> COUT_DMG 	return chip;
//> COUT_DMG }

//> COUT_DMG static void glue_logic_process_06(ChipGlueLogic *chip) {
//> COUT_DMG 	assert(chip);
//> COUT_DMG 
//> COUT_DMG 	// F1 (8,9,10)
//> COUT_DMG 	SIGNAL_WRITE(RA1AND3, SIGNAL_READ(RA1) && SIGNAL_READ(RA3));
//> COUT_DMG 
//> COUT_DMG 	// H10 (4,5,6)
//> COUT_DMG 	SIGNAL_WRITE(RA4AND6, SIGNAL_READ(RA4) && SIGNAL_READ(RA6));
//> COUT_DMG 
//> COUT_DMG 	// H10 (1,2,3)
//> COUT_DMG 	SIGNAL_WRITE(RA5AND6_B, SIGNAL_READ(RA5) && SIGNAL_READ(RA6_B));
//> COUT_DMG 
//> COUT_DMG 	// H10 (11,12,13)
//> COUT_DMG 	bool video_on = SIGNAL_READ(H8Q) && SIGNAL_READ(H8Q2);
//> COUT_DMG 	SIGNAL_WRITE(VIDEO_ON, video_on);
//> COUT_DMG 
//> COUT_DMG 	// G10 (11,12,13)
//> COUT_DMG 	bool vert_drive = !(SIGNAL_READ(H8Q2_B) && SIGNAL_READ(H8Q_B));
//> COUT_DMG 	SIGNAL_WRITE(VERT_DRIVE, vert_drive);
//> COUT_DMG 
//> COUT_DMG 	// H5 (1,2,3)
//> COUT_DMG 	bool h53 = !(SIGNAL_READ(BANKSEL) && SIGNAL_READ(BPHI2));
//> COUT_DMG 	SIGNAL_WRITE(H53, h53);
//> COUT_DMG 
//> COUT_DMG 	// G1 (11,12,13)
//> COUT_DMG 	bool ras0_b = SIGNAL_READ(H1Q1_B) && SIGNAL_READ(H1Q2_B);
//> COUT_DMG 	SIGNAL_WRITE(RAS0_B, ras0_b);
//> COUT_DMG 
//> COUT_DMG 	// G7 (3,4,5,6)
//> COUT_DMG 	bool cas1_b = !(SIGNAL_READ(H4Y4) && SIGNAL_READ(BA14));
//> COUT_DMG 	SIGNAL_WRITE(CAS1_B, cas1_b);
//> COUT_DMG 
//> COUT_DMG 	// H2 (8,9)
//> COUT_DMG 	bool ba14_b = !SIGNAL_READ(BA14);
//> COUT_DMG 	SIGNAL_WRITE(BA14_B, ba14_b);
//> COUT_DMG 
//> COUT_DMG 	// G7 (1,2,12,13)
//> COUT_DMG 	bool cas0_b = !(SIGNAL_READ(H4Y4) && ba14_b);
//> COUT_DMG 	SIGNAL_WRITE(CAS0_B, cas0_b);
//> COUT_DMG }

// glue-logic: master timing

#undef  SIGNAL_PREFIX
#define SIGNAL_PREFIX		CHIP_GLUE_06P_
#undef  SIGNAL_OWNER
#define SIGNAL_OWNER		chip

enum ChipGlueLogic06PSignalAssignment {
	CHIP_GLUE_06P_BPHI2A_B = CHIP_PIN_01,
	CHIP_GLUE_06P_BPHI2A,
	CHIP_GLUE_06P_BPHI2B_B,
	CHIP_GLUE_06P_BPHI2B,
	CHIP_GLUE_06P_BPHI2F_B,
	CHIP_GLUE_06P_BPHI2F,
	CHIP_GLUE_06P_BPHI2G_B,
	CHIP_GLUE_06P_BPHI2G,
	CHIP_GLUE_06P_BPHI2H,
	CHIP_GLUE_06P_VIDEO_LATCH,

	CHIP_GLUE_06P_PIN_COUNT
};
static_assert(CHIP_GLUE_06P_PIN_COUNT <= CHIP_GLUE_LOGIC_MAX_PIN_COUNT, "Too many signals in GlueLogic06P");

//> COUT_DMG static ChipGlueLogic *glue_logic_create_06_phases(DevCommodorePet *device) {
//> COUT_DMG 	assert(device);
//> COUT_DMG 
//> COUT_DMG 	ChipGlueLogic *chip = glue_logic_create(device);
//> COUT_DMG 	CHIP_SET_FUNCTIONS(chip, glue_logic_process_06_phases, glue_logic_destroy);
//> COUT_DMG 	CHIP_SET_VARIABLES(chip, chip->device->simulator, chip->signals, ChipGlueLogic06P_PinTypes, CHIP_GLUE_06P_PIN_COUNT);
//> COUT_DMG 
//> COUT_DMG 	GLUE_PIN(BPHI2A_B,		CHIP_PIN_OUTPUT);
//> COUT_DMG 	GLUE_PIN(BPHI2A,		CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(BPHI2B_B,		CHIP_PIN_OUTPUT);
//> COUT_DMG 	GLUE_PIN(BPHI2B,		CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(BPHI2F_B,		CHIP_PIN_OUTPUT);
//> COUT_DMG 	GLUE_PIN(BPHI2F,		CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(BPHI2G_B,		CHIP_PIN_OUTPUT);
//> COUT_DMG 	GLUE_PIN(BPHI2G,		CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(BPHI2H,		CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(VIDEO_LATCH,	CHIP_PIN_OUTPUT);
//> COUT_DMG 
//> COUT_DMG 	return chip;
//> COUT_DMG }

//> COUT_DMG static void glue_logic_process_06_phases(ChipGlueLogic *chip) {
//> COUT_DMG 	assert(chip);
//> COUT_DMG 
//> COUT_DMG 	// H2 (1,2) + (3,4) + (11,10) + (13,12)
//> COUT_DMG 	SIGNAL_WRITE(BPHI2A_B, !SIGNAL_READ(BPHI2A));
//> COUT_DMG 	SIGNAL_WRITE(BPHI2B_B, !SIGNAL_READ(BPHI2B));
//> COUT_DMG 	SIGNAL_WRITE(BPHI2F_B, !SIGNAL_READ(BPHI2F));
//> COUT_DMG 	SIGNAL_WRITE(BPHI2G_B, !SIGNAL_READ(BPHI2G));
//> COUT_DMG 
//> COUT_DMG 	// G1 (8,9,10)
//> COUT_DMG 	bool video_latch = (!SIGNAL_READ(BPHI2F) && SIGNAL_READ(BPHI2H));
//> COUT_DMG 	SIGNAL_WRITE(VIDEO_LATCH, video_latch);
//> COUT_DMG }

// glue-logic: display logic
#undef  SIGNAL_PREFIX
#define SIGNAL_PREFIX		CHIP_GLUE_07_
#undef  SIGNAL_OWNER
#define SIGNAL_OWNER		chip

enum ChipGlueLogic07SignalAssignment {
	CHIP_GLUE_07_BRW = CHIP_PIN_01,
	CHIP_GLUE_07_BA11_B,
	CHIP_GLUE_07_SEL8,
	CHIP_GLUE_07_TV_SEL,
	CHIP_GLUE_07_TV_READ_B,
	CHIP_GLUE_07_BPHI2,
	CHIP_GLUE_07_A5_12,
	CHIP_GLUE_07_VIDEO_ON_B,
	CHIP_GLUE_07_VIDEO_ON,
	CHIP_GLUE_07_GA6,
	CHIP_GLUE_07_PULLUP_2,
	CHIP_GLUE_07_RA9,
	CHIP_GLUE_07_LINES_20_B,
	CHIP_GLUE_07_LGA6,
	CHIP_GLUE_07_LGA7,
	CHIP_GLUE_07_LGA8,
	CHIP_GLUE_07_LGA9,
	CHIP_GLUE_07_LGA_HI_B,
	CHIP_GLUE_07_LGA_HI,
	CHIP_GLUE_07_LGA3,
	CHIP_GLUE_07_LINES_200_B,
	CHIP_GLUE_07_LINE_220,
	CHIP_GLUE_07_HORZ_DISP_OFF,
	CHIP_GLUE_07_W220_OFF,
	CHIP_GLUE_07_RELOAD_B,
	CHIP_GLUE_07_NEXT_B,
	CHIP_GLUE_07_RELOAD_NEXT,

	CHIP_GLUE_07_PIN_COUNT
};
static_assert(CHIP_GLUE_07_PIN_COUNT <= CHIP_GLUE_LOGIC_MAX_PIN_COUNT, "Too many signals in GlueLogic07");

//> COUT_DMG static ChipGlueLogic *glue_logic_create_07(DevCommodorePet *device) {
//> COUT_DMG 	assert(device);
//> COUT_DMG 
//> COUT_DMG 	ChipGlueLogic *chip = glue_logic_create(device);
//> COUT_DMG 	CHIP_SET_FUNCTIONS(chip, glue_logic_process_07, glue_logic_destroy);
//> COUT_DMG 	CHIP_SET_VARIABLES(chip, chip->device->simulator, chip->signals, ChipGlueLogic07_PinTypes, CHIP_GLUE_07_PIN_COUNT);
//> COUT_DMG 
//> COUT_DMG 	GLUE_PIN(BRW,			CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(BA11_B,		CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(SEL8,			CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(TV_SEL,		CHIP_PIN_OUTPUT);
//> COUT_DMG 	GLUE_PIN(TV_READ_B,		CHIP_PIN_OUTPUT);
//> COUT_DMG 	GLUE_PIN(BPHI2,			CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(A5_12,			CHIP_PIN_OUTPUT);
//> COUT_DMG 	GLUE_PIN(VIDEO_ON_B,	CHIP_PIN_OUTPUT);
//> COUT_DMG 	GLUE_PIN(VIDEO_ON,		CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(GA6,			CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(PULLUP_2,		CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(RA9,			CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(LINES_20_B,	CHIP_PIN_OUTPUT);
//> COUT_DMG 	GLUE_PIN(LGA6,			CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(LGA7,			CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(LGA8,			CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(LGA9,			CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(LGA_HI_B,		CHIP_PIN_OUTPUT);
//> COUT_DMG 	GLUE_PIN(LGA_HI,		CHIP_PIN_OUTPUT);
//> COUT_DMG 	GLUE_PIN(LGA3,			CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(LINES_200_B,	CHIP_PIN_OUTPUT);
//> COUT_DMG 	GLUE_PIN(LINE_220,		CHIP_PIN_OUTPUT);
//> COUT_DMG 	GLUE_PIN(HORZ_DISP_OFF,	CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(W220_OFF,		CHIP_PIN_OUTPUT);
//> COUT_DMG 	GLUE_PIN(RELOAD_B,		CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(NEXT_B,		CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(RELOAD_NEXT,	CHIP_PIN_OUTPUT);
//> COUT_DMG 
//> COUT_DMG 	return chip;
//> COUT_DMG }

//> COUT_DMG static void glue_logic_process_07(ChipGlueLogic *chip) {
//> COUT_DMG 	assert(chip);
//> COUT_DMG 
//> COUT_DMG 	bool buf_rw = SIGNAL_READ(BRW);
//> COUT_DMG 
//> COUT_DMG 	// >> F1 (4,5,6)
//> COUT_DMG 	bool tv_sel = SIGNAL_READ(BA11_B) && SIGNAL_READ(SEL8);
//> COUT_DMG 	SIGNAL_WRITE(TV_SEL, tv_sel);
//> COUT_DMG 
//> COUT_DMG 	// >> A5 (8,9,10,11)
//> COUT_DMG 	bool tv_read_b = !(true && buf_rw && tv_sel);
//> COUT_DMG 	SIGNAL_WRITE(TV_READ_B, tv_read_b);
//> COUT_DMG 
//> COUT_DMG 	// >> A5 (1,2,12,13)
//> COUT_DMG 	bool a5_12 = !(tv_sel && !buf_rw && SIGNAL_READ(BPHI2));
//> COUT_DMG 	SIGNAL_WRITE(A5_12, a5_12);
//> COUT_DMG 
//> COUT_DMG 	// >> I1 (5,6)
//> COUT_DMG 	SIGNAL_WRITE(VIDEO_ON_B, !SIGNAL_READ(VIDEO_ON));
//> COUT_DMG 
//> COUT_DMG 	// >> G2 (1,2,4,5,6)
//> COUT_DMG 	bool lines_20_b = !(SIGNAL_READ(GA6) && SIGNAL_READ(PULLUP_2) && !SIGNAL_READ(VIDEO_ON) && SIGNAL_READ(RA9));
//> COUT_DMG 	SIGNAL_WRITE(LINES_20_B, lines_20_b);
//> COUT_DMG 
//> COUT_DMG 	// >> G2 (8,9,10,12,13)
//> COUT_DMG 	bool lga_hi_b = !(SIGNAL_READ(LGA6) && SIGNAL_READ(LGA7) && SIGNAL_READ(LGA8) && SIGNAL_READ(LGA9));
//> COUT_DMG 	SIGNAL_WRITE(LGA_HI_B, lga_hi_b);
//> COUT_DMG 
//> COUT_DMG 	// >> I1 (8,9)
//> COUT_DMG 	SIGNAL_WRITE(LGA_HI, !lga_hi_b);
//> COUT_DMG 
//> COUT_DMG 	// >> H5 (4,5,6)
//> COUT_DMG 	bool lines_200_b = !(SIGNAL_READ(LGA3) && SIGNAL_READ(LGA_HI));
//> COUT_DMG 	SIGNAL_WRITE(LINES_200_B, lines_200_b);
//> COUT_DMG 
//> COUT_DMG 	// >> H5 (8,9,10)
//> COUT_DMG 	bool line_220 = !(lines_200_b && lines_20_b);
//> COUT_DMG 	SIGNAL_WRITE(LINE_220, line_220);
//> COUT_DMG 
//> COUT_DMG 	// >> G3 (1,2,3)
//> COUT_DMG 	bool w220_off = line_220 && SIGNAL_READ(HORZ_DISP_OFF);
//> COUT_DMG 	SIGNAL_WRITE(W220_OFF, w220_off);
//> COUT_DMG 
//> COUT_DMG 	// >> H5 (11,12,13)
//> COUT_DMG 	bool reload_next = !(SIGNAL_READ(RELOAD_B) && SIGNAL_READ(NEXT_B));
//> COUT_DMG 	SIGNAL_WRITE(RELOAD_NEXT, reload_next);
//> COUT_DMG }

#undef  SIGNAL_PREFIX
#define SIGNAL_PREFIX		CHIP_GLUE_17_
#undef  SIGNAL_OWNER
#define SIGNAL_OWNER		chip

enum ChipGlueLogic17SignalAssignment {
	CHIP_GLUE_17_BA11_B,
	CHIP_GLUE_17_SEL8,
	CHIP_GLUE_17_TV_SEL,
	CHIP_GLUE_17_BRW,
	CHIP_GLUE_17_TV_READ_B,

	CHIP_GLUE_17_PIN_COUNT
};
static_assert(CHIP_GLUE_17_PIN_COUNT <= CHIP_GLUE_LOGIC_MAX_PIN_COUNT, "Too many signals in GlueLogic07Lite");

//> COUT_DMG static ChipGlueLogic *glue_logic_create_07_lite(DevCommodorePet *device) {
//> COUT_DMG 	assert(device);
//> COUT_DMG 
//> COUT_DMG 	ChipGlueLogic *chip = glue_logic_create(device);
//> COUT_DMG 	CHIP_SET_FUNCTIONS(chip, glue_logic_process_07_lite, glue_logic_destroy);
//> COUT_DMG 	CHIP_SET_VARIABLES(chip, chip->device->simulator, chip->signals, ChipGlueLogic17_PinTypes, CHIP_GLUE_17_PIN_COUNT);
//> COUT_DMG 
//> COUT_DMG 	GLUE_PIN(BA11_B,	CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(SEL8,		CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(TV_SEL,	CHIP_PIN_OUTPUT);
//> COUT_DMG 	GLUE_PIN(BRW,		CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(TV_READ_B,	CHIP_PIN_OUTPUT);
//> COUT_DMG 
//> COUT_DMG 	return chip;
//> COUT_DMG }
//> COUT_DMG 
//> COUT_DMG static void glue_logic_process_07_lite(ChipGlueLogic *chip) {
//> COUT_DMG 	assert(chip);
//> COUT_DMG 
//> COUT_DMG 	// >> F1 (4,5,6)
//> COUT_DMG 	bool tv_sel = SIGNAL_READ(BA11_B) && SIGNAL_READ(SEL8);
//> COUT_DMG 	SIGNAL_WRITE(TV_SEL, tv_sel);
//> COUT_DMG 
//> COUT_DMG 	// >> A5 (8,9,10,11)
//> COUT_DMG 	bool tv_read_b = !(true && SIGNAL_READ(BRW) && tv_sel);
//> COUT_DMG 	SIGNAL_WRITE(TV_READ_B, tv_read_b);
//> COUT_DMG }

// glue-logic: display rams
#undef  SIGNAL_PREFIX
#define SIGNAL_PREFIX		CHIP_GLUE_08_
#undef  SIGNAL_OWNER
#define SIGNAL_OWNER		chip

enum ChipGlueLogic08SignalAssignment {
	CHIP_GLUE_08_HORZ_DISP_ON,
	CHIP_GLUE_08_RA7,
	CHIP_GLUE_08_RA8,
	CHIP_GLUE_08_RA9,
	CHIP_GLUE_08_RELOAD_B,
	CHIP_GLUE_08_G9Q,
	CHIP_GLUE_08_E11QH,
	CHIP_GLUE_08_G106,
	CHIP_GLUE_08_G9Q_B,
	CHIP_GLUE_08_E11QH_B,
	CHIP_GLUE_08_G108,
	CHIP_GLUE_08_H108,
	CHIP_GLUE_08_VIDEO_ON,
	CHIP_GLUE_08_VIDEO,

	CHIP_GLUE_08_PIN_COUNT
};
static_assert(CHIP_GLUE_08_PIN_COUNT <= CHIP_GLUE_LOGIC_MAX_PIN_COUNT, "Too many signals in GlueLogic08");

//> COUT_DMG static ChipGlueLogic *glue_logic_create_08(DevCommodorePet *device) {
//> COUT_DMG 	assert(device);
//> COUT_DMG 
//> COUT_DMG 	ChipGlueLogic *chip = glue_logic_create(device);
//> COUT_DMG 	CHIP_SET_FUNCTIONS(chip, glue_logic_process_08, glue_logic_destroy);
//> COUT_DMG 	CHIP_SET_VARIABLES(chip, chip->device->simulator, chip->signals, ChipGlueLogic08_PinTypes, CHIP_GLUE_08_PIN_COUNT);
//> COUT_DMG 
//> COUT_DMG 	GLUE_PIN(HORZ_DISP_ON,	CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(RA7,			CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(RA8,			CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(RA9,			CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(RELOAD_B,		CHIP_PIN_OUTPUT);
//> COUT_DMG 	GLUE_PIN(G9Q,			CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(E11QH,			CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(G106,			CHIP_PIN_OUTPUT);
//> COUT_DMG 	GLUE_PIN(G9Q_B,			CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(E11QH_B,		CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(G108,			CHIP_PIN_OUTPUT);
//> COUT_DMG 	GLUE_PIN(H108,			CHIP_PIN_OUTPUT);
//> COUT_DMG 	GLUE_PIN(VIDEO_ON,		CHIP_PIN_INPUT | CHIP_PIN_TRIGGER);
//> COUT_DMG 	GLUE_PIN(VIDEO,			CHIP_PIN_OUTPUT);
//> COUT_DMG 
//> COUT_DMG 	return chip;
//> COUT_DMG }
//> COUT_DMG 
//> COUT_DMG static void glue_logic_process_08(ChipGlueLogic *chip) {
//> COUT_DMG 	assert(chip);
//> COUT_DMG 
//> COUT_DMG 	// g11 (8,9,10,12,13)
//> COUT_DMG 	bool reload_b = !(SIGNAL_READ(HORZ_DISP_ON) && SIGNAL_READ(RA7) && SIGNAL_READ(RA8) && SIGNAL_READ(RA9));
//> COUT_DMG 	SIGNAL_WRITE(RELOAD_B, reload_b);
//> COUT_DMG 
//> COUT_DMG 	// g10 (4,5,6)
//> COUT_DMG 	bool g106 = !(SIGNAL_READ(G9Q) && SIGNAL_READ(E11QH));
//> COUT_DMG 	SIGNAL_WRITE(G106, g106);
//> COUT_DMG 
//> COUT_DMG 	// g10 (8,9,10)
//> COUT_DMG 	bool g108 = !(SIGNAL_READ(G9Q_B) && SIGNAL_READ(E11QH_B));
//> COUT_DMG 	SIGNAL_WRITE(G108, g108);
//> COUT_DMG 
//> COUT_DMG 	// h10 (8,9,10)
//> COUT_DMG 	bool h108 = g106 && g108;
//> COUT_DMG 	SIGNAL_WRITE(H108, h108);
//> COUT_DMG 
//> COUT_DMG 	// g11 (1,2,5,4,6)
//> COUT_DMG 	bool video = !(h108 && true && SIGNAL_READ(VIDEO_ON) && SIGNAL_READ(HORZ_DISP_ON));
//> COUT_DMG 	SIGNAL_WRITE(VIDEO, video);
//> COUT_DMG }
//> COUT_DMG 
///////////////////////////////////////////////////////////////////////////////
//
// internal - pet-lite 'fake' display
//

#undef  SIGNAL_PREFIX
#define SIGNAL_PREFIX		CHIP_LITE_DISPLAY_
#undef  SIGNAL_OWNER
#define SIGNAL_OWNER		chip

//> COUT_DMG enum ChipLiteDisplayPinAssignments {
//> COUT_DMG 	CHIP_LITE_DISPLAY_VIDEO_ON = 0,
//> COUT_DMG 
//> COUT_DMG 	CHIP_LITE_DISPLAY_PIN_COUNT
//> COUT_DMG };
//> COUT_DMG 
//> COUT_DMG static uint8_t ChipLiteDisplay_PinTypes[CHIP_LITE_DISPLAY_PIN_COUNT] = {0};
//> COUT_DMG 
//> COUT_DMG typedef struct ChipLiteDisplay {
//> COUT_DMG 	CHIP_DECLARE_BASE
//> COUT_DMG 
//> COUT_DMG 	DevCommodorePet *	device;
//> COUT_DMG  	Ram8d16a *			vram;
//> COUT_DMG 	uint8_t				char_rom[ROM_6316_DATA_SIZE];
//> COUT_DMG 
//> COUT_DMG 	Signal				signals[CHIP_LITE_DISPLAY_PIN_COUNT];
//> COUT_DMG 	SignalPool *		signal_pool;
//> COUT_DMG 
//> COUT_DMG 	int64_t				refresh_delay;
//> COUT_DMG 	int64_t				retrace_hold;
//> COUT_DMG } ChipLiteDisplay;
//> COUT_DMG 
//> COUT_DMG static void lite_display_destroy(ChipLiteDisplay *chip);
//> COUT_DMG static void lite_display_process(ChipLiteDisplay *chip);
//> COUT_DMG 
//> COUT_DMG static ChipLiteDisplay *lite_display_create(DevCommodorePet *device) {
//> COUT_DMG 	ChipLiteDisplay *chip = (ChipLiteDisplay *) dms_calloc(1, sizeof(ChipLiteDisplay));
//> COUT_DMG 	chip->device = device;
//> COUT_DMG 	chip->signal_pool = device->signal_pool;
//> COUT_DMG 	chip->vram = (Ram8d16a *) simulator_chip_by_name(device->simulator, "VRAM");
//> COUT_DMG 
//> COUT_DMG 	CHIP_SET_FUNCTIONS(chip, lite_display_process, lite_display_destroy);
//> COUT_DMG 	CHIP_SET_VARIABLES(chip, chip->device->simulator, chip->signals, ChipLiteDisplay_PinTypes, CHIP_LITE_DISPLAY_PIN_COUNT);
//> COUT_DMG 
//> COUT_DMG 	GLUE_PIN(VIDEO_ON, CHIP_PIN_INPUT | CHIP_PIN_OUTPUT);
//> COUT_DMG 
//> COUT_DMG 	file_load_binary_fixed("runtime/commodore_pet/characters-2.901447-10.bin", chip->char_rom, ROM_6316_DATA_SIZE);
//> COUT_DMG 
//> COUT_DMG 	chip->refresh_delay = simulator_interval_to_tick_count(device->simulator, FREQUENCY_TO_PS(60));
//> COUT_DMG 	chip->retrace_hold = simulator_interval_to_tick_count(device->simulator, US_TO_PS(1));
//> COUT_DMG 
//> COUT_DMG 	return chip;
//> COUT_DMG }
//> COUT_DMG 
//> COUT_DMG static void lite_display_destroy(ChipLiteDisplay *chip) {
//> COUT_DMG 	assert(chip);
//> COUT_DMG 	dms_free(chip);
//> COUT_DMG }

//> COUT_DMG void pet_lite_fake_display(ChipLiteDisplay *chip) {
//> COUT_DMG 	DevCommodorePet *device = chip->device;
//> COUT_DMG 
//> COUT_DMG 	const uint32_t COLORS[2] = {
//> COUT_DMG 		0xff111111,
//> COUT_DMG 		0xff55ff55
//> COUT_DMG 	};
//> COUT_DMG 
//> COUT_DMG 	const size_t SCREEN_WIDTH = 40;
//> COUT_DMG 	const size_t SCREEN_HEIGHT = 25;
//> COUT_DMG 	const size_t CHAR_HEIGHT = 8;
//> COUT_DMG 
//> COUT_DMG 	for (size_t pos_char_y = 0; pos_char_y < SCREEN_HEIGHT; ++pos_char_y) {
//> COUT_DMG 		uint8_t *vram_row = chip->vram->data_array + (pos_char_y * SCREEN_WIDTH);
//> COUT_DMG 		for (uint8_t pos_line = 0; pos_line < 8; ++pos_line) {
//> COUT_DMG 			uint32_t *screen_ptr = device->screen->frame + ((pos_char_y * CHAR_HEIGHT + pos_line) * device->screen->width);
//> COUT_DMG 
//> COUT_DMG 			for (size_t pos_char_x = 0; pos_char_x < SCREEN_WIDTH; ++pos_char_x) {
//> COUT_DMG 				// get character to display & check for inverted char
//> COUT_DMG 				uint8_t value = vram_row[pos_char_x];
//> COUT_DMG 				unsigned int invert = (value >> 7) & 0x1;
//> COUT_DMG 				value &= 0x7f;
//> COUT_DMG 
//> COUT_DMG 				// get approriate data from the character rom
//> COUT_DMG 				int rom_addr = (int) value << 3 | pos_line;
//> COUT_DMG 				uint8_t line_value = chip->char_rom[rom_addr];
//> COUT_DMG 
//> COUT_DMG 				// write character line to screen
//> COUT_DMG 				for (int i = 7; i >= 0; --i) {
//> COUT_DMG 					*screen_ptr++ = COLORS[((line_value >> i) & 0x1) ^ invert];
//> COUT_DMG 				}
//> COUT_DMG 			}
//> COUT_DMG 		}
//> COUT_DMG 	}
//> COUT_DMG }
//> COUT_DMG 
//> COUT_DMG static void lite_display_process(ChipLiteDisplay *chip) {
//> COUT_DMG 	assert(chip);
//> COUT_DMG 
//> COUT_DMG 	if (SIGNAL_READ(VIDEO_ON)) {
//> COUT_DMG 		// redraw screen
//> COUT_DMG 		pet_lite_fake_display(chip);
//> COUT_DMG 
//> COUT_DMG 		// signal vertical retrace
//> COUT_DMG 		SIGNAL_WRITE(VIDEO_ON, false);
//> COUT_DMG 
//> COUT_DMG 		// hold vertical retrace for a bit
//> COUT_DMG 		chip->schedule_timestamp = chip->simulator->current_tick + chip->retrace_hold;
//> COUT_DMG 	} else {
//> COUT_DMG 		// end of vertical retrace
//> COUT_DMG 		SIGNAL_WRITE(VIDEO_ON, true);
//> COUT_DMG 		chip->schedule_timestamp = chip->simulator->current_tick + chip->refresh_delay;
//> COUT_DMG 	}
//> COUT_DMG }

///////////////////////////////////////////////////////////////////////////////
//
// internal - rom functions
//

#undef  SIGNAL_PREFIX
#define SIGNAL_PREFIX		SIG_P2001N_
#undef  SIGNAL_OWNER
#define SIGNAL_OWNER		device

//> COUT_DMG static Chip63xxRom *load_rom(DevCommodorePet *device, const char *filename, size_t num_lines, Signal rom_cs1_b) {
//> COUT_DMG 
//> COUT_DMG 	Chip63xxSignals signals = {
//> COUT_DMG 		[CHIP_6332_A0] = SIGNAL(BA0),
//> COUT_DMG 		[CHIP_6332_A1] = SIGNAL(BA1),
//> COUT_DMG 		[CHIP_6332_A2] = SIGNAL(BA2),
//> COUT_DMG 		[CHIP_6332_A3] = SIGNAL(BA3),
//> COUT_DMG 		[CHIP_6332_A4] = SIGNAL(BA4),
//> COUT_DMG 		[CHIP_6332_A5] = SIGNAL(BA5),
//> COUT_DMG 		[CHIP_6332_A6] = SIGNAL(BA6),
//> COUT_DMG 		[CHIP_6332_A7] = SIGNAL(BA7),
//> COUT_DMG 		[CHIP_6332_A8] = SIGNAL(BA8),
//> COUT_DMG 		[CHIP_6332_A9] = SIGNAL(BA9),
//> COUT_DMG 		[CHIP_6332_A10] = SIGNAL(BA10),
//> COUT_DMG 		[CHIP_6332_A11] = SIGNAL(BA11),			// used as cs2_b for 6316 rom
//> COUT_DMG 
//> COUT_DMG 		[CHIP_6332_D0] = SIGNAL(D0),
//> COUT_DMG 		[CHIP_6332_D1] = SIGNAL(D1),
//> COUT_DMG 		[CHIP_6332_D2] = SIGNAL(D2),
//> COUT_DMG 		[CHIP_6332_D3] = SIGNAL(D3),
//> COUT_DMG 		[CHIP_6332_D4] = SIGNAL(D4),
//> COUT_DMG 		[CHIP_6332_D5] = SIGNAL(D5),
//> COUT_DMG 		[CHIP_6332_D6] = SIGNAL(D6),
//> COUT_DMG 		[CHIP_6332_D7] = SIGNAL(D7),
//> COUT_DMG 
//> COUT_DMG 		[CHIP_6332_CS1_B] = rom_cs1_b,
//> COUT_DMG 		[CHIP_6332_CS3] = SIGNAL(HIGH)
//> COUT_DMG 	};
//> COUT_DMG 
//> COUT_DMG 	Chip63xxRom *rom = (num_lines == 12) ?
//> COUT_DMG 		chip_6332_rom_create(device->simulator, signals) :
//> COUT_DMG 		chip_6316_rom_create(device->simulator, signals);
//> COUT_DMG 
//> COUT_DMG 	if (file_load_binary_fixed(filename, rom->data_array, (num_lines == 12) ? ROM_6332_DATA_SIZE : ROM_6316_DATA_SIZE) == 0) {
//> COUT_DMG 		rom->destroy(rom);
//> COUT_DMG 		return NULL;
//> COUT_DMG 	}
//> COUT_DMG 
//> COUT_DMG 	return rom;
//> COUT_DMG }
//> COUT_DMG 
//> COUT_DMG static Chip63xxRom *load_character_rom(DevCommodorePet *device, const char *filename) {
//> COUT_DMG 
//> COUT_DMG 	Chip63xxRom *rom = chip_6316_rom_create(device->simulator, (Signal[24]) {
//> COUT_DMG 										[CHIP_6316_CS1_B] = SIGNAL(LOW),
//> COUT_DMG 										[CHIP_6316_CS2_B] = SIGNAL(LOW),
//> COUT_DMG 										[CHIP_6316_CS3] = SIGNAL(INIT_B),
//> COUT_DMG 										[CHIP_6316_A0] = SIGNAL(RA7),
//> COUT_DMG 										[CHIP_6316_A1] = SIGNAL(RA8),
//> COUT_DMG 										[CHIP_6316_A2] = SIGNAL(RA9),
//> COUT_DMG 										[CHIP_6316_A3] = SIGNAL(LSD0),
//> COUT_DMG 										[CHIP_6316_A4] = SIGNAL(LSD1),
//> COUT_DMG 										[CHIP_6316_A5] = SIGNAL(LSD2),
//> COUT_DMG 										[CHIP_6316_A6] = SIGNAL(LSD3),
//> COUT_DMG 										[CHIP_6316_A7] = SIGNAL(LSD4),
//> COUT_DMG 										[CHIP_6316_A8] = SIGNAL(LSD5),
//> COUT_DMG 										[CHIP_6316_A9] = SIGNAL(LSD6),
//> COUT_DMG 										[CHIP_6316_A10] = SIGNAL(GRAPHIC),
//> COUT_DMG 										[CHIP_6316_D0] = SIGNAL(CD0),
//> COUT_DMG 										[CHIP_6316_D1] = SIGNAL(CD1),
//> COUT_DMG 										[CHIP_6316_D2] = SIGNAL(CD2),
//> COUT_DMG 										[CHIP_6316_D3] = SIGNAL(CD3),
//> COUT_DMG 										[CHIP_6316_D4] = SIGNAL(CD4),
//> COUT_DMG 										[CHIP_6316_D5] = SIGNAL(CD5),
//> COUT_DMG 										[CHIP_6316_D6] = SIGNAL(CD6),
//> COUT_DMG 										[CHIP_6316_D7] = SIGNAL(CD7)
//> COUT_DMG 	});
//> COUT_DMG 
//> COUT_DMG 	if (file_load_binary_fixed(filename, rom->data_array, ROM_6316_DATA_SIZE) == 0) {
//> COUT_DMG 		rom->destroy(rom);
//> COUT_DMG 		return NULL;
//> COUT_DMG 	}
//> COUT_DMG 
//> COUT_DMG 	return rom;
//> COUT_DMG }

///////////////////////////////////////////////////////////////////////////////
//
// internal - circuit creation functions
//

// sheet 01: microprocessor & memory expansion
static void circuit_create_01(DevCommodorePet *device) {

	// power-on-reset
	DEVICE_REGISTER_CHIP("POR", poweronreset_create(US_TO_PS(500), device->simulator, (PowerOnResetSignals) {
											[CHIP_POR_TRIGGER_B] = SIGNAL(RESET_BTN_B),
											[CHIP_POR_RESET_B] = SIGNAL(RESET_B)
	}));

	// cpu
	device->cpu = cpu_6502_create(device->simulator, (Cpu6502Signals) {
										[PIN_6502_AB0]  = SIGNAL(AB0),
										[PIN_6502_AB1]  = SIGNAL(AB1),
										[PIN_6502_AB2]  = SIGNAL(AB2),
										[PIN_6502_AB3]  = SIGNAL(AB3),
										[PIN_6502_AB4]  = SIGNAL(AB4),
										[PIN_6502_AB5]  = SIGNAL(AB5),
										[PIN_6502_AB6]  = SIGNAL(AB6),
										[PIN_6502_AB7]  = SIGNAL(AB7),
										[PIN_6502_AB8]  = SIGNAL(AB8),
										[PIN_6502_AB9]  = SIGNAL(AB9),
										[PIN_6502_AB10] = SIGNAL(AB10),
										[PIN_6502_AB11] = SIGNAL(AB11),
										[PIN_6502_AB12] = SIGNAL(AB12),
										[PIN_6502_AB13] = SIGNAL(AB13),
										[PIN_6502_AB14] = SIGNAL(AB14),
										[PIN_6502_AB15] = SIGNAL(AB15),

										[PIN_6502_DB0]  = SIGNAL(D0),
										[PIN_6502_DB1]  = SIGNAL(D1),
										[PIN_6502_DB2]  = SIGNAL(D2),
										[PIN_6502_DB3]  = SIGNAL(D3),
										[PIN_6502_DB4]  = SIGNAL(D4),
										[PIN_6502_DB5]  = SIGNAL(D5),
										[PIN_6502_DB6]  = SIGNAL(D6),
										[PIN_6502_DB7]  = SIGNAL(D7),

										[PIN_6502_CLK]   = SIGNAL(CLK1),
										[PIN_6502_RES_B] = SIGNAL(RESET_B),
										[PIN_6502_RW]	 = SIGNAL(RW),
										[PIN_6502_IRQ_B] = SIGNAL(IRQ_B),
										[PIN_6502_NMI_B] = SIGNAL(NMI_B),
										[PIN_6502_SYNC]  = SIGNAL(SYNC),
										[PIN_6502_RDY]   = SIGNAL(RDY)
	});
	DEVICE_REGISTER_CHIP("C4", device->cpu);

//> COUT_DMG 	// >> c3 - octal buffer
//> COUT_DMG 	DEVICE_REGISTER_CHIP("C3", chip_74244_octal_buffer_create(device->simulator, (Chip74244Signals) {
//> COUT_DMG 										[CHIP_74244_G1_B] = SIGNAL(LOW),
//> COUT_DMG 										[CHIP_74244_G2_B] = SIGNAL(LOW),
//> COUT_DMG 										[CHIP_74244_A11] = SIGNAL(AB0),
//> COUT_DMG 										[CHIP_74244_A24] = SIGNAL(AB1),
//> COUT_DMG 										[CHIP_74244_A12] = SIGNAL(AB2),
//> COUT_DMG 										[CHIP_74244_A23] = SIGNAL(AB3),
//> COUT_DMG 										[CHIP_74244_A13] = SIGNAL(AB4),
//> COUT_DMG 										[CHIP_74244_A22] = SIGNAL(AB5),
//> COUT_DMG 										[CHIP_74244_A14] = SIGNAL(AB6),
//> COUT_DMG 										[CHIP_74244_A21] = SIGNAL(AB7),
//> COUT_DMG 										[CHIP_74244_Y11] = SIGNAL(BA0),
//> COUT_DMG 										[CHIP_74244_Y24] = SIGNAL(BA1),
//> COUT_DMG 										[CHIP_74244_Y12] = SIGNAL(BA2),
//> COUT_DMG 										[CHIP_74244_Y23] = SIGNAL(BA3),
//> COUT_DMG 										[CHIP_74244_Y13] = SIGNAL(BA4),
//> COUT_DMG 										[CHIP_74244_Y22] = SIGNAL(BA5),
//> COUT_DMG 										[CHIP_74244_Y14] = SIGNAL(BA6),
//> COUT_DMG 										[CHIP_74244_Y21] = SIGNAL(BA7),
//> COUT_DMG 
//> COUT_DMG 	}));
//> COUT_DMG 
//> COUT_DMG 	// >> b3 - octal buffer
//> COUT_DMG 	DEVICE_REGISTER_CHIP("B3", chip_74244_octal_buffer_create(device->simulator, (Chip74244Signals) {
//> COUT_DMG 										[CHIP_74244_G1_B] = SIGNAL(LOW),	// 01
//> COUT_DMG 										[CHIP_74244_G2_B] = SIGNAL(LOW),	// 19
//> COUT_DMG 										[CHIP_74244_A11]  = SIGNAL(AB8),	// 02
//> COUT_DMG 										[CHIP_74244_A24]  = SIGNAL(AB9),	// 17
//> COUT_DMG 										[CHIP_74244_A12]  = SIGNAL(AB10),	// 04
//> COUT_DMG 										[CHIP_74244_A23]  = SIGNAL(AB11),	// 15
//> COUT_DMG 										[CHIP_74244_A13]  = SIGNAL(AB12),	// 06
//> COUT_DMG 										[CHIP_74244_A22]  = SIGNAL(AB13),	// 13
//> COUT_DMG 										[CHIP_74244_A14]  = SIGNAL(AB14),	// 08
//> COUT_DMG 										[CHIP_74244_A21]  = SIGNAL(AB15),	// 11
//> COUT_DMG 										[CHIP_74244_Y11]  = SIGNAL(BA8),	// 18
//> COUT_DMG 										[CHIP_74244_Y24]  = SIGNAL(BA9),	// 03
//> COUT_DMG 										[CHIP_74244_Y12]  = SIGNAL(BA10),	// 16
//> COUT_DMG 										[CHIP_74244_Y23]  = SIGNAL(BA11),	// 05
//> COUT_DMG 										[CHIP_74244_Y13]  = SIGNAL(BA12),	// 14
//> COUT_DMG 										[CHIP_74244_Y22]  = SIGNAL(BA13),	// 07
//> COUT_DMG 										[CHIP_74244_Y14]  = SIGNAL(BA14),	// 12
//> COUT_DMG 										[CHIP_74244_Y21]  = SIGNAL(BA15),	// 09
//> COUT_DMG 
//> COUT_DMG 	}));
//> COUT_DMG 
//> COUT_DMG 	// >> d2 - 4-to-16 decoder
//> COUT_DMG 	DEVICE_REGISTER_CHIP("D2", chip_74154_decoder_create(device->simulator, (Chip74154Signals) {
//> COUT_DMG 										[CHIP_74154_G1_B] = SIGNAL(LOW),
//> COUT_DMG 										[CHIP_74154_G2_B] = SIGNAL(LOW),
//> COUT_DMG 										[CHIP_74154_A] = SIGNAL(BA12),
//> COUT_DMG 										[CHIP_74154_B] = SIGNAL(BA13),
//> COUT_DMG 										[CHIP_74154_C] = SIGNAL(BA14),
//> COUT_DMG 										[CHIP_74154_D] = SIGNAL(BA15),
//> COUT_DMG 										[CHIP_74154_Y0_B] = SIGNAL(SEL0_B),
//> COUT_DMG 										[CHIP_74154_Y1_B] = SIGNAL(SEL1_B),
//> COUT_DMG 										[CHIP_74154_Y2_B] = SIGNAL(SEL2_B),
//> COUT_DMG 										[CHIP_74154_Y3_B] = SIGNAL(SEL3_B),
//> COUT_DMG 										[CHIP_74154_Y4_B] = SIGNAL(SEL4_B),
//> COUT_DMG 										[CHIP_74154_Y5_B] = SIGNAL(SEL5_B),
//> COUT_DMG 										[CHIP_74154_Y6_B] = SIGNAL(SEL6_B),
//> COUT_DMG 										[CHIP_74154_Y7_B] = SIGNAL(SEL7_B),
//> COUT_DMG 										[CHIP_74154_Y8_B] = SIGNAL(SEL8_B),
//> COUT_DMG 										[CHIP_74154_Y9_B] = SIGNAL(SEL9_B),
//> COUT_DMG 										[CHIP_74154_Y10_B] = SIGNAL(SELA_B),
//> COUT_DMG 										[CHIP_74154_Y11_B] = SIGNAL(SELB_B),
//> COUT_DMG 										[CHIP_74154_Y12_B] = SIGNAL(SELC_B),
//> COUT_DMG 										[CHIP_74154_Y13_B] = SIGNAL(SELD_B),
//> COUT_DMG 										[CHIP_74154_Y14_B] = SIGNAL(SELE_B),
//> COUT_DMG 										[CHIP_74154_Y15_B] = SIGNAL(SELF_B),
//> COUT_DMG 	}));
//> COUT_DMG 
//> COUT_DMG 	// >> e9 - octal buffer
//> COUT_DMG 	DEVICE_REGISTER_CHIP("E9", chip_74244_octal_buffer_create(device->simulator, (Chip74244Signals) {
//> COUT_DMG 										[CHIP_74244_G1_B] = SIGNAL(RAMW_B),	// 01
//> COUT_DMG 										[CHIP_74244_G2_B] = SIGNAL(RAMR_B),	// 19
//> COUT_DMG 										[CHIP_74244_A11]  = SIGNAL(D0),		// 02
//> COUT_DMG 										[CHIP_74244_Y24]  = SIGNAL(D0),		// 03
//> COUT_DMG 										[CHIP_74244_A12]  = SIGNAL(D1),		// 04
//> COUT_DMG 										[CHIP_74244_Y23]  = SIGNAL(D1),		// 05
//> COUT_DMG 										[CHIP_74244_A13]  = SIGNAL(D2),		// 06
//> COUT_DMG 										[CHIP_74244_Y22]  = SIGNAL(D2),		// 07
//> COUT_DMG 										[CHIP_74244_A14]  = SIGNAL(D3),		// 08
//> COUT_DMG 										[CHIP_74244_Y21]  = SIGNAL(D3),		// 09
//> COUT_DMG 
//> COUT_DMG 										[CHIP_74244_Y11]  = SIGNAL(BD0),	// 18
//> COUT_DMG 										[CHIP_74244_A24]  = SIGNAL(BD0),	// 17
//> COUT_DMG 										[CHIP_74244_Y12]  = SIGNAL(BD1),	// 16
//> COUT_DMG 										[CHIP_74244_A23]  = SIGNAL(BD1),	// 15
//> COUT_DMG 										[CHIP_74244_Y13]  = SIGNAL(BD2),	// 14
//> COUT_DMG 										[CHIP_74244_A22]  = SIGNAL(BD2),	// 13
//> COUT_DMG 										[CHIP_74244_Y14]  = SIGNAL(BD3),	// 12
//> COUT_DMG 										[CHIP_74244_A21]  = SIGNAL(BD3)		// 11
//> COUT_DMG 	}));
//> COUT_DMG 
//> COUT_DMG 	// >> e10 - octal buffer
//> COUT_DMG 	DEVICE_REGISTER_CHIP("E10", chip_74244_octal_buffer_create(device->simulator, (Chip74244Signals) {
//> COUT_DMG 										[CHIP_74244_G1_B] = SIGNAL(RAMW_B),	// 01
//> COUT_DMG 										[CHIP_74244_G2_B] = SIGNAL(RAMR_B),	// 19
//> COUT_DMG 										[CHIP_74244_A11]  = SIGNAL(D4),		// 02
//> COUT_DMG 										[CHIP_74244_Y24]  = SIGNAL(D4),		// 03
//> COUT_DMG 										[CHIP_74244_A12]  = SIGNAL(D5),		// 04
//> COUT_DMG 										[CHIP_74244_Y23]  = SIGNAL(D5),		// 05
//> COUT_DMG 										[CHIP_74244_A13]  = SIGNAL(D6),		// 06
//> COUT_DMG 										[CHIP_74244_Y22]  = SIGNAL(D6),		// 07
//> COUT_DMG 										[CHIP_74244_A14]  = SIGNAL(D7),		// 08
//> COUT_DMG 										[CHIP_74244_Y21]  = SIGNAL(D7),		// 09
//> COUT_DMG 
//> COUT_DMG 										[CHIP_74244_Y11]  = SIGNAL(BD4),	// 18
//> COUT_DMG 										[CHIP_74244_A24]  = SIGNAL(BD4),	// 17
//> COUT_DMG 										[CHIP_74244_Y12]  = SIGNAL(BD5),	// 16
//> COUT_DMG 										[CHIP_74244_A23]  = SIGNAL(BD5),	// 15
//> COUT_DMG 										[CHIP_74244_Y13]  = SIGNAL(BD6),	// 14
//> COUT_DMG 										[CHIP_74244_A22]  = SIGNAL(BD6),	// 13
//> COUT_DMG 										[CHIP_74244_Y14]  = SIGNAL(BD7),	// 12
//> COUT_DMG 										[CHIP_74244_A21]  = SIGNAL(BD7)		// 11
//> COUT_DMG 	}));
//> COUT_DMG 
//> COUT_DMG 	// glue-logic
//> COUT_DMG 	DEVICE_REGISTER_CHIP("LOGIC1", glue_logic_create_01(device));
}
//> COUT_DMG 
//> COUT_DMG // sheet 02: IEEE-488 Interface
//> COUT_DMG void circuit_create_02(DevCommodorePet *device) {
//> COUT_DMG 
//> COUT_DMG 	// pia 1 (C6 - IEEE-488 interface)
//> COUT_DMG 	device->pia_1 = chip_6520_create(device->simulator, (Chip6520Signals) {
//> COUT_DMG 										[CHIP_6520_D0] = SIGNAL(D0),
//> COUT_DMG 										[CHIP_6520_D1] = SIGNAL(D1),
//> COUT_DMG 										[CHIP_6520_D2] = SIGNAL(D2),
//> COUT_DMG 										[CHIP_6520_D3] = SIGNAL(D3),
//> COUT_DMG 										[CHIP_6520_D4] = SIGNAL(D4),
//> COUT_DMG 										[CHIP_6520_D5] = SIGNAL(D5),
//> COUT_DMG 										[CHIP_6520_D6] = SIGNAL(D6),
//> COUT_DMG 										[CHIP_6520_D7] = SIGNAL(D7),
//> COUT_DMG 										[CHIP_6520_PHI2] = SIGNAL(CLK1),
//> COUT_DMG 										[CHIP_6520_RESET_B] = SIGNAL(RESET_B),
//> COUT_DMG 										[CHIP_6520_RW] = SIGNAL(RW),
//> COUT_DMG 										[CHIP_6520_CS0] = SIGNAL(X8XX),
//> COUT_DMG 										[CHIP_6520_CS1] = SIGNAL(BA5),
//> COUT_DMG 										[CHIP_6520_CS2_B] = SIGNAL(SELE_B),							// io_b on schematic (jumpered to SELE_B)
//> COUT_DMG 										[CHIP_6520_RS0] = SIGNAL(BA0),
//> COUT_DMG 										[CHIP_6520_RS1] = SIGNAL(BA1),
//> COUT_DMG 										[CHIP_6520_CA1] = SIGNAL(ATN_IN_B),
//> COUT_DMG 										[CHIP_6520_CA2] = SIGNAL(NDAC_OUT_B),
//> COUT_DMG 										[CHIP_6520_PA0] = SIGNAL(DI0),
//> COUT_DMG 										[CHIP_6520_PA1] = SIGNAL(DI1),
//> COUT_DMG 										[CHIP_6520_PA2] = SIGNAL(DI2),
//> COUT_DMG 										[CHIP_6520_PA3] = SIGNAL(DI3),
//> COUT_DMG 										[CHIP_6520_PA4] = SIGNAL(DI4),
//> COUT_DMG 										[CHIP_6520_PA5] = SIGNAL(DI5),
//> COUT_DMG 										[CHIP_6520_PA6] = SIGNAL(DI6),
//> COUT_DMG 										[CHIP_6520_PA7] = SIGNAL(DI7),
//> COUT_DMG 										[CHIP_6520_PB0] = SIGNAL(DO0),
//> COUT_DMG 										[CHIP_6520_PB1] = SIGNAL(DO1),
//> COUT_DMG 										[CHIP_6520_PB2] = SIGNAL(DO2),
//> COUT_DMG 										[CHIP_6520_PB3] = SIGNAL(DO3),
//> COUT_DMG 										[CHIP_6520_PB4] = SIGNAL(DO4),
//> COUT_DMG 										[CHIP_6520_PB5] = SIGNAL(DO5),
//> COUT_DMG 										[CHIP_6520_PB6] = SIGNAL(DO6),
//> COUT_DMG 										[CHIP_6520_PB7] = SIGNAL(DO7),
//> COUT_DMG 										[CHIP_6520_CB1] = SIGNAL(SRQ_IN_B),
//> COUT_DMG 										[CHIP_6520_CB2] = SIGNAL(DAV_OUT_B),
//> COUT_DMG 										[CHIP_6520_IRQA_B] = SIGNAL(IRQ_B),
//> COUT_DMG 										[CHIP_6520_IRQB_B] = SIGNAL(IRQ_B)
//> COUT_DMG 	});
//> COUT_DMG 	DEVICE_REGISTER_CHIP("C6", device->pia_1);
//> COUT_DMG 
//> COUT_DMG 	// >> A9 - bus transceiver
//> COUT_DMG 	DEVICE_REGISTER_CHIP("A9", chip_mc3446a_create(device->simulator, (ChipMC3446ASignals) {
//> COUT_DMG 										[CHIP_MC3446A_AO] = SIGNAL(DAV_IN_B),		// 01
//> COUT_DMG 										[CHIP_MC3446A_AB] = SIGNAL(DAV_B),			// 02
//> COUT_DMG 										[CHIP_MC3446A_AI] = SIGNAL(DAV_OUT_B),		// 03
//> COUT_DMG 										[CHIP_MC3446A_ABCE_B] = SIGNAL(LOW),		// 04
//> COUT_DMG 										[CHIP_MC3446A_BI] = SIGNAL(NRFD_OUT_B),		// 05
//> COUT_DMG 										[CHIP_MC3446A_BB] = SIGNAL(NRFD_B),			// 06
//> COUT_DMG 										[CHIP_MC3446A_BO] = SIGNAL(NRFD_IN_B),		// 07
//> COUT_DMG 										[CHIP_MC3446A_CO] = SIGNAL(NDAC_IN_B),		// 09
//> COUT_DMG 										[CHIP_MC3446A_CB] = SIGNAL(NDAC_B),			// 10
//> COUT_DMG 										[CHIP_MC3446A_CI] = SIGNAL(NDAC_OUT_B),		// 11
//> COUT_DMG 										[CHIP_MC3446A_DE_B] = SIGNAL(LOW),			// 12
//> COUT_DMG 										[CHIP_MC3446A_DI] = SIGNAL(ATN_OUT_B),		// 13
//> COUT_DMG 										[CHIP_MC3446A_DB] = SIGNAL(ATN_B),			// 14
//> COUT_DMG 										[CHIP_MC3446A_DO] = SIGNAL(ATN_IN_B)		// 15
//> COUT_DMG 	}));
//> COUT_DMG 
//> COUT_DMG 	// >> A8 - bus transceiver
//> COUT_DMG 	DEVICE_REGISTER_CHIP("A8", chip_mc3446a_create(device->simulator, (ChipMC3446ASignals) {
//> COUT_DMG 										[CHIP_MC3446A_AO] = SIGNAL(DI0),			// 01
//> COUT_DMG 										[CHIP_MC3446A_AB] = SIGNAL(DIO0),			// 02
//> COUT_DMG 										[CHIP_MC3446A_AI] = SIGNAL(DO0),			// 03
//> COUT_DMG 										[CHIP_MC3446A_ABCE_B] = SIGNAL(LOW),		// 04
//> COUT_DMG 										[CHIP_MC3446A_BI] = SIGNAL(DO1),			// 05
//> COUT_DMG 										[CHIP_MC3446A_BB] = SIGNAL(DIO1),			// 06
//> COUT_DMG 										[CHIP_MC3446A_BO] = SIGNAL(DI1),			// 07
//> COUT_DMG 										[CHIP_MC3446A_CO] = SIGNAL(DI2),			// 09
//> COUT_DMG 										[CHIP_MC3446A_CB] = SIGNAL(DIO2),			// 10
//> COUT_DMG 										[CHIP_MC3446A_CI] = SIGNAL(DO2),			// 11
//> COUT_DMG 										[CHIP_MC3446A_DE_B] = SIGNAL(LOW),			// 12
//> COUT_DMG 										[CHIP_MC3446A_DI] = SIGNAL(DO3),			// 13
//> COUT_DMG 										[CHIP_MC3446A_DB] = SIGNAL(DIO3),			// 14
//> COUT_DMG 										[CHIP_MC3446A_DO] = SIGNAL(DI3)				// 15
//> COUT_DMG 	}));
//> COUT_DMG 
//> COUT_DMG 	// >> A7 - bus transceiver
//> COUT_DMG 	DEVICE_REGISTER_CHIP("A8", chip_mc3446a_create(device->simulator, (ChipMC3446ASignals) {
//> COUT_DMG 										[CHIP_MC3446A_AO] = SIGNAL(DI4),			// 01
//> COUT_DMG 										[CHIP_MC3446A_AB] = SIGNAL(DIO4),			// 02
//> COUT_DMG 										[CHIP_MC3446A_AI] = SIGNAL(DO4),			// 03
//> COUT_DMG 										[CHIP_MC3446A_ABCE_B] = SIGNAL(LOW),		// 04
//> COUT_DMG 										[CHIP_MC3446A_BI] = SIGNAL(DO5),			// 05
//> COUT_DMG 										[CHIP_MC3446A_BB] = SIGNAL(DIO5),			// 06
//> COUT_DMG 										[CHIP_MC3446A_BO] = SIGNAL(DI5),			// 07
//> COUT_DMG 										[CHIP_MC3446A_CO] = SIGNAL(DI6),			// 09
//> COUT_DMG 										[CHIP_MC3446A_CB] = SIGNAL(DIO6),			// 10
//> COUT_DMG 										[CHIP_MC3446A_CI] = SIGNAL(DO6),			// 11
//> COUT_DMG 										[CHIP_MC3446A_DE_B] = SIGNAL(LOW),			// 12
//> COUT_DMG 										[CHIP_MC3446A_DI] = SIGNAL(DO7),			// 13
//> COUT_DMG 										[CHIP_MC3446A_DB] = SIGNAL(DIO7),			// 14
//> COUT_DMG 										[CHIP_MC3446A_DO] = SIGNAL(DI7)				// 15
//> COUT_DMG 	}));
//> COUT_DMG }
//> COUT_DMG 
//> COUT_DMG // sheet 03: cassette & keyboard
//> COUT_DMG void circuit_create_03(DevCommodorePet *device) {
//> COUT_DMG 
//> COUT_DMG 	// pia 2 (C7 - keyboard)
//> COUT_DMG 	device->pia_2 = chip_6520_create(device->simulator, (Chip6520Signals) {
//> COUT_DMG 										[CHIP_6520_D0] = SIGNAL(D0),
//> COUT_DMG 										[CHIP_6520_D1] = SIGNAL(D1),
//> COUT_DMG 										[CHIP_6520_D2] = SIGNAL(D2),
//> COUT_DMG 										[CHIP_6520_D3] = SIGNAL(D3),
//> COUT_DMG 										[CHIP_6520_D4] = SIGNAL(D4),
//> COUT_DMG 										[CHIP_6520_D5] = SIGNAL(D5),
//> COUT_DMG 										[CHIP_6520_D6] = SIGNAL(D6),
//> COUT_DMG 										[CHIP_6520_D7] = SIGNAL(D7),
//> COUT_DMG 										[CHIP_6520_PHI2] = SIGNAL(CLK1),
//> COUT_DMG 										[CHIP_6520_RESET_B] = SIGNAL(RESET_B),
//> COUT_DMG 										[CHIP_6520_RW] = SIGNAL(RW),
//> COUT_DMG 										[CHIP_6520_CS0] = SIGNAL(X8XX),
//> COUT_DMG 										[CHIP_6520_CS1] = SIGNAL(BA4),
//> COUT_DMG 										[CHIP_6520_CS2_B] = SIGNAL(SELE_B),							// io_b on schematic (jumpered to SELE_B)
//> COUT_DMG 										[CHIP_6520_RS0] = SIGNAL(BA0),
//> COUT_DMG 										[CHIP_6520_RS1] = SIGNAL(BA1),
//> COUT_DMG 										[CHIP_6520_CA1] = SIGNAL(CASS_READ_1),
//> COUT_DMG 										[CHIP_6520_CA2] = SIGNAL(EOI_OUT_B),
//> COUT_DMG 										[CHIP_6520_PA0] = SIGNAL(KEYA),
//> COUT_DMG 										[CHIP_6520_PA1] = SIGNAL(KEYB),
//> COUT_DMG 										[CHIP_6520_PA2] = SIGNAL(KEYC),
//> COUT_DMG 										[CHIP_6520_PA3] = SIGNAL(KEYD),
//> COUT_DMG 										[CHIP_6520_PA4] = SIGNAL(CASS_SWITCH_1),
//> COUT_DMG 										[CHIP_6520_PA5] = SIGNAL(CASS_SWITCH_2),
//> COUT_DMG 										[CHIP_6520_PA6] = SIGNAL(EOI_IN_B),
//> COUT_DMG 										[CHIP_6520_PA7] = SIGNAL(DIAG),
//> COUT_DMG 										[CHIP_6520_PB0] = SIGNAL(KIN0),
//> COUT_DMG 										[CHIP_6520_PB1] = SIGNAL(KIN1),
//> COUT_DMG 										[CHIP_6520_PB2] = SIGNAL(KIN2),
//> COUT_DMG 										[CHIP_6520_PB3] = SIGNAL(KIN3),
//> COUT_DMG 										[CHIP_6520_PB4] = SIGNAL(KIN4),
//> COUT_DMG 										[CHIP_6520_PB5] = SIGNAL(KIN5),
//> COUT_DMG 										[CHIP_6520_PB6] = SIGNAL(KIN6),
//> COUT_DMG 										[CHIP_6520_PB7] = SIGNAL(KIN7),
//> COUT_DMG 										[CHIP_6520_CB1] = SIGNAL(VIDEO_ON),
//> COUT_DMG 										[CHIP_6520_CB2] = SIGNAL(CASS_MOTOR_1_B),
//> COUT_DMG 										[CHIP_6520_IRQA_B] = SIGNAL(IRQ_B),
//> COUT_DMG 										[CHIP_6520_IRQB_B] = SIGNAL(IRQ_B)
//> COUT_DMG 	});
//> COUT_DMG 	DEVICE_REGISTER_CHIP("C7", device->pia_2);
//> COUT_DMG 
//> COUT_DMG 	// via (C5)
//> COUT_DMG 	device->via = chip_6522_create(device->simulator, (Chip6522Signals) {
//> COUT_DMG 										[CHIP_6522_D0] = SIGNAL(D0),
//> COUT_DMG 										[CHIP_6522_D1] = SIGNAL(D1),
//> COUT_DMG 										[CHIP_6522_D2] = SIGNAL(D2),
//> COUT_DMG 										[CHIP_6522_D3] = SIGNAL(D3),
//> COUT_DMG 										[CHIP_6522_D4] = SIGNAL(D4),
//> COUT_DMG 										[CHIP_6522_D5] = SIGNAL(D5),
//> COUT_DMG 										[CHIP_6522_D6] = SIGNAL(D6),
//> COUT_DMG 										[CHIP_6522_D7] = SIGNAL(D7),
//> COUT_DMG 										[CHIP_6522_PHI2] = SIGNAL(CLK1),
//> COUT_DMG 										[CHIP_6522_RESET_B] = SIGNAL(RESET_B),
//> COUT_DMG 										[CHIP_6522_RW] = SIGNAL(RW),
//> COUT_DMG 										[CHIP_6522_CS1] = SIGNAL(CS1),
//> COUT_DMG 										[CHIP_6522_CS2_B] = SIGNAL(SELE_B),							// io_b on schematic (jumpered to SELE_B)
//> COUT_DMG 										[CHIP_6522_RS0] = SIGNAL(BA0),
//> COUT_DMG 										[CHIP_6522_RS1] = SIGNAL(BA1),
//> COUT_DMG 										[CHIP_6522_RS2] = SIGNAL(BA2),
//> COUT_DMG 										[CHIP_6522_RS3] = SIGNAL(BA3),
//> COUT_DMG 										[CHIP_6522_CA1] = SIGNAL(CA1),
//> COUT_DMG 										[CHIP_6522_CA2] = SIGNAL(GRAPHIC),
//> COUT_DMG 										[CHIP_6522_PA0] = SIGNAL(PA0),
//> COUT_DMG 										[CHIP_6522_PA1] = SIGNAL(PA1),
//> COUT_DMG 										[CHIP_6522_PA2] = SIGNAL(PA2),
//> COUT_DMG 										[CHIP_6522_PA3] = SIGNAL(PA3),
//> COUT_DMG 										[CHIP_6522_PA4] = SIGNAL(PA4),
//> COUT_DMG 										[CHIP_6522_PA5] = SIGNAL(PA5),
//> COUT_DMG 										[CHIP_6522_PA6] = SIGNAL(PA6),
//> COUT_DMG 										[CHIP_6522_PA7] = SIGNAL(PA7),
//> COUT_DMG 										[CHIP_6522_PB0] = SIGNAL(NDAC_IN_B),
//> COUT_DMG 										[CHIP_6522_PB1] = SIGNAL(NRFD_OUT_B),
//> COUT_DMG 										[CHIP_6522_PB2] = SIGNAL(ATN_OUT_B),
//> COUT_DMG 										[CHIP_6522_PB3] = SIGNAL(CASS_WRITE),
//> COUT_DMG 										[CHIP_6522_PB4] = SIGNAL(CASS_MOTOR_2_B),
//> COUT_DMG 										[CHIP_6522_PB5] = SIGNAL(VIDEO_ON),
//> COUT_DMG 										[CHIP_6522_PB6] = SIGNAL(NRFD_IN_B),
//> COUT_DMG 										[CHIP_6522_PB7] = SIGNAL(DAV_IN_B),
//> COUT_DMG 										[CHIP_6522_CB1] = SIGNAL(CASS_READ_2),
//> COUT_DMG 										[CHIP_6522_CB2] = SIGNAL(CB2),
//> COUT_DMG 										[CHIP_6522_IRQ_B] = SIGNAL(IRQ_B)
//> COUT_DMG 	});
//> COUT_DMG 	DEVICE_REGISTER_CHIP("C5", device->via);
//> COUT_DMG 
//> COUT_DMG 	// >> c9 - bcd decoder
//> COUT_DMG 	DEVICE_REGISTER_CHIP("C9", chip_74145_bcd_decoder_create(device->simulator, (Chip74145Signals) {
//> COUT_DMG 										[CHIP_74145_A] = SIGNAL(KEYA),
//> COUT_DMG 										[CHIP_74145_B] = SIGNAL(KEYB),
//> COUT_DMG 										[CHIP_74145_C] = SIGNAL(KEYC),
//> COUT_DMG 										[CHIP_74145_D] = SIGNAL(KEYD),
//> COUT_DMG 										[CHIP_74145_Y0_B] = SIGNAL(KOUT0),
//> COUT_DMG 										[CHIP_74145_Y1_B] = SIGNAL(KOUT1),
//> COUT_DMG 										[CHIP_74145_Y2_B] = SIGNAL(KOUT2),
//> COUT_DMG 										[CHIP_74145_Y3_B] = SIGNAL(KOUT3),
//> COUT_DMG 										[CHIP_74145_Y4_B] = SIGNAL(KOUT4),
//> COUT_DMG 										[CHIP_74145_Y5_B] = SIGNAL(KOUT5),
//> COUT_DMG 										[CHIP_74145_Y6_B] = SIGNAL(KOUT6),
//> COUT_DMG 										[CHIP_74145_Y7_B] = SIGNAL(KOUT7),
//> COUT_DMG 										[CHIP_74145_Y8_B] = SIGNAL(KOUT8),
//> COUT_DMG 										[CHIP_74145_Y9_B] = SIGNAL(KOUT9)
//> COUT_DMG 	}));
//> COUT_DMG 
//> COUT_DMG 	// glue-logic
//> COUT_DMG 	DEVICE_REGISTER_CHIP("LOGIC3", glue_logic_create_03(device));
//> COUT_DMG }
//> COUT_DMG 
//> COUT_DMG 
//> COUT_DMG // sheet 04: roms
//> COUT_DMG void circuit_create_04(DevCommodorePet *device) {
//> COUT_DMG 
//> COUT_DMG 	int rom_count = 0;
//> COUT_DMG 	device->roms[rom_count++] = load_rom(device, "runtime/commodore_pet/basic-4-b000.901465-19.bin", 12, SIGNAL(SELB_B));
//> COUT_DMG 	device->roms[rom_count++] = load_rom(device, "runtime/commodore_pet/basic-4-c000.901465-20.bin", 12, SIGNAL(SELC_B));
//> COUT_DMG 	device->roms[rom_count++] = load_rom(device, "runtime/commodore_pet/basic-4-d000.901465-21.bin", 12, SIGNAL(SELD_B));
//> COUT_DMG 	device->roms[rom_count++] = load_rom(device, "runtime/commodore_pet/edit-4-n.901447-29.bin", 11, SIGNAL(SELE_B));
//> COUT_DMG 	device->roms[rom_count++] = load_rom(device, "runtime/commodore_pet/kernal-4.901465-22.bin", 12, SIGNAL(SELF_B));
//> COUT_DMG 
//> COUT_DMG 	for (int i = 0; i < rom_count; ++i) {
//> COUT_DMG 		assert(device->roms[i]);
//> COUT_DMG 		DEVICE_REGISTER_CHIP("ROM", device->roms[i]);
//> COUT_DMG 	}
//> COUT_DMG }
//> COUT_DMG 
//> COUT_DMG // sheet 05: RAMS
//> COUT_DMG void circuit_create_05(DevCommodorePet *device) {
//> COUT_DMG 
//> COUT_DMG 	DEVICE_REGISTER_CHIP("I11", chip_74244_octal_buffer_create(device->simulator, (Chip74244Signals) {
//> COUT_DMG 										[CHIP_74244_G2_B] = SIGNAL(BRW),	// 19
//> COUT_DMG 										[CHIP_74244_G1_B] = SIGNAL(G7_8),	// 01
//> COUT_DMG 
//> COUT_DMG 										[CHIP_74244_A11] = SIGNAL(RD7),		// 02
//> COUT_DMG 										[CHIP_74244_Y24] = SIGNAL(RD7),		// 03
//> COUT_DMG 										[CHIP_74244_A12] = SIGNAL(RD6),		// 04
//> COUT_DMG 										[CHIP_74244_Y23] = SIGNAL(RD6),		// 05
//> COUT_DMG 										[CHIP_74244_A13] = SIGNAL(RD5),		// 06
//> COUT_DMG 										[CHIP_74244_Y22] = SIGNAL(RD5),		// 07
//> COUT_DMG 										[CHIP_74244_A14] = SIGNAL(RD4),		// 08
//> COUT_DMG 										[CHIP_74244_Y21] = SIGNAL(RD4),		// 09
//> COUT_DMG 
//> COUT_DMG 										[CHIP_74244_Y11] = SIGNAL(BD7),		// 18
//> COUT_DMG 										[CHIP_74244_A24] = SIGNAL(BD7),		// 17
//> COUT_DMG 										[CHIP_74244_Y12] = SIGNAL(BD6),		// 16
//> COUT_DMG 										[CHIP_74244_A23] = SIGNAL(BD6),		// 15
//> COUT_DMG 										[CHIP_74244_Y13] = SIGNAL(BD5),		// 14
//> COUT_DMG 										[CHIP_74244_A22] = SIGNAL(BD5),		// 13
//> COUT_DMG 										[CHIP_74244_Y14] = SIGNAL(BD4),		// 12
//> COUT_DMG 										[CHIP_74244_A21] = SIGNAL(BD4)		// 11
//> COUT_DMG 	}));
//> COUT_DMG 
//> COUT_DMG 	DEVICE_REGISTER_CHIP("I10", chip_74244_octal_buffer_create(device->simulator, (Chip74244Signals) {
//> COUT_DMG 										[CHIP_74244_G2_B] = SIGNAL(BRW),	// 19
//> COUT_DMG 										[CHIP_74244_G1_B] = SIGNAL(G7_8),	// 01
//> COUT_DMG 
//> COUT_DMG 										[CHIP_74244_A11]  = SIGNAL(RD3),	// 02
//> COUT_DMG 										[CHIP_74244_Y24]  = SIGNAL(RD3),	// 03
//> COUT_DMG 										[CHIP_74244_A12]  = SIGNAL(RD2),	// 04
//> COUT_DMG 										[CHIP_74244_Y23]  = SIGNAL(RD2),	// 05
//> COUT_DMG 										[CHIP_74244_A13]  = SIGNAL(RD1),	// 06
//> COUT_DMG 										[CHIP_74244_Y22]  = SIGNAL(RD1),	// 07
//> COUT_DMG 										[CHIP_74244_A14]  = SIGNAL(RD0),	// 08
//> COUT_DMG 										[CHIP_74244_Y21]  = SIGNAL(RD0),	// 09
//> COUT_DMG 
//> COUT_DMG 										[CHIP_74244_Y11]  = SIGNAL(BD3),	// 18
//> COUT_DMG 										[CHIP_74244_A24]  = SIGNAL(BD3),	// 17
//> COUT_DMG 										[CHIP_74244_Y12]  = SIGNAL(BD2),	// 16
//> COUT_DMG 										[CHIP_74244_A23]  = SIGNAL(BD2),	// 15
//> COUT_DMG 										[CHIP_74244_Y13]  = SIGNAL(BD1),	// 14
//> COUT_DMG 										[CHIP_74244_A22]  = SIGNAL(BD1),	// 13
//> COUT_DMG 										[CHIP_74244_Y14]  = SIGNAL(BD0),	// 12
//> COUT_DMG 										[CHIP_74244_A21]  = SIGNAL(BD0)		// 11
//> COUT_DMG 	}));
//> COUT_DMG 
//> COUT_DMG 	DEVICE_REGISTER_CHIP("E3", chip_74153_multiplexer_create(device->simulator, (Chip74153Signals) {
//> COUT_DMG 										[CHIP_74153_G1] = SIGNAL(LOW),		// pin 1
//> COUT_DMG 										[CHIP_74153_C10] = SIGNAL(RA7),		// pin 6
//> COUT_DMG 										[CHIP_74153_C11] = SIGNAL(RA7),		// pin 5
//> COUT_DMG 										[CHIP_74153_C12] = SIGNAL(BA0),		// pin 4
//> COUT_DMG 										[CHIP_74153_C13] = SIGNAL(BA13),	// pin 3
//> COUT_DMG 										[CHIP_74153_Y1] = SIGNAL(FA0),		// pin 7
//> COUT_DMG 
//> COUT_DMG 										[CHIP_74153_A] = SIGNAL(MUXA),		// pin 14
//> COUT_DMG 										[CHIP_74153_B] = SIGNAL(CLK1),		// pin 2
//> COUT_DMG 
//> COUT_DMG 										[CHIP_74153_G2] = SIGNAL(LOW),		// pin 15
//> COUT_DMG 										[CHIP_74153_C20] = SIGNAL(RA1),		// pin 10
//> COUT_DMG 										[CHIP_74153_C21] = SIGNAL(RA1),		// pin 11
//> COUT_DMG 										[CHIP_74153_C22] = SIGNAL(BA1),		// pin 12
//> COUT_DMG 										[CHIP_74153_C23] = SIGNAL(BA7),		// pin 13
//> COUT_DMG 										[CHIP_74153_Y2] = SIGNAL(FA1),		// pin 9
//> COUT_DMG 	}));
//> COUT_DMG 
//> COUT_DMG 	DEVICE_REGISTER_CHIP("E4", chip_74153_multiplexer_create(device->simulator, (Chip74153Signals) {
//> COUT_DMG 										[CHIP_74153_G1] = SIGNAL(LOW),		// pin 1
//> COUT_DMG 										[CHIP_74153_C10] = SIGNAL(RA2),		// pin 6
//> COUT_DMG 										[CHIP_74153_C11] = SIGNAL(RA2),		// pin 5
//> COUT_DMG 										[CHIP_74153_C12] = SIGNAL(BA2),		// pin 4
//> COUT_DMG 										[CHIP_74153_C13] = SIGNAL(BA8),		// pin 3
//> COUT_DMG 										[CHIP_74153_Y1] = SIGNAL(FA2),		// pin 7
//> COUT_DMG 
//> COUT_DMG 										[CHIP_74153_A] = SIGNAL(MUXA),		// pin 14
//> COUT_DMG 										[CHIP_74153_B] = SIGNAL(CLK1),		// pin 2
//> COUT_DMG 
//> COUT_DMG 										[CHIP_74153_G2] = SIGNAL(LOW),		// pin 15
//> COUT_DMG 										[CHIP_74153_C20] = SIGNAL(RA3),		// pin 10
//> COUT_DMG 										[CHIP_74153_C21] = SIGNAL(RA3),		// pin 11
//> COUT_DMG 										[CHIP_74153_C22] = SIGNAL(BA3),		// pin 12
//> COUT_DMG 										[CHIP_74153_C23] = SIGNAL(BA9),		// pin 13
//> COUT_DMG 										[CHIP_74153_Y2] = SIGNAL(FA3),		// pin 9
//> COUT_DMG 	}));
//> COUT_DMG 
//> COUT_DMG 	DEVICE_REGISTER_CHIP("E5", chip_74153_multiplexer_create(device->simulator, (Chip74153Signals) {
//> COUT_DMG 										[CHIP_74153_G1] = SIGNAL(LOW),		// pin 1
//> COUT_DMG 										[CHIP_74153_C10] = SIGNAL(RA4),		// pin 6
//> COUT_DMG 										[CHIP_74153_C11] = SIGNAL(RA4),		// pin 5
//> COUT_DMG 										[CHIP_74153_C12] = SIGNAL(BA4),		// pin 4
//> COUT_DMG 										[CHIP_74153_C13] = SIGNAL(BA10),	// pin 3
//> COUT_DMG 										[CHIP_74153_Y1] = SIGNAL(FA4),		// pin 7
//> COUT_DMG 
//> COUT_DMG 										[CHIP_74153_A] = SIGNAL(MUXA),		// pin 14
//> COUT_DMG 										[CHIP_74153_B] = SIGNAL(CLK1),		// pin 2
//> COUT_DMG 
//> COUT_DMG 										[CHIP_74153_G2] = SIGNAL(LOW),		// pin 15
//> COUT_DMG 										[CHIP_74153_C20] = SIGNAL(RA5),		// pin 10
//> COUT_DMG 										[CHIP_74153_C21] = SIGNAL(RA5),		// pin 11
//> COUT_DMG 										[CHIP_74153_C22] = SIGNAL(BA5),		// pin 12
//> COUT_DMG 										[CHIP_74153_C23] = SIGNAL(BA11),	// pin 13
//> COUT_DMG 										[CHIP_74153_Y2] = SIGNAL(FA5),		// pin 9
//> COUT_DMG 	}));
//> COUT_DMG 
//> COUT_DMG 	DEVICE_REGISTER_CHIP("E6", chip_74153_multiplexer_create(device->simulator, (Chip74153Signals) {
//> COUT_DMG 										[CHIP_74153_G1] = SIGNAL(LOW),		// pin 1
//> COUT_DMG 										[CHIP_74153_C10] = SIGNAL(RA6),		// pin 6
//> COUT_DMG 										[CHIP_74153_C11] = SIGNAL(RA6),		// pin 5
//> COUT_DMG 										[CHIP_74153_C12] = SIGNAL(BA6),		// pin 4
//> COUT_DMG 										[CHIP_74153_C13] = SIGNAL(BA12),	// pin 3
//> COUT_DMG 										[CHIP_74153_Y1] = SIGNAL(FA6),		// pin 7
//> COUT_DMG 
//> COUT_DMG 										[CHIP_74153_A] = SIGNAL(MUXA),		// pin 14
//> COUT_DMG 										[CHIP_74153_B] = SIGNAL(CLK1),		// pin 2
//> COUT_DMG 	}));
//> COUT_DMG 
//> COUT_DMG 	DEVICE_REGISTER_CHIP("I2-9", chip_8x4116_dram_create(device->simulator, (Chip8x4116DRamSignals) {
//> COUT_DMG 										[CHIP_4116_A0] = SIGNAL(FA0),
//> COUT_DMG 										[CHIP_4116_A1] = SIGNAL(FA1),
//> COUT_DMG 										[CHIP_4116_A2] = SIGNAL(FA2),
//> COUT_DMG 										[CHIP_4116_A3] = SIGNAL(FA3),
//> COUT_DMG 										[CHIP_4116_A4] = SIGNAL(FA4),
//> COUT_DMG 										[CHIP_4116_A5] = SIGNAL(FA5),
//> COUT_DMG 										[CHIP_4116_A6] = SIGNAL(FA6),
//> COUT_DMG 
//> COUT_DMG 										[CHIP_4116_DI0] = SIGNAL(RD0),
//> COUT_DMG 										[CHIP_4116_DI1] = SIGNAL(RD1),
//> COUT_DMG 										[CHIP_4116_DI2] = SIGNAL(RD2),
//> COUT_DMG 										[CHIP_4116_DI3] = SIGNAL(RD3),
//> COUT_DMG 										[CHIP_4116_DI4] = SIGNAL(RD4),
//> COUT_DMG 										[CHIP_4116_DI5] = SIGNAL(RD5),
//> COUT_DMG 										[CHIP_4116_DI6] = SIGNAL(RD6),
//> COUT_DMG 										[CHIP_4116_DI7] = SIGNAL(RD7),
//> COUT_DMG 
//> COUT_DMG 										[CHIP_4116_DO0] = SIGNAL(RD0),
//> COUT_DMG 										[CHIP_4116_DO1] = SIGNAL(RD1),
//> COUT_DMG 										[CHIP_4116_DO2] = SIGNAL(RD2),
//> COUT_DMG 										[CHIP_4116_DO3] = SIGNAL(RD3),
//> COUT_DMG 										[CHIP_4116_DO4] = SIGNAL(RD4),
//> COUT_DMG 										[CHIP_4116_DO5] = SIGNAL(RD5),
//> COUT_DMG 										[CHIP_4116_DO6] = SIGNAL(RD6),
//> COUT_DMG 										[CHIP_4116_DO7] = SIGNAL(RD7),
//> COUT_DMG 
//> COUT_DMG 										[CHIP_4116_WE_B] = SIGNAL(RAMRW),
//> COUT_DMG 										[CHIP_4116_RAS_B] = SIGNAL(RAS0_B),
//> COUT_DMG 										[CHIP_4116_CAS_B] = SIGNAL(CAS0_B)
//> COUT_DMG 	}));
//> COUT_DMG 
//> COUT_DMG 	DEVICE_REGISTER_CHIP("J2-9", chip_8x4116_dram_create(device->simulator, (Chip8x4116DRamSignals) {
//> COUT_DMG 										[CHIP_4116_A0] = SIGNAL(FA0),
//> COUT_DMG 										[CHIP_4116_A1] = SIGNAL(FA1),
//> COUT_DMG 										[CHIP_4116_A2] = SIGNAL(FA2),
//> COUT_DMG 										[CHIP_4116_A3] = SIGNAL(FA3),
//> COUT_DMG 										[CHIP_4116_A4] = SIGNAL(FA4),
//> COUT_DMG 										[CHIP_4116_A5] = SIGNAL(FA5),
//> COUT_DMG 										[CHIP_4116_A6] = SIGNAL(FA6),
//> COUT_DMG 
//> COUT_DMG 										[CHIP_4116_DI0] = SIGNAL(RD0),
//> COUT_DMG 										[CHIP_4116_DI1] = SIGNAL(RD1),
//> COUT_DMG 										[CHIP_4116_DI2] = SIGNAL(RD2),
//> COUT_DMG 										[CHIP_4116_DI3] = SIGNAL(RD3),
//> COUT_DMG 										[CHIP_4116_DI4] = SIGNAL(RD4),
//> COUT_DMG 										[CHIP_4116_DI5] = SIGNAL(RD5),
//> COUT_DMG 										[CHIP_4116_DI6] = SIGNAL(RD6),
//> COUT_DMG 										[CHIP_4116_DI7] = SIGNAL(RD7),
//> COUT_DMG 
//> COUT_DMG 										[CHIP_4116_DO0] = SIGNAL(RD0),
//> COUT_DMG 										[CHIP_4116_DO1] = SIGNAL(RD1),
//> COUT_DMG 										[CHIP_4116_DO2] = SIGNAL(RD2),
//> COUT_DMG 										[CHIP_4116_DO3] = SIGNAL(RD3),
//> COUT_DMG 										[CHIP_4116_DO4] = SIGNAL(RD4),
//> COUT_DMG 										[CHIP_4116_DO5] = SIGNAL(RD5),
//> COUT_DMG 										[CHIP_4116_DO6] = SIGNAL(RD6),
//> COUT_DMG 										[CHIP_4116_DO7] = SIGNAL(RD7),
//> COUT_DMG 
//> COUT_DMG 										[CHIP_4116_WE_B] = SIGNAL(RAMRW),
//> COUT_DMG 										[CHIP_4116_RAS_B] = SIGNAL(RAS0_B),
//> COUT_DMG 										[CHIP_4116_CAS_B] = SIGNAL(CAS1_B)
//> COUT_DMG 	}));
//> COUT_DMG 
//> COUT_DMG 	// glue-logic
//> COUT_DMG 	DEVICE_REGISTER_CHIP("LOGIC5", glue_logic_create_05(device));
//> COUT_DMG }

// sheet 06 - master timing
void circuit_create_06(DevCommodorePet *device) {

	// >> y1 - oscillator
	device->oscillator_y1 = oscillator_create(2000000, device->simulator, (OscillatorSignals) {
										[CHIP_OSCILLATOR_CLK_OUT] = SIGNAL(CLK16)
	});
	DEVICE_REGISTER_CHIP("Y1", device->oscillator_y1);

	// >> g5 - binary counter
//> COUT_DMG 	DEVICE_REGISTER_CHIP("G5", chip_74191_binary_counter_create(device->simulator, (Chip74191Signals) {
//> COUT_DMG 										[CHIP_74191_ENABLE_B] = SIGNAL(LOW),		// pin 04
//> COUT_DMG 										[CHIP_74191_D_U] = SIGNAL(LOW),				// pin 05
//> COUT_DMG 										[CHIP_74191_A] = SIGNAL(LOW),				// pin 15
//> COUT_DMG 										[CHIP_74191_B] = SIGNAL(LOW),				// pin 01
//> COUT_DMG 										[CHIP_74191_C] = SIGNAL(LOW),				// pin 10
//> COUT_DMG 										[CHIP_74191_D] = SIGNAL(LOW),				// pin 09
//> COUT_DMG 										[CHIP_74191_LOAD_B] = SIGNAL(INIT_B),		// pin 11
//> COUT_DMG 										[CHIP_74191_CLK] = SIGNAL(CLK16),			// pin 14
//> COUT_DMG 										[CHIP_74191_QA] = SIGNAL(CLK8),				// pin 03
//> COUT_DMG 										[CHIP_74191_QB] = SIGNAL(CLK4),				// pin 02
//> COUT_DMG 										[CHIP_74191_QC] = SIGNAL(CLK2),				// pin 06
//> COUT_DMG 										[CHIP_74191_QD] = SIGNAL(CLK1),				// pin 07
//> COUT_DMG 								//		[CHIP_74191_MAX_MIN] = not connected        // pin 12
//> COUT_DMG 								//		[CHIP_74191_RCO_B] = not connected          // pin 13
//> COUT_DMG 	}));
//> COUT_DMG 
//> COUT_DMG 	// >> h3 - 8-bit shift register
//> COUT_DMG 	DEVICE_REGISTER_CHIP("H3", chip_74164_shift_register_create(device->simulator, (Chip74164Signals) {
//> COUT_DMG 										[CHIP_74164_A] = SIGNAL(CLK1),				// pin 01
//> COUT_DMG 										[CHIP_74164_B] = SIGNAL(HIGH),				// pin 02
//> COUT_DMG 										[CHIP_74164_CLK] = SIGNAL(CLK16),			// pin 08
//> COUT_DMG 										[CHIP_74164_CLEAR_B] = SIGNAL(INIT_B),		// pin 09
//> COUT_DMG 										[CHIP_74164_QA] = SIGNAL(BPHI2A),			// pin 03
//> COUT_DMG 										[CHIP_74164_QB] = SIGNAL(BPHI2B),			// pin 04
//> COUT_DMG 										[CHIP_74164_QC] = SIGNAL(BPHI2C),			// pin 05
//> COUT_DMG 										[CHIP_74164_QD] = SIGNAL(BPHI2D),			// pin 06
//> COUT_DMG 										[CHIP_74164_QE] = SIGNAL(BPHI2E),			// pin 10
//> COUT_DMG 										[CHIP_74164_QF] = SIGNAL(BPHI2F),			// pin 11
//> COUT_DMG 										[CHIP_74164_QG] = SIGNAL(BPHI2G),			// pin 12
//> COUT_DMG 										[CHIP_74164_QH] = SIGNAL(BPHI2H),			// pin 13
//> COUT_DMG 	}));
//> COUT_DMG 
//> COUT_DMG 	// >> h6 - JK flip-flop
//> COUT_DMG 	DEVICE_REGISTER_CHIP("H6", chip_74107_jk_flipflop_create(device->simulator, (Chip74107Signals) {
//> COUT_DMG 										[CHIP_74107_CLR1_B] = SIGNAL(INIT_B),		// pin 13
//> COUT_DMG 										[CHIP_74107_CLK1] = SIGNAL(BPHI2A_B),		// pin 12
//> COUT_DMG 										[CHIP_74107_J1] = SIGNAL(INIT_B),			// pin 1
//> COUT_DMG 										[CHIP_74107_K1] = SIGNAL(INIT_B),			// pin 4
//> COUT_DMG 										[CHIP_74107_Q1] = SIGNAL(RA1),				// pin 3
//> COUT_DMG 										[CHIP_74107_Q1_B] = SIGNAL(RA1_B),			// pin 2
//> COUT_DMG 
//> COUT_DMG 										[CHIP_74107_CLR2_B] = SIGNAL(INIT_B),		// pin 10
//> COUT_DMG 										[CHIP_74107_CLK2] = SIGNAL(RA5),			// pin 9
//> COUT_DMG 										[CHIP_74107_J2] = SIGNAL(INIT_B),			// pin 8
//> COUT_DMG 										[CHIP_74107_K2] = SIGNAL(INIT_B),			// pin 11
//> COUT_DMG 										[CHIP_74107_Q2] = SIGNAL(RA6),				// pin 5
//> COUT_DMG 										[CHIP_74107_Q2_B] = SIGNAL(RA6_B)			// pin 6
//> COUT_DMG 	}));
//> COUT_DMG 
//> COUT_DMG 	// >> h9 - binary counter
//> COUT_DMG 	DEVICE_REGISTER_CHIP("H9", chip_7493_binary_counter_create(device->simulator, (Chip7493Signals) {
//> COUT_DMG 										[CHIP_7493_A_B] = SIGNAL(RA1),				// pin 14
//> COUT_DMG 										[CHIP_7493_B_B] = SIGNAL(RA2),				// pin 1
//> COUT_DMG 										[CHIP_7493_R01] = SIGNAL(INIT),				// pin 2
//> COUT_DMG 										[CHIP_7493_R02] = SIGNAL(INIT),				// pin 3
//> COUT_DMG 										[CHIP_7493_QA] = SIGNAL(RA2),				// pin 12
//> COUT_DMG 										[CHIP_7493_QB] = SIGNAL(RA3),				// pin 9
//> COUT_DMG 										[CHIP_7493_QC] = SIGNAL(RA4),				// pin 8
//> COUT_DMG 										[CHIP_7493_QD] = SIGNAL(RA5),				// pin 11
//> COUT_DMG 	}));
//> COUT_DMG 
//> COUT_DMG 	// >> g9 - d flip-flop (2 flipflop is used on sheet 8)
//> COUT_DMG 	DEVICE_REGISTER_CHIP("G9", chip_7474_d_flipflop_create(device->simulator, (Chip7474Signals) {
//> COUT_DMG 										[CHIP_7474_D1] = SIGNAL(INIT_B),			// pin 2
//> COUT_DMG 										[CHIP_7474_CLR1_B] = SIGNAL(BPHI2B),		// pin 1
//> COUT_DMG 										[CHIP_7474_CLK1] = SIGNAL(BPHI2A_B),		// pin 3
//> COUT_DMG 										[CHIP_7474_PR1_B] = SIGNAL(HIGH),			// pin 4
//> COUT_DMG 										[CHIP_7474_Q1] = SIGNAL(LOAD_SR),			// pin 5
//> COUT_DMG 										[CHIP_7474_Q1_B] = SIGNAL(LOAD_SR_B),		// pin 6
//> COUT_DMG 
//> COUT_DMG 										[CHIP_7474_D2] = SIGNAL(LSD7),				// pin 12
//> COUT_DMG 										[CHIP_7474_CLR2_B] = SIGNAL(INIT_B),		// pin 13
//> COUT_DMG 										[CHIP_7474_CLK2] = SIGNAL(LOAD_SR),			// pin 11
//> COUT_DMG 										[CHIP_7474_PR2_B] = SIGNAL(HIGH),			// pin 10
//> COUT_DMG 										[CHIP_7474_Q2] = SIGNAL(G9Q),				// pin 9
//> COUT_DMG 										[CHIP_7474_Q2_B] = SIGNAL(G9Q_B),			// pin 8
//> COUT_DMG 	}));
//> COUT_DMG 
//> COUT_DMG 	// >> h7 - JK flip-flop
//> COUT_DMG 	DEVICE_REGISTER_CHIP("H7", chip_74107_jk_flipflop_create(device->simulator, (Chip74107Signals) {
//> COUT_DMG 										[CHIP_74107_CLR1_B] = SIGNAL(INIT_B),		// pin 13
//> COUT_DMG 										[CHIP_74107_CLK1] = SIGNAL(RA1AND3),		// pin 12
//> COUT_DMG 										[CHIP_74107_J1] = SIGNAL(HORZ_DISP_ON),		// pin 1
//> COUT_DMG 										[CHIP_74107_K1] = SIGNAL(HORZ_DISP_OFF),	// pin 4
//> COUT_DMG 										[CHIP_74107_Q1] = SIGNAL(HORZ_DRIVE_B),		// pin 3
//> COUT_DMG 										[CHIP_74107_Q1_B] = SIGNAL(HORZ_DRIVE),		// pin 2
//> COUT_DMG 
//> COUT_DMG 										[CHIP_74107_CLR2_B] = SIGNAL(INIT_B),		// pin 10
//> COUT_DMG 										[CHIP_74107_CLK2] = SIGNAL(LOAD_SR_B),		// pin 9
//> COUT_DMG 										[CHIP_74107_J2] = SIGNAL(RA4AND6),			// pin 8
//> COUT_DMG 										[CHIP_74107_K2] = SIGNAL(RA5AND6_B),		// pin 11
//> COUT_DMG 										[CHIP_74107_Q2] = SIGNAL(HORZ_DISP_ON),		// pin 5
//> COUT_DMG 										[CHIP_74107_Q2_B] = SIGNAL(HORZ_DISP_OFF)	// pin 6
//> COUT_DMG 	}));
//> COUT_DMG 
//> COUT_DMG 	// >> h8 - JK flip-flop
//> COUT_DMG 	DEVICE_REGISTER_CHIP("H8", chip_74107_jk_flipflop_create(device->simulator, (Chip74107Signals) {
//> COUT_DMG 										[CHIP_74107_CLR1_B] = SIGNAL(INIT_B),		// pin 13
//> COUT_DMG 										[CHIP_74107_CLK1] = SIGNAL(NEXT),			// pin 12
//> COUT_DMG 										[CHIP_74107_J1] = SIGNAL(H8Q2_B),			// pin 1
//> COUT_DMG 										[CHIP_74107_K1] = SIGNAL(H8Q2),				// pin 4
//> COUT_DMG 										[CHIP_74107_Q1] = SIGNAL(H8Q),				// pin 3
//> COUT_DMG 										[CHIP_74107_Q1_B] = SIGNAL(H8Q_B),			// pin 2
//> COUT_DMG 
//> COUT_DMG 										[CHIP_74107_CLR2_B] = SIGNAL(INIT_B),		// pin 10
//> COUT_DMG 										[CHIP_74107_CLK2] = SIGNAL(NEXT),			// pin 9
//> COUT_DMG 										[CHIP_74107_J2] = SIGNAL(H8Q),				// pin 8
//> COUT_DMG 										[CHIP_74107_K2] = SIGNAL(H8Q_B),			// pin 11
//> COUT_DMG 										[CHIP_74107_Q2] = SIGNAL(H8Q2),				// pin 5
//> COUT_DMG 										[CHIP_74107_Q2_B] = SIGNAL(H8Q2_B)			// pin 6
//> COUT_DMG 	}));
//> COUT_DMG 
//> COUT_DMG 	// >> h4 - Quad 2to1 multiplexer
//> COUT_DMG 	DEVICE_REGISTER_CHIP("H4", chip_74157_multiplexer_create(device->simulator, (Chip74157Signals) {
//> COUT_DMG 										[CHIP_74157_I0A] = SIGNAL(BPHI2B),			// pin 2
//> COUT_DMG 										[CHIP_74157_I1A] = SIGNAL(BPHI2A),			// pin 3
//> COUT_DMG 										[CHIP_74157_ZA] = SIGNAL(H4Y1),				// pin 4
//> COUT_DMG 
//> COUT_DMG 										[CHIP_74157_I0D] = SIGNAL(BPHI2C),			// pin 11
//> COUT_DMG 										[CHIP_74157_I1D] = SIGNAL(BPHI2B),			// pin 10
//> COUT_DMG 										[CHIP_74157_ZD] = SIGNAL(MUXA),				// pin 9
//> COUT_DMG 
//> COUT_DMG 										[CHIP_74157_I0C] = SIGNAL(BPHI2D),			// pin 14
//> COUT_DMG 										[CHIP_74157_I1C] = SIGNAL(BPHI2C),			// pin 13
//> COUT_DMG 										[CHIP_74157_ZC] = SIGNAL(H4Y4),				// pin 12
//> COUT_DMG 
//> COUT_DMG 										[CHIP_74157_SEL] = SIGNAL(BRW),				// pin 1
//> COUT_DMG 										[CHIP_74157_ENABLE_B] = SIGNAL(H53)			// pin 15
//> COUT_DMG 	}));
//> COUT_DMG 
//> COUT_DMG 	// >> h1 - d flipflop
//> COUT_DMG 	DEVICE_REGISTER_CHIP("H1", chip_7474_d_flipflop_create(device->simulator, (Chip7474Signals) {
//> COUT_DMG 										[CHIP_7474_D1] = SIGNAL(INIT_B),			// pin 2
//> COUT_DMG 										[CHIP_7474_CLR1_B] = SIGNAL(BPHI2G_B),		// pin 1
//> COUT_DMG 										[CHIP_7474_CLK1] = SIGNAL(H4Y1),			// pin 3
//> COUT_DMG 										[CHIP_7474_PR1_B] = SIGNAL(INIT_B),			// pin 4
//> COUT_DMG 										[CHIP_7474_Q1] = SIGNAL(H1Q1),				// pin 5
//> COUT_DMG 										[CHIP_7474_Q1_B] = SIGNAL(H1Q1_B),			// pin 6
//> COUT_DMG 
//> COUT_DMG 										[CHIP_7474_D2] = SIGNAL(INIT_B),			// pin 12
//> COUT_DMG 										[CHIP_7474_CLR2_B] = SIGNAL(BPHI2F),		// pin 13
//> COUT_DMG 										[CHIP_7474_CLK2] = SIGNAL(BPHI2B_B),		// pin 11
//> COUT_DMG 										[CHIP_7474_PR2_B] = SIGNAL(PULLUP_2),		// pin 10
//> COUT_DMG 										[CHIP_7474_Q2] = SIGNAL(H1Q2),				// pin 9
//> COUT_DMG 										[CHIP_7474_Q2_B] = SIGNAL(H1Q2_B)			// pin 8
//> COUT_DMG 	}));
//> COUT_DMG 
//> COUT_DMG 	// glue-logic
//> COUT_DMG 	DEVICE_REGISTER_CHIP("LOGIC6", glue_logic_create_06(device));
//> COUT_DMG 	DEVICE_REGISTER_CHIP("LOGIC6P", glue_logic_create_06_phases(device));
}

// sheet 07 - display logic components
//> COUT_DMG void circuit_create_07(DevCommodorePet *device) {

	// >> g6 - JK flip-flop
//> COUT_DMG 	DEVICE_REGISTER_CHIP("G6", chip_74107_jk_flipflop_create(device->simulator, (Chip74107Signals) {
//> COUT_DMG 										[CHIP_74107_CLR1_B] = SIGNAL(HORZ_DISP_ON),	// pin 13
//> COUT_DMG 										[CHIP_74107_CLK1] = SIGNAL(RA1_B),			// pin 12
//> COUT_DMG 										[CHIP_74107_J1] = SIGNAL(INIT_B),			// pin 1
//> COUT_DMG 										[CHIP_74107_K1] = SIGNAL(INIT_B),			// pin 4
//> COUT_DMG 										[CHIP_74107_Q1] = SIGNAL(G6_Q),				// pin 3
//> COUT_DMG 										[CHIP_74107_Q1_B] = SIGNAL(G6_Q_B),			// pin 2
//> COUT_DMG 	}));
//> COUT_DMG 
//> COUT_DMG 	// >> f6 - quad 2-to-1 multiplexer
//> COUT_DMG 	DEVICE_REGISTER_CHIP("F6", chip_74157_multiplexer_create(device->simulator, (Chip74157Signals) {
//> COUT_DMG 										[CHIP_74157_I0D] = SIGNAL(HIGH),			// pin 11
//> COUT_DMG 										[CHIP_74157_I1D] = SIGNAL(HIGH),			// pin 10
//> COUT_DMG 										[CHIP_74157_I0B] = SIGNAL(HIGH),			// pin 05
//> COUT_DMG 										[CHIP_74157_I1B] = SIGNAL(A5_12),			// pin 06
//> COUT_DMG 										[CHIP_74157_I0C] = SIGNAL(RA1_B),			// pin 14
//> COUT_DMG 										[CHIP_74157_I1C] = SIGNAL(BA0),				// pin 13
//> COUT_DMG 										[CHIP_74157_I0A] = SIGNAL(G6_Q),			// pin 02
//> COUT_DMG 										[CHIP_74157_I1A] = SIGNAL(BA1),				// pin 03
//> COUT_DMG 
//> COUT_DMG 										[CHIP_74157_SEL] = SIGNAL(CLK1),			// pin 01
//> COUT_DMG 										[CHIP_74157_ENABLE_B] = SIGNAL(LOW),		// pin 15
//> COUT_DMG 
//> COUT_DMG 										[CHIP_74157_ZB] = SIGNAL(TV_RAM_RW),		// pin 07
//> COUT_DMG 										[CHIP_74157_ZD] = SIGNAL(F6_Y3),			// pin 09
//> COUT_DMG 										[CHIP_74157_ZC] = SIGNAL(SA0),				// pin 12
//> COUT_DMG 										[CHIP_74157_ZA] = SIGNAL(SA1)				// pin 04
//> COUT_DMG 	}));
//> COUT_DMG 
//> COUT_DMG 	// >> f5 - quad 2-to-1 multiplexer
//> COUT_DMG 	DEVICE_REGISTER_CHIP("F5", chip_74157_multiplexer_create(device->simulator, (Chip74157Signals) {
//> COUT_DMG 										[CHIP_74157_I0D] = SIGNAL(GA2),				// pin 11
//> COUT_DMG 										[CHIP_74157_I1D] = SIGNAL(BA2),				// pin 10
//> COUT_DMG 										[CHIP_74157_I0B] = SIGNAL(GA3),				// pin 05
//> COUT_DMG 										[CHIP_74157_I1B] = SIGNAL(BA3),				// pin 06
//> COUT_DMG 										[CHIP_74157_I0C] = SIGNAL(GA4),				// pin 14
//> COUT_DMG 										[CHIP_74157_I1C] = SIGNAL(BA4),				// pin 13
//> COUT_DMG 										[CHIP_74157_I0A] = SIGNAL(GA5),				// pin 02
//> COUT_DMG 										[CHIP_74157_I1A] = SIGNAL(BA5),				// pin 03
//> COUT_DMG 
//> COUT_DMG 										[CHIP_74157_SEL] = SIGNAL(CLK1),			// pin 01
//> COUT_DMG 										[CHIP_74157_ENABLE_B] = SIGNAL(LOW),		// pin 15
//> COUT_DMG 
//> COUT_DMG 										[CHIP_74157_ZD] = SIGNAL(SA2),				// pin 09
//> COUT_DMG 										[CHIP_74157_ZB] = SIGNAL(SA3),				// pin 07
//> COUT_DMG 										[CHIP_74157_ZC] = SIGNAL(SA4),				// pin 12
//> COUT_DMG 										[CHIP_74157_ZA] = SIGNAL(SA5)				// pin 04
//> COUT_DMG 	}));
//> COUT_DMG 
//> COUT_DMG 	// >> f3 - quad 2-to-1 multiplexer
//> COUT_DMG 	DEVICE_REGISTER_CHIP("F3", chip_74157_multiplexer_create(device->simulator, (Chip74157Signals) {
//> COUT_DMG 										[CHIP_74157_I0A] = SIGNAL(GA6),				// pin 02
//> COUT_DMG 										[CHIP_74157_I1A] = SIGNAL(BA6),				// pin 03
//> COUT_DMG 										[CHIP_74157_I0C] = SIGNAL(GA7),				// pin 14
//> COUT_DMG 										[CHIP_74157_I1C] = SIGNAL(BA7),				// pin 13
//> COUT_DMG 										[CHIP_74157_I0B] = SIGNAL(GA8),				// pin 05
//> COUT_DMG 										[CHIP_74157_I1B] = SIGNAL(BA8),				// pin 06
//> COUT_DMG 										[CHIP_74157_I0D] = SIGNAL(GA9),				// pin 11
//> COUT_DMG 										[CHIP_74157_I1D] = SIGNAL(BA9),				// pin 10
//> COUT_DMG 
//> COUT_DMG 										[CHIP_74157_SEL] = SIGNAL(CLK1),			// pin 01
//> COUT_DMG 										[CHIP_74157_ENABLE_B] = SIGNAL(LOW),		// pin 15
//> COUT_DMG 
//> COUT_DMG 										[CHIP_74157_ZA] = SIGNAL(SA6),				// pin 04
//> COUT_DMG 										[CHIP_74157_ZC] = SIGNAL(SA7),				// pin 12
//> COUT_DMG 										[CHIP_74157_ZB] = SIGNAL(SA8),				// pin 07
//> COUT_DMG 										[CHIP_74157_ZD] = SIGNAL(SA9)				// pin 09
//> COUT_DMG 	}));
//> COUT_DMG 
//> COUT_DMG 	// >> f4 - binary counter
//> COUT_DMG 	DEVICE_REGISTER_CHIP("F4", chip_74177_binary_counter_create(device->simulator, (Chip74177Signals) {
//> COUT_DMG 										[CHIP_74177_CLK2] = SIGNAL(GA2),				// pin 06
//> COUT_DMG 										[CHIP_74177_CLK1] = SIGNAL(G6_Q),				// pin 08
//> COUT_DMG 										[CHIP_74177_LOAD_B] = SIGNAL(HORZ_DISP_ON),		// pin 01
//> COUT_DMG 										[CHIP_74177_CLEAR_B] = SIGNAL(NEXT_B),			// pin 13
//> COUT_DMG 										[CHIP_74177_A] = SIGNAL(LGA2),					// pin 04
//> COUT_DMG 										[CHIP_74177_B] = SIGNAL(LGA3),					// pin 10
//> COUT_DMG 										[CHIP_74177_C] = SIGNAL(LGA4),					// pin 03
//> COUT_DMG 										[CHIP_74177_D] = SIGNAL(LGA5),					// pin 11
//> COUT_DMG 
//> COUT_DMG 										[CHIP_74177_QA] = SIGNAL(GA2),					// pin 05
//> COUT_DMG 										[CHIP_74177_QB] = SIGNAL(GA3),					// pin 09
//> COUT_DMG 										[CHIP_74177_QC] = SIGNAL(GA4),					// pin 02
//> COUT_DMG 										[CHIP_74177_QD] = SIGNAL(GA5)					// pin 12
//> COUT_DMG 	}));
//> COUT_DMG 
//> COUT_DMG 	// >> f2 - binary counter
//> COUT_DMG 	DEVICE_REGISTER_CHIP("F2", chip_74177_binary_counter_create(device->simulator, (Chip74177Signals) {
//> COUT_DMG 										[CHIP_74177_CLK2] = SIGNAL(GA6),				// pin 06
//> COUT_DMG 										[CHIP_74177_CLK1] = SIGNAL(GA5),				// pin 08
//> COUT_DMG 										[CHIP_74177_LOAD_B] = SIGNAL(HORZ_DISP_ON),		// pin 01
//> COUT_DMG 										[CHIP_74177_CLEAR_B] = SIGNAL(NEXT_B),			// pin 13
//> COUT_DMG 										[CHIP_74177_A] = SIGNAL(LGA6),					// pin 04
//> COUT_DMG 										[CHIP_74177_B] = SIGNAL(LGA7),					// pin 10
//> COUT_DMG 										[CHIP_74177_C] = SIGNAL(LGA8),					// pin 03
//> COUT_DMG 										[CHIP_74177_D] = SIGNAL(LGA9),					// pin 11
//> COUT_DMG 
//> COUT_DMG 										[CHIP_74177_QA] = SIGNAL(GA6),					// pin 05
//> COUT_DMG 										[CHIP_74177_QB] = SIGNAL(GA7),					// pin 09
//> COUT_DMG 										[CHIP_74177_QC] = SIGNAL(GA8),					// pin 02
//> COUT_DMG 										[CHIP_74177_QD] = SIGNAL(GA9)					// pin 12
//> COUT_DMG 	}));
//> COUT_DMG 
//> COUT_DMG 	// >> g3 - 8-bit latch
//> COUT_DMG 	DEVICE_REGISTER_CHIP("G3", chip_74373_latch_create(device->simulator, (Chip74373Signals) {
//> COUT_DMG 										[CHIP_74373_D5] = SIGNAL(GA2),			// pin 13
//> COUT_DMG 										[CHIP_74373_D8] = SIGNAL(GA3),			// pin 18
//> COUT_DMG 										[CHIP_74373_D6] = SIGNAL(GA4),			// pin 14
//> COUT_DMG 										[CHIP_74373_D7] = SIGNAL(GA5),			// pin 17
//> COUT_DMG 										[CHIP_74373_D2] = SIGNAL(GA6),			// pin 04
//> COUT_DMG 										[CHIP_74373_D3] = SIGNAL(GA7),			// pin 07
//> COUT_DMG 										[CHIP_74373_D1] = SIGNAL(GA8),			// pin 03
//> COUT_DMG 										[CHIP_74373_D4] = SIGNAL(GA9),			// pin 08
//> COUT_DMG 
//> COUT_DMG 										[CHIP_74373_C] = SIGNAL(RELOAD_NEXT),	// pin 11 - enable input
//> COUT_DMG 										[CHIP_74373_OC_B] = SIGNAL(LOW),		// pin 01 - output control
//> COUT_DMG 
//> COUT_DMG 										[CHIP_74373_Q5] = SIGNAL(LGA2),			// pin 12
//> COUT_DMG 										[CHIP_74373_Q8] = SIGNAL(LGA3),			// pin 19
//> COUT_DMG 										[CHIP_74373_Q6] = SIGNAL(LGA4),			// pin 15
//> COUT_DMG 										[CHIP_74373_Q7] = SIGNAL(LGA5),			// pin 16
//> COUT_DMG 										[CHIP_74373_Q2] = SIGNAL(LGA6),			// pin 05
//> COUT_DMG 										[CHIP_74373_Q3] = SIGNAL(LGA7),			// pin 06
//> COUT_DMG 										[CHIP_74373_Q1] = SIGNAL(LGA8),			// pin 02
//> COUT_DMG 										[CHIP_74373_Q4] = SIGNAL(LGA9)			// pin 09
//> COUT_DMG 	}));
//> COUT_DMG 
//> COUT_DMG 	// >> g8 - d flip-flop
//> COUT_DMG 	DEVICE_REGISTER_CHIP("G8", chip_7474_d_flipflop_create(device->simulator, (Chip7474Signals) {
//> COUT_DMG 										// [CHIP_7474_D1] = not used,				// pin 2
//> COUT_DMG 										// [CHIP_7474_CLR1_B] = not used,			// pin 1
//> COUT_DMG 										// [CHIP_7474_CLK1] = not used,				// pin 3
//> COUT_DMG 										// [CHIP_7474_PR1_B] = not used,			// pin 4
//> COUT_DMG 										// [CHIP_7474_Q1] = not used,				// pin 5
//> COUT_DMG 										// [CHIP_7474_Q1_B] = not used,				// pin 6
//> COUT_DMG 
//> COUT_DMG 										[CHIP_7474_PR2_B] = SIGNAL(PULLUP_1),		// pin 10
//> COUT_DMG 										[CHIP_7474_D2] = SIGNAL(W220_OFF),			// pin 12
//> COUT_DMG 										[CHIP_7474_CLK2] = SIGNAL(VIDEO_LATCH),		// pin 11
//> COUT_DMG 										[CHIP_7474_CLR2_B] = SIGNAL(BPHI2H),		// pin 13
//> COUT_DMG 										[CHIP_7474_Q2] = SIGNAL(NEXT),				// pin 9
//> COUT_DMG 										[CHIP_7474_Q2_B] = SIGNAL(NEXT_B)			// pin 8
//> COUT_DMG 	}));
//> COUT_DMG 
//> COUT_DMG 	// glue-logic
//> COUT_DMG 	DEVICE_REGISTER_CHIP("LOGIC7", glue_logic_create_07(device));
//> COUT_DMG }

// sheet 08: display rams components
//> COUT_DMG void circuit_create_08(DevCommodorePet *device) {

	// >> h11 - binary counter
//> COUT_DMG 	DEVICE_REGISTER_CHIP("H11", chip_7493_binary_counter_create(device->simulator, (Chip7493Signals) {
//> COUT_DMG 										[CHIP_7493_A_B] = SIGNAL(LOW),				// pin 14
//> COUT_DMG 										[CHIP_7493_B_B] = SIGNAL(HORZ_DISP_ON),		// pin 1
//> COUT_DMG 										[CHIP_7493_R01] = SIGNAL(NEXT),				// pin 2
//> COUT_DMG 										[CHIP_7493_R02] = SIGNAL(NEXT),				// pin 3
//> COUT_DMG 										// [CHIP_7493_QA] = not used,				// pin 12
//> COUT_DMG 										[CHIP_7493_QB] = SIGNAL(RA7),				// pin 9
//> COUT_DMG 										[CHIP_7493_QC] = SIGNAL(RA8),				// pin 8
//> COUT_DMG 										[CHIP_7493_QD] = SIGNAL(RA9),				// pin 11
//> COUT_DMG 	}));
//> COUT_DMG 
//> COUT_DMG 	// >> f7 - 1k x 4bit SRAM
//> COUT_DMG 	DEVICE_REGISTER_CHIP("F7", chip_6114_sram_create(device->simulator, (Chip6114SRamSignals) {
//> COUT_DMG 										[CHIP_6114_A0] = SIGNAL(SA0),
//> COUT_DMG 										[CHIP_6114_A1] = SIGNAL(SA1),
//> COUT_DMG 										[CHIP_6114_A2] = SIGNAL(SA2),
//> COUT_DMG 										[CHIP_6114_A3] = SIGNAL(SA3),
//> COUT_DMG 										[CHIP_6114_A4] = SIGNAL(SA4),
//> COUT_DMG 										[CHIP_6114_A5] = SIGNAL(SA5),
//> COUT_DMG 										[CHIP_6114_A6] = SIGNAL(SA6),
//> COUT_DMG 										[CHIP_6114_A7] = SIGNAL(SA7),
//> COUT_DMG 										[CHIP_6114_A8] = SIGNAL(SA8),
//> COUT_DMG 										[CHIP_6114_A9] = SIGNAL(SA9),
//> COUT_DMG 
//> COUT_DMG 										[CHIP_6114_IO0] = SIGNAL(SD4),
//> COUT_DMG 										[CHIP_6114_IO1] = SIGNAL(SD5),
//> COUT_DMG 										[CHIP_6114_IO2] = SIGNAL(SD6),
//> COUT_DMG 										[CHIP_6114_IO3] = SIGNAL(SD7),
//> COUT_DMG 
//> COUT_DMG 										[CHIP_6114_CE_B] = SIGNAL(LOW),
//> COUT_DMG 										[CHIP_6114_RW] = SIGNAL(TV_RAM_RW)
//> COUT_DMG 	}));
//> COUT_DMG 
//> COUT_DMG 	// >> f8 - 1k x 4bit SRAM
//> COUT_DMG 	DEVICE_REGISTER_CHIP("F8", chip_6114_sram_create(device->simulator, (Chip6114SRamSignals) {
//> COUT_DMG 										[CHIP_6114_A0] = SIGNAL(SA0),
//> COUT_DMG 										[CHIP_6114_A1] = SIGNAL(SA1),
//> COUT_DMG 										[CHIP_6114_A2] = SIGNAL(SA2),
//> COUT_DMG 										[CHIP_6114_A3] = SIGNAL(SA3),
//> COUT_DMG 										[CHIP_6114_A4] = SIGNAL(SA4),
//> COUT_DMG 										[CHIP_6114_A5] = SIGNAL(SA5),
//> COUT_DMG 										[CHIP_6114_A6] = SIGNAL(SA6),
//> COUT_DMG 										[CHIP_6114_A7] = SIGNAL(SA7),
//> COUT_DMG 										[CHIP_6114_A8] = SIGNAL(SA8),
//> COUT_DMG 										[CHIP_6114_A9] = SIGNAL(SA9),
//> COUT_DMG 
//> COUT_DMG 										[CHIP_6114_IO0] = SIGNAL(SD0),
//> COUT_DMG 										[CHIP_6114_IO1] = SIGNAL(SD1),
//> COUT_DMG 										[CHIP_6114_IO2] = SIGNAL(SD2),
//> COUT_DMG 										[CHIP_6114_IO3] = SIGNAL(SD3),
//> COUT_DMG 
//> COUT_DMG 										[CHIP_6114_CE_B] = SIGNAL(LOW),
//> COUT_DMG 										[CHIP_6114_RW] = SIGNAL(TV_RAM_RW)
//> COUT_DMG 	}));
//> COUT_DMG 
//> COUT_DMG 	// >> e8 - octal buffer
//> COUT_DMG 	DEVICE_REGISTER_CHIP("E8", chip_74244_octal_buffer_create(device->simulator, (Chip74244Signals) {
//> COUT_DMG 										[CHIP_74244_G2_B] = SIGNAL(TV_RAM_RW),	// 19
//> COUT_DMG 										[CHIP_74244_G1_B] = SIGNAL(TV_READ_B),	// 01
//> COUT_DMG 
//> COUT_DMG 										[CHIP_74244_A11]  = SIGNAL(SD0),		// 02
//> COUT_DMG 										[CHIP_74244_Y24]  = SIGNAL(SD0),		// 03
//> COUT_DMG 										[CHIP_74244_A12]  = SIGNAL(SD1),		// 04
//> COUT_DMG 										[CHIP_74244_Y23]  = SIGNAL(SD1),		// 05
//> COUT_DMG 										[CHIP_74244_A13]  = SIGNAL(SD2),		// 06
//> COUT_DMG 										[CHIP_74244_Y22]  = SIGNAL(SD2),		// 07
//> COUT_DMG 										[CHIP_74244_A14]  = SIGNAL(SD3),		// 08
//> COUT_DMG 										[CHIP_74244_Y21]  = SIGNAL(SD3),		// 09
//> COUT_DMG 
//> COUT_DMG 										[CHIP_74244_Y11]  = SIGNAL(BD0),		// 18
//> COUT_DMG 										[CHIP_74244_A24]  = SIGNAL(BD0),		// 17
//> COUT_DMG 										[CHIP_74244_Y12]  = SIGNAL(BD1),		// 16
//> COUT_DMG 										[CHIP_74244_A23]  = SIGNAL(BD1),		// 15
//> COUT_DMG 										[CHIP_74244_Y13]  = SIGNAL(BD2),		// 14
//> COUT_DMG 										[CHIP_74244_A22]  = SIGNAL(BD2),		// 13
//> COUT_DMG 										[CHIP_74244_Y14]  = SIGNAL(BD3),		// 12
//> COUT_DMG 										[CHIP_74244_A21]  = SIGNAL(BD3)			// 11
//> COUT_DMG 	}));
//> COUT_DMG 
//> COUT_DMG 	// >> e7 - octal buffer
//> COUT_DMG 	DEVICE_REGISTER_CHIP("E7", chip_74244_octal_buffer_create(device->simulator, (Chip74244Signals) {
//> COUT_DMG 										[CHIP_74244_G2_B] = SIGNAL(TV_RAM_RW),	// 19
//> COUT_DMG 										[CHIP_74244_G1_B] = SIGNAL(TV_READ_B),	// 01
//> COUT_DMG 
//> COUT_DMG 										[CHIP_74244_A11]  = SIGNAL(SD4),		// 02
//> COUT_DMG 										[CHIP_74244_Y24]  = SIGNAL(SD4),		// 03
//> COUT_DMG 										[CHIP_74244_A12]  = SIGNAL(SD5),		// 04
//> COUT_DMG 										[CHIP_74244_Y23]  = SIGNAL(SD5),		// 05
//> COUT_DMG 										[CHIP_74244_A13]  = SIGNAL(SD6),		// 06
//> COUT_DMG 										[CHIP_74244_Y22]  = SIGNAL(SD6),		// 07
//> COUT_DMG 										[CHIP_74244_A14]  = SIGNAL(SD7),		// 08
//> COUT_DMG 										[CHIP_74244_Y21]  = SIGNAL(SD7),		// 09
//> COUT_DMG 
//> COUT_DMG 										[CHIP_74244_Y11]  = SIGNAL(BD4),		// 18
//> COUT_DMG 										[CHIP_74244_A24]  = SIGNAL(BD4),		// 17
//> COUT_DMG 										[CHIP_74244_Y12]  = SIGNAL(BD5),		// 16
//> COUT_DMG 										[CHIP_74244_A23]  = SIGNAL(BD5),		// 15
//> COUT_DMG 										[CHIP_74244_Y13]  = SIGNAL(BD6),		// 14
//> COUT_DMG 										[CHIP_74244_A22]  = SIGNAL(BD6),		// 13
//> COUT_DMG 										[CHIP_74244_Y14]  = SIGNAL(BD7),		// 12
//> COUT_DMG 										[CHIP_74244_A21]  = SIGNAL(BD7)			// 11
//> COUT_DMG 	}));
//> COUT_DMG 
//> COUT_DMG 	// >> f9 - 8-bit latch
//> COUT_DMG 	DEVICE_REGISTER_CHIP("F9", chip_74373_latch_create(device->simulator, (Chip74373Signals) {
//> COUT_DMG 										[CHIP_74373_C] = SIGNAL(VIDEO_LATCH),	// 11
//> COUT_DMG 										[CHIP_74373_OC_B] = SIGNAL(LOW),		// 1
//> COUT_DMG 										[CHIP_74373_D1] = SIGNAL(SD0),			// 3
//> COUT_DMG 										[CHIP_74373_D8] = SIGNAL(SD1),			// 18
//> COUT_DMG 										[CHIP_74373_D2] = SIGNAL(SD2),			// 4
//> COUT_DMG 										[CHIP_74373_D7] = SIGNAL(SD3),			// 17
//> COUT_DMG 										[CHIP_74373_D3] = SIGNAL(SD4),			// 7
//> COUT_DMG 										[CHIP_74373_D6] = SIGNAL(SD5),			// 14
//> COUT_DMG 										[CHIP_74373_D4] = SIGNAL(SD6),			// 8
//> COUT_DMG 										[CHIP_74373_D5] = SIGNAL(SD7),			// 13
//> COUT_DMG 
//> COUT_DMG 										[CHIP_74373_Q1] = SIGNAL(LSD0),			// 2
//> COUT_DMG 										[CHIP_74373_Q8] = SIGNAL(LSD1),			// 19
//> COUT_DMG 										[CHIP_74373_Q2] = SIGNAL(LSD2),			// 5
//> COUT_DMG 										[CHIP_74373_Q7] = SIGNAL(LSD3),			// 16
//> COUT_DMG 										[CHIP_74373_Q3] = SIGNAL(LSD4),			// 6
//> COUT_DMG 										[CHIP_74373_Q6] = SIGNAL(LSD5),			// 15
//> COUT_DMG 										[CHIP_74373_Q4] = SIGNAL(LSD6),			// 9
//> COUT_DMG 										[CHIP_74373_Q5] = SIGNAL(LSD7),			// 12
//> COUT_DMG 	}));
//> COUT_DMG 
//> COUT_DMG 	// >> f10 - character rom
//> COUT_DMG 	Chip63xxRom *char_rom = load_character_rom(device, "runtime/commodore_pet/characters-2.901447-10.bin");
//> COUT_DMG 	assert(char_rom);
//> COUT_DMG 	DEVICE_REGISTER_CHIP("F10", char_rom);
//> COUT_DMG 
//> COUT_DMG 	// >> e11 - shift register
//> COUT_DMG 	DEVICE_REGISTER_CHIP("E11", chip_74165_shift_register_create(device->simulator, (Chip74165Signals) {
//> COUT_DMG 										[CHIP_74165_SL		] = SIGNAL(LOAD_SR_B),	// 1
//> COUT_DMG 										[CHIP_74165_CLK	] = SIGNAL(CLK8),			// 2
//> COUT_DMG 										[CHIP_74165_CLK_INH] = SIGNAL(LOW),			// 15
//> COUT_DMG 										[CHIP_74165_SI		] = SIGNAL(CD7),		// 10
//> COUT_DMG 										[CHIP_74165_A		] = SIGNAL(CD0),		// 11
//> COUT_DMG 										[CHIP_74165_B		] = SIGNAL(CD1),		// 12
//> COUT_DMG 										[CHIP_74165_C		] = SIGNAL(CD2),		// 13
//> COUT_DMG 										[CHIP_74165_D		] = SIGNAL(CD3),		// 14
//> COUT_DMG 										[CHIP_74165_E		] = SIGNAL(CD4),		// 3
//> COUT_DMG 										[CHIP_74165_F		] = SIGNAL(CD5),		// 4
//> COUT_DMG 										[CHIP_74165_G		] = SIGNAL(CD6),		// 5
//> COUT_DMG 										[CHIP_74165_H		] = SIGNAL(CD7),		// 6
//> COUT_DMG 										[CHIP_74165_QH		] = SIGNAL(E11QH),		// 9
//> COUT_DMG 										[CHIP_74165_QH_B	] = SIGNAL(E11QH_B),	// 7
//> COUT_DMG 	}));
//> COUT_DMG 
//> COUT_DMG 	// glue-logic
//> COUT_DMG 	DEVICE_REGISTER_CHIP("LOGIC8", glue_logic_create_08(device));
//> COUT_DMG }

// peripherals
//> COUT_DMG void circuit_create_peripherals(DevCommodorePet *device, bool lite) {

	// keyboard
//> COUT_DMG 	device->keypad = input_keypad_create(device->simulator, false, 10, 8, 500, 100,
//> COUT_DMG 										 (Signal[10]) {	 SIGNAL(KOUT0), SIGNAL(KOUT1),
//> COUT_DMG 														 SIGNAL(KOUT2), SIGNAL(KOUT3),
//> COUT_DMG 														 SIGNAL(KOUT4), SIGNAL(KOUT5),
//> COUT_DMG 														 SIGNAL(KOUT6), SIGNAL(KOUT7),
//> COUT_DMG 														 SIGNAL(KOUT8), SIGNAL(KOUT9)},
//> COUT_DMG 										 (Signal[8]) {	 SIGNAL(KIN0), SIGNAL(KIN1),
//> COUT_DMG 														 SIGNAL(KIN2), SIGNAL(KIN3),
//> COUT_DMG 														 SIGNAL(KIN4), SIGNAL(KIN5),
//> COUT_DMG 														 SIGNAL(KIN6), SIGNAL(KIN7)}
//> COUT_DMG 	);
//> COUT_DMG 	DEVICE_REGISTER_CHIP("KEYPAD", device->keypad);
//> COUT_DMG 
//> COUT_DMG 	// display
//> COUT_DMG 	if (!lite) {
//> COUT_DMG 		device->crt = perif_pet_crt_create(device->simulator, (PerifPetCrtSignals) {
//> COUT_DMG 											[PIN_PETCRT_VIDEO_IN] = SIGNAL(VIDEO),
//> COUT_DMG 											[PIN_PETCRT_HORZ_DRIVE_IN] = SIGNAL(HORZ_DRIVE),
//> COUT_DMG 											[PIN_PETCRT_VERT_DRIVE_IN] = SIGNAL(VERT_DRIVE)
//> COUT_DMG 		});
//> COUT_DMG 		device->screen = device->crt->display;
//> COUT_DMG 		DEVICE_REGISTER_CHIP("CRT", device->crt);
//> COUT_DMG 	} else {
//> COUT_DMG 		device->screen = display_rgba_create(40 * 8, 25 * 8);
//> COUT_DMG 		DEVICE_REGISTER_CHIP("DISPLAY", lite_display_create(device));
//> COUT_DMG 	}
//> COUT_DMG 
//> COUT_DMG 	// datassette
//> COUT_DMG 	device->datassette = perif_datassette_create(device->simulator, (PerifDatassetteSignals) {
//> COUT_DMG 											[PIN_DS1530_SENSE] = SIGNAL(CASS_SWITCH_1),
//> COUT_DMG 											[PIN_DS1530_MOTOR] = SIGNAL(CASS_MOTOR_1),
//> COUT_DMG 											[PIN_DS1530_DATA_FROM_DS] = SIGNAL(CASS_READ_1),
//> COUT_DMG 											[PIN_DS1530_DATA_TO_DS] = SIGNAL(CASS_WRITE)
//> COUT_DMG 	});
//> COUT_DMG 	DEVICE_REGISTER_CHIP("CASS1", device->datassette);
//> COUT_DMG 
//> COUT_DMG 	// ieee-488 tester
//> COUT_DMG 	device->disk_2031 = perif_fd2031_create(device->simulator, (PerifDisk2031Signals) {
//> COUT_DMG 											[PERIF_FD2031_EOI_B ] = SIGNAL(EOI_B),
//> COUT_DMG 											[PERIF_FD2031_DAV_B ] = SIGNAL(DAV_B),
//> COUT_DMG 											[PERIF_FD2031_NRFD_B] = SIGNAL(NRFD_B),
//> COUT_DMG 											[PERIF_FD2031_NDAC_B] = SIGNAL(NDAC_B),
//> COUT_DMG 											[PERIF_FD2031_ATN_B ] = SIGNAL(ATN_B),
//> COUT_DMG 											[PERIF_FD2031_SRQ_B ] = SIGNAL(SRQ_IN_B),
//> COUT_DMG 											[PERIF_FD2031_IFC_B ] = SIGNAL(IFC_B),
//> COUT_DMG 
//> COUT_DMG 											[PERIF_FD2031_DIO0  ] = SIGNAL(DIO0),
//> COUT_DMG 											[PERIF_FD2031_DIO1  ] = SIGNAL(DIO1),
//> COUT_DMG 											[PERIF_FD2031_DIO2  ] = SIGNAL(DIO2),
//> COUT_DMG 											[PERIF_FD2031_DIO3  ] = SIGNAL(DIO3),
//> COUT_DMG 											[PERIF_FD2031_DIO4  ] = SIGNAL(DIO4),
//> COUT_DMG 											[PERIF_FD2031_DIO5  ] = SIGNAL(DIO5),
//> COUT_DMG 											[PERIF_FD2031_DIO6  ] = SIGNAL(DIO6),
//> COUT_DMG 											[PERIF_FD2031_DIO7  ] = SIGNAL(DIO7),
//> COUT_DMG 	});
//> COUT_DMG 	DEVICE_REGISTER_CHIP("488TEST", device->disk_2031);
//> COUT_DMG }

// lite-PET: RAM circuitry
//> COUT_DMG void circuit_lite_create_ram(DevCommodorePet *device) {

//> COUT_DMG 	Ram8d16a *ram = ram_8d16a_create(15, device->simulator, (Ram8d16aSignals) {
//> COUT_DMG 										[CHIP_RAM8D16A_A0] = SIGNAL(BA0),
//> COUT_DMG 										[CHIP_RAM8D16A_A1] = SIGNAL(BA1),
//> COUT_DMG 										[CHIP_RAM8D16A_A2] = SIGNAL(BA2),
//> COUT_DMG 										[CHIP_RAM8D16A_A3] = SIGNAL(BA3),
//> COUT_DMG 										[CHIP_RAM8D16A_A4] = SIGNAL(BA4),
//> COUT_DMG 										[CHIP_RAM8D16A_A5] = SIGNAL(BA5),
//> COUT_DMG 										[CHIP_RAM8D16A_A6] = SIGNAL(BA6),
//> COUT_DMG 										[CHIP_RAM8D16A_A7] = SIGNAL(BA7),
//> COUT_DMG 										[CHIP_RAM8D16A_A8] = SIGNAL(BA8),
//> COUT_DMG 										[CHIP_RAM8D16A_A9] = SIGNAL(BA9),
//> COUT_DMG 										[CHIP_RAM8D16A_A10] = SIGNAL(BA10),
//> COUT_DMG 										[CHIP_RAM8D16A_A11] = SIGNAL(BA11),
//> COUT_DMG 										[CHIP_RAM8D16A_A12] = SIGNAL(BA12),
//> COUT_DMG 										[CHIP_RAM8D16A_A13] = SIGNAL(BA13),
//> COUT_DMG 										[CHIP_RAM8D16A_A14] = SIGNAL(BA14),
//> COUT_DMG 										[CHIP_RAM8D16A_A15] = SIGNAL(BA15),
//> COUT_DMG 
//> COUT_DMG 										[CHIP_RAM8D16A_D0] = SIGNAL(BD0),
//> COUT_DMG 										[CHIP_RAM8D16A_D1] = SIGNAL(BD1),
//> COUT_DMG 										[CHIP_RAM8D16A_D2] = SIGNAL(BD2),
//> COUT_DMG 										[CHIP_RAM8D16A_D3] = SIGNAL(BD3),
//> COUT_DMG 										[CHIP_RAM8D16A_D4] = SIGNAL(BD4),
//> COUT_DMG 										[CHIP_RAM8D16A_D5] = SIGNAL(BD5),
//> COUT_DMG 										[CHIP_RAM8D16A_D6] = SIGNAL(BD6),
//> COUT_DMG 										[CHIP_RAM8D16A_D7] = SIGNAL(BD7),
//> COUT_DMG 
//> COUT_DMG 										[CHIP_RAM8D16A_CE_B] = SIGNAL(BA15),
//> COUT_DMG 										[CHIP_RAM8D16A_OE_B] = SIGNAL(G7_8),
//> COUT_DMG 										[CHIP_RAM8D16A_WE_B] = SIGNAL(RAMRW)
//> COUT_DMG 	});
//> COUT_DMG 	DEVICE_REGISTER_CHIP("RAM", ram);
//> COUT_DMG 
//> COUT_DMG 	// glue-logic
//> COUT_DMG 	DEVICE_REGISTER_CHIP("LOGIC5", glue_logic_create_05(device));
//> COUT_DMG }

// lite-PET: master timing
void circuit_lite_create_timing(DevCommodorePet *device) {
	device->oscillator_y1 = oscillator_create(1000000, device->simulator, (OscillatorSignals) {
										[CHIP_OSCILLATOR_CLK_OUT] = SIGNAL(CLK1)
	});
	DEVICE_REGISTER_CHIP("OSC", device->oscillator_y1);
}

// lite-PET: display ram
//> COUT_DMG void circuit_lite_create_vram(DevCommodorePet *device) {
//> COUT_DMG	assert(device);
//> COUT_DMG 	DEVICE_REGISTER_CHIP("VRAM", ram_8d16a_create(10, device->simulator, (Ram8d16aSignals) {
//> COUT_DMG 										[CHIP_RAM8D16A_A0] = SIGNAL(BA0),
//> COUT_DMG 										[CHIP_RAM8D16A_A1] = SIGNAL(BA1),
//> COUT_DMG 										[CHIP_RAM8D16A_A2] = SIGNAL(BA2),
//> COUT_DMG 										[CHIP_RAM8D16A_A3] = SIGNAL(BA3),
//> COUT_DMG 										[CHIP_RAM8D16A_A4] = SIGNAL(BA4),
//> COUT_DMG 										[CHIP_RAM8D16A_A5] = SIGNAL(BA5),
//> COUT_DMG 										[CHIP_RAM8D16A_A6] = SIGNAL(BA6),
//> COUT_DMG 										[CHIP_RAM8D16A_A7] = SIGNAL(BA7),
//> COUT_DMG 										[CHIP_RAM8D16A_A8] = SIGNAL(BA8),
//> COUT_DMG 										[CHIP_RAM8D16A_A9] = SIGNAL(BA9),
//> COUT_DMG 
//> COUT_DMG 										[CHIP_RAM8D16A_D0] = SIGNAL(BD0),
//> COUT_DMG 										[CHIP_RAM8D16A_D1] = SIGNAL(BD1),
//> COUT_DMG 										[CHIP_RAM8D16A_D2] = SIGNAL(BD2),
//> COUT_DMG 										[CHIP_RAM8D16A_D3] = SIGNAL(BD3),
//> COUT_DMG 										[CHIP_RAM8D16A_D4] = SIGNAL(BD4),
//> COUT_DMG 										[CHIP_RAM8D16A_D5] = SIGNAL(BD5),
//> COUT_DMG 										[CHIP_RAM8D16A_D6] = SIGNAL(BD6),
//> COUT_DMG 										[CHIP_RAM8D16A_D7] = SIGNAL(BD7),
//> COUT_DMG 
//> COUT_DMG 										[CHIP_RAM8D16A_CE_B] = SIGNAL(SEL8_B),
//> COUT_DMG 										[CHIP_RAM8D16A_OE_B] = SIGNAL(TV_READ_B),
//> COUT_DMG 										[CHIP_RAM8D16A_WE_B] = SIGNAL(RAMRW)
//> COUT_DMG 	}));
//> COUT_DMG 
//> COUT_DMG 	DEVICE_REGISTER_CHIP("LOGIC7", glue_logic_create_07_lite(device));
//> COUT_DMG }

///////////////////////////////////////////////////////////////////////////////
//
// interface functions
//

//> COUT_DMG void dev_commodore_pet_read_memory(DevCommodorePet *device, size_t start_address, size_t size, uint8_t *output);
//> COUT_DMG void dev_commodore_pet_write_memory(DevCommodorePet *device, size_t start_address, size_t size, uint8_t *input);
//> COUT_DMG void dev_commodore_pet_lite_read_memory(DevCommodorePet *device, size_t start_address, size_t size, uint8_t *output);
//> COUT_DMG void dev_commodore_pet_lite_write_memory(DevCommodorePet *device, size_t start_address, size_t size, uint8_t *input);
//> COUT_DMG size_t dev_commodore_pet_get_irq_signals(DevCommodorePet *device, SignalBreakpoint **irq_signals);

Cpu6502* dev_commodore_pet_get_cpu(DevCommodorePet *device) {
	assert(device);
	return device->cpu;
}

DevCommodorePet *create_pet_device(bool lite) {
	DevCommodorePet *device = (DevCommodorePet *) dms_calloc(1, sizeof(DevCommodorePet));
	device->is_lite = lite;

	// interface
	device->get_cpu = (DEVICE_GET_CPU) dev_commodore_pet_get_cpu;
	device->process = (DEVICE_PROCESS) device_process;
	device->reset = (DEVICE_RESET) dev_commodore_pet_reset;
	device->destroy = (DEVICE_DESTROY) dev_commodore_pet_destroy;
//> COUT_DMG 	device->read_memory = (DEVICE_READ_MEMORY) ((!lite) ? dev_commodore_pet_read_memory : dev_commodore_pet_lite_read_memory);
//> COUT_DMG	    device->write_memory = (DEVICE_WRITE_MEMORY) ((!lite) ? dev_commodore_pet_write_memory : dev_commodore_pet_lite_write_memory);
//> COUT_DMG 	device->get_irq_signals = (DEVICE_GET_IRQ_SIGNALS) dev_commodore_pet_get_irq_signals;

	device->simulator = simulator_create(6250);		// 6.25 ns - 160 Mhz

	device->diag_mode = false;
	device->diag_toggled = false;

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// signals
	//

	device->signal_pool = device->simulator->signal_pool;

	//
	// signals - general
	//

	SIGNAL_DEFINE_DEFAULT_N(RESET_BTN_B, ACTLO_DEASSERT, "RESBTN");
	SIGNAL_DEFINE_DEFAULT_N(HIGH, true, "VCC");
	SIGNAL_DEFINE_DEFAULT_N(LOW, false, "GND");

	//
	// signals - sheet 1: microprocessor / memory expansion
	//

	SIGNAL_DEFINE_DEFAULT_N(RESET_B, ACTLO_ASSERT, "/RES");
	SIGNAL_DEFINE_N(RESET, "RES");
	SIGNAL_DEFINE_DEFAULT_N(IRQ_B, ACTLO_DEASSERT, "/IRQ");
	SIGNAL_DEFINE_DEFAULT_N(NMI_B, ACTLO_DEASSERT, "/NMI");

	SIGNAL_GROUP_NEW_N(cpu_address, 16, &SIGNAL(AB0), "cpu_address", "AB%d");

	SIGNAL_GROUP_NEW_N(cpu_data, 8, &SIGNAL(D0), "cpu_data", "D%d");

	SIGNAL_DEFINE_DEFAULT_N(RW, true, "RW");
	SIGNAL_DEFINE_N(SYNC, "SYNC");
	SIGNAL_DEFINE_DEFAULT_N(RDY, ACTHI_ASSERT, "RDY");

	SIGNAL_GROUP_NEW_N(buf_address, 16, &SIGNAL(BA0), "buffered_address", "BA%d");

	SIGNAL_GROUP_NEW_N(buf_data, 8, &SIGNAL(BD0), "buffered_data", "BD%d");

	SIGNAL_GROUP_NEW_N(mem_sel, 16, &SIGNAL(SEL0_B), "mem_sel", "/SEL%X");

	SIGNAL_DEFINE_N(SEL8, "SEL8");

	SIGNAL_DEFINE_N(X8XX, "X8XX");
	SIGNAL_DEFINE_N(88XX_B, "/88XX");
	SIGNAL_DEFINE_N(ROMA_B, "/ROMA");

	SIGNAL_DEFINE_N(RAMR_B, "/RAMR");
	SIGNAL_DEFINE_N(RAMW_B, "/RAMW");

	SIGNAL_DEFINE_N(PHI2, "PHI2");
	SIGNAL_DEFINE_N(BPHI2, "BPHI2");
	SIGNAL_DEFINE_N(CPHI2, "CPHI2");

	SIGNAL_DEFINE_N(BRW, "BRW");
	SIGNAL_DEFINE_N(BRW_B, "/BRW");
	SIGNAL_DEFINE_N(RAMRW, "RAMRW");

	SIGNAL_DEFINE_N(A5_12, "A512");

	SIGNAL_DEFINE_N(BA11_B, "/BA11");

	//
	// signals - sheet 2: IEEE-488 interface
	//

	SIGNAL_DEFINE_N(ATN_IN_B, "/ATNIN");
	SIGNAL_DEFINE_N(NDAC_OUT_B, "/NDACOUT");
	SIGNAL_DEFINE_N(DAV_OUT_B, "/DAVOUT");

	SIGNAL_DEFINE_DEFAULT_N(EOI_B, true, "/EOI");
	SIGNAL_DEFINE_N(DAV_B, "/DAV");
	SIGNAL_DEFINE_N(NRFD_B, "/NRFD");
	SIGNAL_DEFINE_N(NDAC_B, "/NDAC");
	SIGNAL_DEFINE_N(ATN_B, "/ATN");
	SIGNAL_DEFINE_N(DIO0, "DIO0");
	SIGNAL_DEFINE_N(DIO1, "DIO1");
	SIGNAL_DEFINE_N(DIO2, "DIO2");
	SIGNAL_DEFINE_N(DIO3, "DIO3");
	SIGNAL_DEFINE_N(DIO4, "DIO4");
	SIGNAL_DEFINE_N(DIO5, "DIO5");
	SIGNAL_DEFINE_N(DIO6, "DIO6");
	SIGNAL_DEFINE_N(DIO7, "DIO7");
	SIGNAL_DEFINE_N(SRQ_IN_B, "/SRQIN");
	SIGNAL_DEFINE_DEFAULT_N(IFC_B, true, "/IFC");

	SIGNAL_GROUP_NEW_N(ieee488_di, 8, &SIGNAL(DI0), "ieee488_di", "DI%d");
	SIGNAL_GROUP_NEW_N(ieee488_do, 8, &SIGNAL(DO0), "ieee488_do", "DO%d");

	SIGNAL_DEFINE_N(CS1, "CS1");

	//
	// signals - sheet 3: Cassette & Keyboard
	//

	SIGNAL_DEFINE_N(CA1, "CA1");
	SIGNAL_DEFINE_N(GRAPHIC, "GRAPHIC");
	SIGNAL_DEFINE_N(CASS_MOTOR_2, "CASSMOTOR2");

	SIGNAL_DEFINE_N(NDAC_IN_B, "/NDACIN");
	SIGNAL_DEFINE_N(NRFD_OUT_B, "/NRFDOUT");
	SIGNAL_DEFINE_N(ATN_OUT_B, "/ATNOUT");
	SIGNAL_DEFINE_N(CASS_WRITE, "CASSWRITE");
	SIGNAL_DEFINE_N(CASS_MOTOR_2_B, "/CASSMOTOR2");
	SIGNAL_DEFINE_N(NRFD_IN_B, "/NRFDIN");
	SIGNAL_DEFINE_N(DAV_IN_B, "/DAVIN");

	SIGNAL_DEFINE_N(KEYA, "KEYA");
	SIGNAL_DEFINE_N(KEYB, "KEYB");
	SIGNAL_DEFINE_N(KEYC, "KEYC");
	SIGNAL_DEFINE_N(KEYD, "KEYD");
	SIGNAL_DEFINE_N(CASS_SWITCH_1, "CASSSWITCH1");
	SIGNAL_DEFINE_N(CASS_SWITCH_2, "CASSSWITCH2");
	SIGNAL_DEFINE_N(EOI_IN_B, "/EOIIN");
	SIGNAL_DEFINE_DEFAULT_N(DIAG, true, "DIAG");

	SIGNAL_DEFINE_N(CB2, "CB2");
	SIGNAL_DEFINE_N(EOI_OUT_B, "/EOIOUT");
	SIGNAL_DEFINE_N(CASS_READ_1, "CASSREAD1");
	SIGNAL_DEFINE_N(CASS_READ_2, "CASSREAD2");
	SIGNAL_DEFINE_N(CASS_MOTOR_1, "CASSMOTOR1");
	SIGNAL_DEFINE_N(CASS_MOTOR_1_B, "/CASSMOTOR1");

	SIGNAL_GROUP_NEW_N(pa, 8, &SIGNAL(PA0), "c5_pa", "PA%d");

	SIGNAL_GROUP_NEW_N(keyboard_in, 8, &SIGNAL(KIN0), "keyboard_in", "KIN%d");
	SIGNAL_GROUP_DEFAULTS(keyboard_in, 0xff);							// pull-up resistors R18-R25

	SIGNAL_GROUP_NEW_N(keyboard_out, 10, &SIGNAL(KOUT0), "keyboard_out", "KOUT%d");

	//
	// signals - sheet 5: RAMS
	//

	SIGNAL_DEFINE_N(BANKSEL, "BANKSEL");
	SIGNAL_DEFINE_N(G7_8, "G78");

	SIGNAL_GROUP_NEW_N(ram_address, 7, &SIGNAL(FA0), "ram_address", "FA%d");

	SIGNAL_GROUP_NEW_N(ram_data, 8, &SIGNAL(RD0), "ram_data", "RD%d");

	//
	// signals - sheet 6: Master timing
	//

	SIGNAL_DEFINE_DEFAULT_N(INIT_B, ACTLO_ASSERT, "/INIT");
	SIGNAL_DEFINE_DEFAULT_N(INIT, ACTHI_ASSERT, "INIT");

	SIGNAL_DEFINE_DEFAULT_N(CLK1, true, "CLK1");

	SIGNAL_DEFINE_N(VIDEO_ON, "VIDEOON");

	if (!lite) {

		SIGNAL_DEFINE_DEFAULT_N(CLK16, true, "CLK16");
		SIGNAL_DEFINE_DEFAULT_N(CLK8, true, "CLK8");
		SIGNAL_DEFINE_DEFAULT_N(CLK4, true, "CLK4");
		SIGNAL_DEFINE_DEFAULT_N(CLK2, true, "CLK2");

		SIGNAL_DEFINE_N(BPHI2A, "BPHI2A");
		SIGNAL_DEFINE_N(BPHI2B, "BPHI2B");
		SIGNAL_DEFINE_N(BPHI2C, "BPHI2C");
		SIGNAL_DEFINE_N(BPHI2D, "BPHI2D");
		SIGNAL_DEFINE_N(BPHI2E, "BPHI2E");
		SIGNAL_DEFINE_N(BPHI2F, "BPHI2F");
		SIGNAL_DEFINE_N(BPHI2G, "BPHI2G");
		SIGNAL_DEFINE_N(BPHI2H, "BPHI2H");

		SIGNAL_DEFINE_N(BPHI2A_B, "/BPHI2A");
		SIGNAL_DEFINE_N(BPHI2B_B, "/BPHI2B");
		SIGNAL_DEFINE_N(BPHI2F_B, "/BPHI2F");
		SIGNAL_DEFINE_N(BPHI2G_B, "/BPHI2G");

		SIGNAL_DEFINE_N(RA1, "RA1");
		SIGNAL_DEFINE_N(RA2, "RA2");
		SIGNAL_DEFINE_N(RA3, "RA3");
		SIGNAL_DEFINE_N(RA4, "RA4");
		SIGNAL_DEFINE_N(RA5, "RA5");
		SIGNAL_DEFINE_N(RA6, "RA6");
		SIGNAL_DEFINE_N(RA7, "RA7");
		SIGNAL_DEFINE_N(RA8, "RA8");
		SIGNAL_DEFINE_N(RA9, "RA9");
		SIGNAL_DEFINE_N(RA1_B, "/RA1");
		SIGNAL_DEFINE_N(RA6_B, "/RA6");

		SIGNAL_DEFINE_N(RA1AND3, "RA1AND3");
		SIGNAL_DEFINE_N(RA4AND6, "RA4AND6");
		SIGNAL_DEFINE_N(RA5AND6_B, "RA5AND/6");

		SIGNAL_DEFINE_N(LOAD_SR, "LOADSR");
		SIGNAL_DEFINE_N(LOAD_SR_B, "/LOADSR");

		SIGNAL_DEFINE_N(HORZ_DISP_ON, "HORZDISPON");
		SIGNAL_DEFINE_N(HORZ_DISP_OFF, "HORZDISPOFF");
		SIGNAL_DEFINE_N(HORZ_DRIVE, "HORZDRIVE");
		SIGNAL_DEFINE_N(HORZ_DRIVE_B, "/HORZDRIVE");

		SIGNAL_DEFINE_N(H8Q, "H8Q");
		SIGNAL_DEFINE_N(H8Q_B, "/H8Q");
		SIGNAL_DEFINE_N(H8Q2, "H8Q2");
		SIGNAL_DEFINE_N(H8Q2_B, "/H8Q2");

		SIGNAL_DEFINE_N(VIDEO_LATCH, "VIDEOLATCH");
		SIGNAL_DEFINE_N(VERT_DRIVE, "VERTDRIVE");

		SIGNAL_DEFINE_N(H53, "H53");
		SIGNAL_DEFINE_N(H4Y1, "H4Y1");
		SIGNAL_DEFINE_N(MUXA, "MUXA");
		SIGNAL_DEFINE_N(H4Y4, "H4Y4");

		SIGNAL_DEFINE_N(H1Q1, "H1Q1");
		SIGNAL_DEFINE_N(H1Q1_B, "/H1Q1");
		SIGNAL_DEFINE_N(H1Q2, "H1Q2");
		SIGNAL_DEFINE_N(H1Q2_B, "/H1Q2");

		SIGNAL_DEFINE_N(RAS0_B, "/RAS0");
		SIGNAL_DEFINE_N(CAS0_B, "/CAS0");
		SIGNAL_DEFINE_N(CAS1_B, "/CAS1");
		SIGNAL_DEFINE_N(BA14_B, "/BA14");
	}

	//
	// signals - sheet 7: display logic
	//

	SIGNAL_DEFINE_N(TV_SEL, "TVSEL");
	SIGNAL_DEFINE_N(TV_READ_B, "/TVREAD");

	if (!lite) {
		SIGNAL_DEFINE_N(G6_Q, "G6Q");
		SIGNAL_DEFINE_N(G6_Q_B, "/G6Q");

		SIGNAL_DEFINE_N(TV_RAM_RW, "TVRAMRW");
		SIGNAL_DEFINE_N(F6_Y3, "F6Y3");

		SIGNAL_GROUP_NEW_N(vram_address, 10, &SIGNAL(SA0), "vram_address", "SA%d");

		SIGNAL_DEFINE_N(GA2, "GA2");
		SIGNAL_DEFINE_N(GA3, "GA3");
		SIGNAL_DEFINE_N(GA4, "GA4");
		SIGNAL_DEFINE_N(GA5, "GA5");
		SIGNAL_DEFINE_N(GA6, "GA6");
		SIGNAL_DEFINE_N(GA7, "GA7");
		SIGNAL_DEFINE_N(GA8, "GA8");
		SIGNAL_DEFINE_N(GA9, "GA9");

		SIGNAL_DEFINE_N(LGA2, "LGA2");
		SIGNAL_DEFINE_N(LGA3, "LGA3");
		SIGNAL_DEFINE_N(LGA4, "LGA4");
		SIGNAL_DEFINE_N(LGA5, "LGA5");
		SIGNAL_DEFINE_N(LGA6, "LGA6");
		SIGNAL_DEFINE_N(LGA7, "LGA7");
		SIGNAL_DEFINE_N(LGA8, "LGA8");
		SIGNAL_DEFINE_N(LGA9, "LGA9");

		SIGNAL_DEFINE_DEFAULT_N(NEXT, true, "NEXT");
		SIGNAL_DEFINE_DEFAULT_N(NEXT_B, false, "/NEXT");

		SIGNAL_DEFINE_N(RELOAD_NEXT, "RELOADNEXT");

		SIGNAL_DEFINE_DEFAULT_N(PULLUP_2, true, "PULLUP2");

		SIGNAL_DEFINE_N(LINES_20_B, "/20LINES");
		SIGNAL_DEFINE_N(LINES_200_B, "/200LINES");
		SIGNAL_DEFINE_N(LINE_220, "220LINE");
		SIGNAL_DEFINE_N(LGA_HI_B, "/LGAHI");
		SIGNAL_DEFINE_N(LGA_HI, "LGAHI");
		SIGNAL_DEFINE_N(W220_OFF, "220OFF");

		SIGNAL_DEFINE_N(VIDEO_ON_B, "/VIDEOON");
	}

	//
	// signals - sheet 8: display rams
	//

	if (!lite) {
		SIGNAL_DEFINE_N(RELOAD_B, "/RELOAD");
		SIGNAL_DEFINE_DEFAULT_N(PULLUP_1, true, "PULLUP1");

		SIGNAL_GROUP_NEW_N(vram_data, 8, &SIGNAL(SD0), "vram_data", "SD%d");
		SIGNAL_GROUP_NEW_N(latched_vram_data, 8, &SIGNAL(LSD0), "latched_vram_data", "LSD%d");
		SIGNAL_GROUP_NEW_N(char_data, 8, &SIGNAL(CD0), "char_data", "CD%d");

		SIGNAL_DEFINE_N(G9Q, "G9Q");
		SIGNAL_DEFINE_N(G9Q_B, "/G9Q");
		SIGNAL_DEFINE_N(E11QH, "E11QH");
		SIGNAL_DEFINE_N(E11QH_B, "/E11QH");
		SIGNAL_DEFINE_N(G106, "G106");
		SIGNAL_DEFINE_N(G108, "G108");
		SIGNAL_DEFINE_N(H108, "H108");

		SIGNAL_DEFINE_N(VIDEO, "VIDEO");
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// components
	//

	// sheet 01: microprocessor & memory expansion
	circuit_create_01(device);

	// sheet 02: IEEE-488 Interface
//> COUT_DMG 	circuit_create_02(device);

	// sheet 03: cassette & keyboard
//> COUT_DMG 	circuit_create_03(device);

	// sheet 04: ROMS
//> COUT_DMG	circuit_create_04(device);

	if (!lite) {
		// sheet 05: RAMS
//> COUT_DMG		circuit_create_05(device);

		// sheet 06: master timing
		circuit_create_06(device);

		// sheet 07: display logic components
//> COUT_DMG		circuit_create_07(device);

		// sheet 08: display rams components
//>	COUT_DMG	circuit_create_08(device);
	} else {
		// main ram
//> COUT_DMG		circuit_lite_create_ram(device);

		// master timing
		circuit_lite_create_timing(device);

		// display ram
//> COUT_DMG		circuit_lite_create_vram(device);
	}

	// peripherals
//> COUT_DMG	circuit_create_peripherals(device, lite);

	// let the simulator know no more chips will be added
	simulator_device_complete(device->simulator);

	// signal history - logic analyzer profiles
	dev_commodore_pet_history_profiles(device, "PET", device->simulator->signal_history);
	cpu_6502_signal_history_profiles(device->cpu, "CPU 6502", device->simulator->signal_history);

//> COUT_DMG 	chip_6520_signal_history_profiles(device->pia_1, "PIA (IEEE-488)", device->simulator->signal_history);
//> COUT_DMG	chip_6520_signal_history_profiles(device->pia_2, "PIA (Keyboard)", device->simulator->signal_history);

//> COUT_DMG	chip_6522_signal_history_profiles(device->via, "VIA", device->simulator->signal_history);

//> COUT_DMG	chip_63xx_signal_history_profiles(device->roms[0], "BASIC ROM 1", device->simulator->signal_history);
//> COUT_DMG	chip_63xx_signal_history_profiles(device->roms[1], "BASIC ROM 2", device->simulator->signal_history);
//> COUT_DMG	chip_63xx_signal_history_profiles(device->roms[2], "BASIC ROM 3", device->simulator->signal_history);
//> COUT_DMG	chip_63xx_signal_history_profiles(device->roms[3], "EDITOR ROM",  device->simulator->signal_history);
//> COUT_DMG	chip_63xx_signal_history_profiles(device->roms[4], "KERNAL ROM",  device->simulator->signal_history);

//> COUT_DMG 	if (!lite) {
//> COUT_DMG 		chip_63xx_signal_history_profiles((Chip63xxRom *) simulator_chip_by_name(device->simulator, "F10"),
//> COUT_DMG 											"CHAR ROM", device->simulator->signal_history);
//> COUT_DMG 	}
//> COUT_DMG 
//> COUT_DMG 	perif_datassette_signal_history_profiles(device->datassette, "Datassette", device->simulator->signal_history);
//> COUT_DMG 	perif_disk2031_signal_history_profiles(device->disk_2031, "Floppy Disk", device->simulator->signal_history);

	return device;
}

DevCommodorePet *dev_commodore_pet_create(void) {
	return create_pet_device(false);
}

DevCommodorePet *dev_commodore_pet_lite_create(void) {
	return create_pet_device(true);
}

void dev_commodore_pet_destroy(DevCommodorePet *device) {
	assert(device);

	signal_group_destroy(device->sg_cpu_address);
	signal_group_destroy(device->sg_cpu_data);
	signal_group_destroy(device->sg_buf_address);
	signal_group_destroy(device->sg_buf_data);
	signal_group_destroy(device->sg_mem_sel);
	signal_group_destroy(device->sg_ieee488_di);
	signal_group_destroy(device->sg_ieee488_do);
	signal_group_destroy(device->sg_pa);
	signal_group_destroy(device->sg_keyboard_in);
	signal_group_destroy(device->sg_keyboard_out);
	signal_group_destroy(device->sg_ram_address);
	signal_group_destroy(device->sg_ram_data);
	signal_group_destroy(device->sg_vram_address);
	signal_group_destroy(device->sg_vram_data);
	signal_group_destroy(device->sg_latched_vram_data);
	signal_group_destroy(device->sg_char_data);

	if (device->is_lite) {
//> COUT_DMG		display_rgba_destroy(device->screen);
	}

	simulator_destroy(device->simulator);
	dms_free(device);
}

void dev_commodore_pet_process_clk1(DevCommodorePet *device) {
	bool prev_clk1 = SIGNAL_READ(CLK1);
	do {
		device->process(device);
	} while (prev_clk1 == SIGNAL_READ(CLK1));
}

void dev_commodore_pet_reset(DevCommodorePet *device) {
	assert(device);
	device->in_reset = true;
}

//> COUT_DMG void dev_commodore_pet_read_memory(DevCommodorePet *device, size_t start_address, size_t size, uint8_t *output) {
//> COUT_DMG 	assert(device);
//> COUT_DMG 
//> COUT_DMG 	assert(output);
//> COUT_DMG 	// note: this function skips the free rom slots
//> COUT_DMG 	static const size_t	REGION_START[] = {0x0000, 0x4000, 0x8000, 0x8800, 0xb000, 0xc000, 0xd000, 0xe000, 0xe800, 0xf000};
//> COUT_DMG 
//> COUT_DMG 	static const size_t REGION_SIZE[]  = {0x4000, 0x4000, 0x0800, 0x2800, 0x1000, 0x1000, 0x1000, 0x0800, 0x0800, 0x1000};
//> COUT_DMG 	static const int NUM_REGIONS = sizeof(REGION_START) / sizeof(REGION_START[0]);
//> COUT_DMG 	if (start_address > 0xffff) {
//> COUT_DMG 		dms_zero(output, size);
//> COUT_DMG 
//> COUT_DMG 		return;
//> COUT_DMG 	}
//> COUT_DMG 	// find start region
//> COUT_DMG 	int sr = NUM_REGIONS - 1;
//> COUT_DMG 
//> COUT_DMG 	while (start_address < REGION_START[sr] && sr > 0) {
//> COUT_DMG 		sr -= 1;
//> COUT_DMG 	}
//> COUT_DMG 	size_t remain = size;
//> COUT_DMG 	size_t done = 0;
//> COUT_DMG 
//> COUT_DMG 	size_t addr = start_address;
//> COUT_DMG 	for (int region = sr; remain > 0 && addr <= 0xffff && region < NUM_REGIONS; ++region) {
//> COUT_DMG 		size_t region_offset = addr - REGION_START[region];
//> COUT_DMG 
//> COUT_DMG 		size_t to_copy = MIN(remain, REGION_SIZE[region] - region_offset);
//> COUT_DMG 		switch (region) {
//> COUT_DMG 			case 0:	{			// RAM (0-16k)
//> COUT_DMG 
//> COUT_DMG 				Chip8x4116DRam *ram = (Chip8x4116DRam *) simulator_chip_by_name(device->simulator, "I2-9");
//> COUT_DMG 				for (size_t i = 0; i < to_copy; ++i) {
//> COUT_DMG 					size_t row = (region_offset + i) & 0x007f;
//> COUT_DMG 					size_t col = ((region_offset + i) & 0x3f80) >> 7;
//> COUT_DMG 					size_t physical = (row << 7) | ((col & 0x003f) << 1) | ((col & 0x0040) >> 6);
//> COUT_DMG 					output[done+i] = ram->data_array[physical];
//> COUT_DMG 				}
//> COUT_DMG 				break;
//> COUT_DMG 			}
//> COUT_DMG 			case 1:	{			// RAM (16-32k)
//> COUT_DMG 				Chip8x4116DRam *ram = (Chip8x4116DRam *) simulator_chip_by_name(device->simulator, "J2-9");
//> COUT_DMG 				for (size_t i = 0; i < to_copy; ++i) {
//> COUT_DMG 					size_t row = (region_offset + i) & 0x007f;
//> COUT_DMG 					size_t col = ((region_offset + i) & 0x3f80) >> 7;
//> COUT_DMG 					size_t physical = (row << 7) | ((col & 0x003f) << 1) | ((col & 0x0040) >> 6);
//> COUT_DMG 					output[done+i] = ram->data_array[physical];
//> COUT_DMG 				}
//> COUT_DMG 				break;
//> COUT_DMG 			}
//> COUT_DMG 			case 2:	{			// Screen RAM
//> COUT_DMG 				Chip6114SRam *ram_hi = (Chip6114SRam *) simulator_chip_by_name(device->simulator, "F7");
//> COUT_DMG 				Chip6114SRam *ram_lo = (Chip6114SRam *) simulator_chip_by_name(device->simulator, "F8");
//> COUT_DMG 				for (size_t i = 0; i < to_copy; ++i) {
//> COUT_DMG 					output[done + i] = (uint8_t) (((ram_hi->data_array[region_offset + i] & 0xf) << 4) |
//> COUT_DMG 
//> COUT_DMG 												   (ram_lo->data_array[region_offset + i] & 0xf));
//> COUT_DMG 				}
//> COUT_DMG 				break;
//> COUT_DMG 			}
//> COUT_DMG 			case 3:				// unused space between video memory and first rom
//> COUT_DMG 			case 8:				// I/O area (pia-1, pia-2, via)
//> COUT_DMG 				dms_zero(output + done, to_copy);
//> COUT_DMG 				break;
//> COUT_DMG 			case 4:				// basic-rom 1
//> COUT_DMG 			case 5:				// basic-rom 2
//> COUT_DMG 			case 6:				// basic-rom 3
//> COUT_DMG 			case 7:				// editor rom
//> COUT_DMG 				dms_memcpy(output + done, device->roms[region-4]->data_array + region_offset, to_copy);
//> COUT_DMG 				break;
//> COUT_DMG 			case 9:				// kernal rom
//> COUT_DMG 				dms_memcpy(output + done, device->roms[4]->data_array + region_offset, to_copy);
//> COUT_DMG 				break;
//> COUT_DMG 		}
//> COUT_DMG 
//> COUT_DMG 		remain -= to_copy;
//> COUT_DMG 		addr += to_copy;
//> COUT_DMG 
//> COUT_DMG 		done += to_copy;
//> COUT_DMG 	}
//> COUT_DMG }

//> COUT_DMG void dev_commodore_pet_write_memory(DevCommodorePet *device, size_t start_address, size_t size, uint8_t *input) {
//> COUT_DMG 	assert(device);
//> COUT_DMG 
//> COUT_DMG 	assert(input);
//> COUT_DMG 	// note: this function only allows writes to the main RAM and video memory
//> COUT_DMG 	static const size_t	REGION_START[] = {0x0000, 0x4000, 0x8000};
//> COUT_DMG 
//> COUT_DMG 	static const size_t REGION_SIZE[]  = {0x4000, 0x4000, 0x0800};
//> COUT_DMG 	static const int NUM_REGIONS = sizeof(REGION_START) / sizeof(REGION_START[0]);
//> COUT_DMG 	if (start_address > 0x87ff) {
//> COUT_DMG 		return;
//> COUT_DMG 
//> COUT_DMG 	}
//> COUT_DMG 	// find start region
//> COUT_DMG 	int sr = NUM_REGIONS - 1;
//> COUT_DMG 
//> COUT_DMG 	while (start_address < REGION_START[sr] && sr > 0) {
//> COUT_DMG 		sr -= 1;
//> COUT_DMG 	}
//> COUT_DMG 	size_t remain = size;
//> COUT_DMG 	size_t done = 0;
//> COUT_DMG 
//> COUT_DMG 	size_t addr = start_address;
//> COUT_DMG 	for (int region = sr; remain > 0 && addr < 0x8800 && region < NUM_REGIONS; ++region) {
//> COUT_DMG 		size_t region_offset = addr - REGION_START[region];
//> COUT_DMG 
//> COUT_DMG 		size_t to_copy = MIN(remain, REGION_SIZE[region] - region_offset);
//> COUT_DMG 		switch (region) {
//> COUT_DMG 			case 0:	{			// RAM (0-16k)
//> COUT_DMG 
//> COUT_DMG 				Chip8x4116DRam *ram = (Chip8x4116DRam *) simulator_chip_by_name(device->simulator, "I2-9");
//> COUT_DMG 				for (size_t i = 0; i < to_copy; ++i) {
//> COUT_DMG 					size_t row = (region_offset + i) & 0x007f;
//> COUT_DMG 					size_t col = ((region_offset + i) & 0x3f80) >> 7;
//> COUT_DMG 					size_t physical = (row << 7) | ((col & 0x003f) << 1) | ((col & 0x0040) >> 6);
//> COUT_DMG 					ram->data_array[physical] = input[done+i];
//> COUT_DMG 				}
//> COUT_DMG 				break;
//> COUT_DMG 			}
//> COUT_DMG 			case 1:	{			// RAM (16-32k)
//> COUT_DMG 				Chip8x4116DRam *ram = (Chip8x4116DRam *) simulator_chip_by_name(device->simulator, "J2-9");
//> COUT_DMG 				for (size_t i = 0; i < to_copy; ++i) {
//> COUT_DMG 					size_t row = (region_offset + i) & 0x007f;
//> COUT_DMG 					size_t col = ((region_offset + i) & 0x3f80) >> 7;
//> COUT_DMG 					size_t physical = (row << 7) | ((col & 0x003f) << 1) | ((col & 0x0040) >> 6);
//> COUT_DMG 					ram->data_array[physical] = input[done+i];
//> COUT_DMG 				}
//> COUT_DMG 				break;
//> COUT_DMG 			}
//> COUT_DMG 			case 2:	{			// Screen RAM
//> COUT_DMG 				Chip6114SRam *ram_hi = (Chip6114SRam *) simulator_chip_by_name(device->simulator, "F7");
//> COUT_DMG 				Chip6114SRam *ram_lo = (Chip6114SRam *) simulator_chip_by_name(device->simulator, "F8");
//> COUT_DMG 
//> COUT_DMG 				for (size_t i = 0; i < to_copy; ++i) {
//> COUT_DMG 					ram_hi->data_array[region_offset+i] = (input[done+i] & 0xf0) >> 4;
//> COUT_DMG 					ram_lo->data_array[region_offset+i] = (input[done+i] & 0x0f);
//> COUT_DMG 				}
//> COUT_DMG 				break;
//> COUT_DMG 			}
//> COUT_DMG 		}
//> COUT_DMG 
//> COUT_DMG 		remain -= to_copy;
//> COUT_DMG 		addr += to_copy;
//> COUT_DMG 		done += to_copy;
//> COUT_DMG 	}
//> COUT_DMG }


//> COUT_DMG void dev_commodore_pet_lite_read_memory(DevCommodorePet *device, size_t start_address, size_t size, uint8_t *output) {
//> COUT_DMG 	assert(device);
//> COUT_DMG 	assert(output);
//> COUT_DMG 
//> COUT_DMG 	// note: this function skips the free rom slots
//> COUT_DMG 	static const size_t	REGION_START[] = {0x0000, 0x8000, 0x8800, 0xb000, 0xc000, 0xd000, 0xe000, 0xe800, 0xf000};
//> COUT_DMG 	static const size_t REGION_SIZE[]  = {0x8000, 0x0800, 0x2800, 0x1000, 0x1000, 0x1000, 0x0800, 0x0800, 0x1000};
//> COUT_DMG 	static const int NUM_REGIONS = sizeof(REGION_START) / sizeof(REGION_START[0]);
//> COUT_DMG 
//> COUT_DMG 	if (start_address > 0xffff) {
//> COUT_DMG 		dms_zero(output, size);
//> COUT_DMG 		return;
//> COUT_DMG 	}
//> COUT_DMG 
//> COUT_DMG 	// find start region
//> COUT_DMG 	int sr = NUM_REGIONS - 1;
//> COUT_DMG 	while (start_address < REGION_START[sr] && sr > 0) {
//> COUT_DMG 		sr -= 1;
//> COUT_DMG 	}
//> COUT_DMG 
//> COUT_DMG 	size_t remain = size;
//> COUT_DMG 	size_t done = 0;
//> COUT_DMG 	size_t addr = start_address;
//> COUT_DMG 
//> COUT_DMG 	for (int region = sr; remain > 0 && addr <= 0xffff && region < NUM_REGIONS; ++region) {
//> COUT_DMG 		size_t region_offset = addr - REGION_START[region];
//> COUT_DMG 		size_t to_copy = MIN(remain, REGION_SIZE[region] - region_offset);
//> COUT_DMG 
//> COUT_DMG 		switch (region) {
//> COUT_DMG 			case 0:	{			// RAM (0-32k)
//> COUT_DMG //> 				Ram8d16a *ram = (Ram8d16a *) simulator_chip_by_name(device->simulator, "RAM");
//> COUT_DMG //> 				dms_memcpy(output + done, ram->data_array + region_offset, to_copy);
//> COUT_DMG 				break;
//> COUT_DMG 			}
//> COUT_DMG 			case 1:	{			// Screen RAM
//> COUT_DMG //> 				Ram8d16a *vram = (Ram8d16a *) simulator_chip_by_name(device->simulator, "VRAM");
//> COUT_DMG //> 				dms_memcpy(output + done, vram->data_array + region_offset, to_copy);
//> COUT_DMG 				break;
//> COUT_DMG 			}
//> COUT_DMG 			case 2:				// unused space between video memory and first rom
//> COUT_DMG 			case 7:				// I/O area (pia-1, pia-2, via)
//> COUT_DMG 				dms_zero(output + done, to_copy);
//> COUT_DMG 				break;
//> COUT_DMG 			case 3:				// basic-rom 1
//> COUT_DMG 			case 4:				// basic-rom 2
//> COUT_DMG 			case 5:				// basic-rom 3
//> COUT_DMG 			case 6:				// editor rom
//> COUT_DMG //> 				dms_memcpy(output + done, device->roms[region-3]->data_array + region_offset, to_copy);
//> COUT_DMG 				break;
//> COUT_DMG 			case 8:				// kernal rom
//> COUT_DMG //> 				dms_memcpy(output + done, device->roms[4]->data_array + region_offset, to_copy);
//> COUT_DMG 				break;
//> COUT_DMG 		}
//> COUT_DMG 
//> COUT_DMG 		remain -= to_copy;
//> COUT_DMG 		addr += to_copy;
//> COUT_DMG 		done += to_copy;
//> COUT_DMG 	}
//> COUT_DMG }

//> COUT_DMG void dev_commodore_pet_lite_write_memory(DevCommodorePet *device, size_t start_address, size_t size, uint8_t *input) {
//> COUT_DMG 	assert(device);
//> COUT_DMG 	assert(input);
//> COUT_DMG 
//> COUT_DMG 	// note: this function only allows writes to the main RAM and video memory
//> COUT_DMG 	static const size_t	REGION_START[] = {0x0000, 0x8000};
//> COUT_DMG 	static const size_t REGION_SIZE[]  = {0x8000, 0x0800};
//> COUT_DMG 	static const int NUM_REGIONS = sizeof(REGION_START) / sizeof(REGION_START[0]);
//> COUT_DMG 
//> COUT_DMG 	if (start_address > 0x87ff) {
//> COUT_DMG 		return;
//> COUT_DMG 	}
//> COUT_DMG 
//> COUT_DMG 	// find start region
//> COUT_DMG 	int sr = NUM_REGIONS - 1;
//> COUT_DMG 	while (start_address < REGION_START[sr] && sr > 0) {
//> COUT_DMG 		sr -= 1;
//> COUT_DMG 	}
//> COUT_DMG 
//> COUT_DMG 	size_t remain = size;
//> COUT_DMG 	size_t done = 0;
//> COUT_DMG 	size_t addr = start_address;
//> COUT_DMG 
//> COUT_DMG 	for (int region = sr; remain > 0 && addr < 0x8800 && region < NUM_REGIONS; ++region) {
//> COUT_DMG 		size_t region_offset = addr - REGION_START[region];
//> COUT_DMG 		size_t to_copy = MIN(remain, REGION_SIZE[region] - region_offset);
//> COUT_DMG 
//> COUT_DMG 		switch (region) {
//> COUT_DMG 			case 0:	{			// RAM (0-32k)
//> COUT_DMG 				Ram8d16a *ram = (Ram8d16a *) simulator_chip_by_name(device->simulator, "RAM");
//> COUT_DMG 				dms_memcpy(ram->data_array + region_offset, input + done, to_copy);
//> COUT_DMG 				break;
//> COUT_DMG 			}
//> COUT_DMG 			case 1:	{			// Screen RAM
//> COUT_DMG 				Ram8d16a *vram = (Ram8d16a *) simulator_chip_by_name(device->simulator, "VRAM");
//> COUT_DMG 				dms_memcpy(vram->data_array + region_offset, input + done, to_copy);
//> COUT_DMG 				break;
//> COUT_DMG 			}
//> COUT_DMG 		}
//> COUT_DMG 
//> COUT_DMG 		remain -= to_copy;
//> COUT_DMG 		addr += to_copy;
//> COUT_DMG 		done += to_copy;
//> COUT_DMG 	}
//> COUT_DMG }

//> COUT_DMG size_t dev_commodore_pet_get_irq_signals(DevCommodorePet *device, SignalBreakpoint **irq_signals) {
//> COUT_DMG 	assert(device);
//> COUT_DMG 	assert(irq_signals);
//> COUT_DMG 
//> COUT_DMG 	static SignalBreakpoint pet_irq = {
//> COUT_DMG 		.pos_edge = false,
//> COUT_DMG 		.neg_edge = false,
//> COUT_DMG 	};
//> COUT_DMG 
//> COUT_DMG 	if (signal_is_undefined(pet_irq.signal)) {
//> COUT_DMG 		pet_irq = (SignalBreakpoint) {SIGNAL(IRQ_B), false, true};
//> COUT_DMG 	}
//> COUT_DMG 
//> COUT_DMG 	*irq_signals = &pet_irq;
//> COUT_DMG 	return 1;
//> COUT_DMG }

void dev_commodore_pet_diag_mode(DevCommodorePet *device, bool in_diag) {
	assert(device);

	if (in_diag != device->diag_mode) {
		device->diag_mode = in_diag;
		device->diag_toggled = true;
	}
}

//> COUT_DMG bool dev_commodore_pet_load_prg(DevCommodorePet* device, const char* filename, bool use_prg_address) {
//> COUT_DMG 
//> COUT_DMG 	int8_t * prg_buffer = NULL;
//> COUT_DMG 	size_t prg_size = file_load_binary(filename, &prg_buffer);
//> COUT_DMG 
//> COUT_DMG 	if (prg_size == 0) {
//> COUT_DMG 		arrfree(prg_buffer);
//> COUT_DMG 		return false;
//> COUT_DMG 	}
//> COUT_DMG 
//> COUT_DMG 	uint16_t ram_offset = 0x401;
//> COUT_DMG 	if (use_prg_address) {
//> COUT_DMG 		ram_offset = *((uint16_t*)prg_buffer);
//> COUT_DMG 	}
//> COUT_DMG 
//> COUT_DMG 
//> COUT_DMG 	device->write_memory(device, ram_offset, prg_size - 2, (uint8_t *) prg_buffer + 2);
//> COUT_DMG 
//> COUT_DMG 	arrfree(prg_buffer);
//> COUT_DMG 	return true;
//> COUT_DMG }
