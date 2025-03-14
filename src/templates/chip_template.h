// chip_template.h - Daniel Molina - BSD-3-Clause (see LICENSE)
//
// Template to create new chips
//>> All lines beggining with //>> Are instructions to create new chip.
//>> Remove then from your final code for clarity


//>> Check to avoid double loading. Update <TEMPLATE> to proper name
#ifndef DROMAIUS_CHIP_TEMPLATE_H
#define DROMAIUS_CHIP_TEMPLATE_H

//>> Basic includes. No more should be needed
#include "chip.h"
#include "signal_line.h"

#ifdef __cplusplus
extern "C" {
#endif

//>> Pinout. Just pin numbers for now
//>> For clarity, include small comment with pin function
//>> Replace "TEMPLATE" for your chip name
// types
typedef enum {
	CHIP_TEMPLATE_CLK_OUT = CHIP_PIN_01,			// 1-bit clock signal
} TemplateSignalAssignment;

//>> Pin Count. Update accordingly
#define CHIP_TEMPLATE_PIN_COUNT 1
typedef Signal TemplateSignals[CHIP_OSCILLATOR_PIN_COUNT];

//>> Chip Struct. Update name add any needed internal variables 
//>>       down in the data section
typedef struct Template {

	CHIP_DECLARE_BASE

	// interface
	SignalPool *		signal_pool;
	OscillatorSignals	signals;

	// data
	int64_t				frequency;
} Template;

// functions
//>> Chip creation declaration. Add any needed parameters
Template *template_create(int64_t frequency, struct Simulator *sim, TemplateSignals signals);

#ifdef __cplusplus
}
#endif

//>> Check to avoid double loading. Update <TEMPLATE> to proper name
#endif // DROMAIUS_CHIP_OSCILLATOR_H
