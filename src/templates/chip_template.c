// chip_template.c - Daniel Molina - BSD-3-Clause (see LICENSE)
//
// Template to create new chips
//>> All lines beggining with //>> Are instructions to create new chip. r
//>> Remove then from your final code for clarity

//>> Include your own header
#include "chip_template.h"
#include "simulator.h"
#include "crt.h"

//>> Prefix for signal names. Change name
#define SIGNAL_PREFIX		CHIP_TEMPLATE_
//>> SIGNAL_OWNER defines the layer. Not so sure yet. Could be the name of the
//>>   object? Still more investigation needed.
#define SIGNAL_OWNER		tmp

//>> Define the pinout.
//>> CHIP_PIN_OUTPUT, CHIP_PIN_INPUT, CHIP_PIN_TRIGGER are valid options.
static uint8_t ChipTemplate_PinTypes[CHIP_TEMPLATE_PIN_COUNT] = {
	[CHIP_TEMPLATE_CLK_OUT] = CHIP_PIN_OUTPUT
};

//////////////////////////////////////////////////////////////////////////////
//
// interface functions
//

//>> Replace name for functions
static void template_destroy(Template *tmp);
static void template_process(Template *tmp);

//>> Creator
Template *template_create(int64_t frequency, Simulator *sim, TemplateSignals signals) {
	Template *tmp = (Template *) dms_calloc(1, sizeof(Template));

	CHIP_SET_FUNCTIONS(tmp, template_process, template_destroy);
	CHIP_SET_VARIABLES(tmp, sim, tmp->signals, ChipTemplate_PinTypes, CHIP_TEMPLATE_PIN_COUNT);

	tmp->signal_pool = sim->signal_pool;

	dms_memcpy(tmp->signals, signals, sizeof(TemplateSignals));
	SIGNAL_DEFINE_DEFAULT(CLK_OUT, false);


//>> Variables needed on startup
	tmp->frequency = frequency;
	tmp->half_period_ticks = 1000000000000l / (tmp->frequency * 2 * tmp->simulator->tick_duration_ps);
	tmp->tick_next_transition = tmp->half_period_ticks;
	tmp->schedule_timestamp = tmp->tick_next_transition;

	return tmp;
}

static void template_destroy(Template *tmp) {
	assert(tmp);
	dms_free(tmp);
}

//>> Chip process
//>> Activity of the chip should be modelled here

static void template_process(Template *tmp) {
	assert(tmp);

	if (tmp->tick_next_transition <= tmp->simulator->current_tick) {
		SIGNAL_WRITE(CLK_OUT, !SIGNAL_READ(CLK_OUT));
		tmp->tick_next_transition = tmp->simulator->current_tick + tmp->half_period_ticks;
		tmp->schedule_timestamp = tmp->tick_next_transition;
	}
}

