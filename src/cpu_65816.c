// cpu_65816.c - Daniel Molina - BSD-3-Clause (see LICENSE)
// bases on
// cpu_6502.c - Johan Smet - BSD-3-Clause (see LICENSE)
//
// Emulation of the MOS 65 02 >> soon to be 65816

#include "cpu_65816.h"
#include "cpu_65816_opcodes.h"
#include "simulator.h"
#include "crt.h"

#define SIGNAL_OWNER		cpu
#define SIGNAL_PREFIX		PIN_65816_

//>TODO Include timing diagram here

//////////////////////////////////////////////////////////////////////////////
//
// internal data types
//

//>TODO PinTypes Declaration
//>TODO Know pins ported directly from the 6502
//>TODO Unknown pins commented as such. Pending review

static uint8_t Cpu65816_PinTypes[CHIP_65816_PIN_COUNT] = {
    [PIN_65816_VPB  ] = CHIP_PIN_OUTPUT,
		//>TODO Vector Pull ACTLO. Just output. Should be OK
	[PIN_65816_RDY  ] = CHIP_PIN_INPUT,
		//>TODO Should be output too?
	[PIN_65816_ABORT] = CHIP_PIN_INPUT,
		//>TODO Sames as IRQB, should be OK
		//>TODO This is a new type of Interrupt
	[PIN_65816_IRQ_B] = CHIP_PIN_INPUT,
	[PIN_65816_MLB  ] = CHIP_PIN_OUTPUT,
		//>TODO Memory Lock ACTLO. Output should be OK
	[PIN_65816_NMI_B] = CHIP_PIN_INPUT,
        //>TODO Should be trigger too??
	[PIN_65816_VPA  ] = CHIP_PIN_OUTPUT,
		//>TODO VPA output. Should be OK
	[PIN_65816_AB0  ] = CHIP_PIN_OUTPUT,
	[PIN_65816_AB1  ] = CHIP_PIN_OUTPUT,
	[PIN_65816_AB2  ] = CHIP_PIN_OUTPUT,
	[PIN_65816_AB3  ] = CHIP_PIN_OUTPUT,
	[PIN_65816_AB4  ] = CHIP_PIN_OUTPUT,
	[PIN_65816_AB5  ] = CHIP_PIN_OUTPUT,
	[PIN_65816_AB6  ] = CHIP_PIN_OUTPUT,
	[PIN_65816_AB7  ] = CHIP_PIN_OUTPUT,
	[PIN_65816_AB8  ] = CHIP_PIN_OUTPUT,
	[PIN_65816_AB9  ] = CHIP_PIN_OUTPUT,
	[PIN_65816_AB10 ] = CHIP_PIN_OUTPUT,
	[PIN_65816_AB11 ] = CHIP_PIN_OUTPUT,
	[PIN_65816_AB12 ] = CHIP_PIN_OUTPUT,
	[PIN_65816_AB13 ] = CHIP_PIN_OUTPUT,
	[PIN_65816_AB14 ] = CHIP_PIN_OUTPUT,
	[PIN_65816_AB15 ] = CHIP_PIN_OUTPUT,
	[PIN_65816_DB7  ] = CHIP_PIN_INPUT | CHIP_PIN_OUTPUT,
	[PIN_65816_DB6  ] = CHIP_PIN_INPUT | CHIP_PIN_OUTPUT,
	[PIN_65816_DB5  ] = CHIP_PIN_INPUT | CHIP_PIN_OUTPUT,
	[PIN_65816_DB4  ] = CHIP_PIN_INPUT | CHIP_PIN_OUTPUT,
	[PIN_65816_DB3  ] = CHIP_PIN_INPUT | CHIP_PIN_OUTPUT,
	[PIN_65816_DB2  ] = CHIP_PIN_INPUT | CHIP_PIN_OUTPUT,
	[PIN_65816_DB1  ] = CHIP_PIN_INPUT | CHIP_PIN_OUTPUT,
	[PIN_65816_DB0  ] = CHIP_PIN_INPUT | CHIP_PIN_OUTPUT,
	[PIN_65816_RWB	] = CHIP_PIN_OUTPUT,
	[PIN_65816_E    ] = CHIP_PIN_OUTPUT,
		//>TODO Emulatoin status. Should be OK
	[PIN_65816_BE   ] = CHIP_PIN_INPUT | CHIP_PIN_TRIGGER,
		//>TODO Bus Enable. Asyc should should be trigger?
	[PIN_65816_PHI2 ] = CHIP_PIN_INPUT | CHIP_PIN_TRIGGER,
	[PIN_65816_MX	] = CHIP_PIN_OUTPUT,
		//>TODO MX status, toggles betweeh PIHI2 H and L
	[PIN_65816_VDA  ] = CHIP_PIN_OUTPUT,
		//>TODO VPA output. Should be OK
	[PIN_65816_RES_B] = CHIP_PIN_INPUT | CHIP_PIN_TRIGGER,
};

#define RW_READ  true
#define RW_WRITE false

typedef enum CPU_65816_CYCLE {
	CYCLE_BEGIN = 0,		// (1) in timing diagram
	CYCLE_MIDDLE = 1,		// (2) in timing diagram
	CYCLE_END = 2			// (3) in timing diagram
} CPU_65816_CYCLE;
	//>TODO. I may need more steps to emulate the CPU

typedef enum CPU_65816_STATE {
	CS_INIT = 0,
	CS_RUNNING = 1,
	CS_IN_IRQ = 2,
	CS_IN_NMI = 3
} CPU_65816_STATE;
	//>TODO. I may need more states too!

typedef enum CPU_65816_INTERRUPT_TYPE {
	INTR_RESET = 0,
	INTR_BRK = 1,
	INTR_IRQ = 2,
	INTR_NMI = 3
} CPU_65816_INTERRUPT_TYPE;
    //>TODO and for sure, at least one more interrupt time. ABORT.

typedef union addr_t {
	uint16_t full;
	struct {
		uint8_t lo_byte;
		uint8_t hi_byte;
	};
} addr_t;

typedef struct output_t {
	bool			drv_data;			// should cpu be driving the databus?
	uint8_t			data;				// data to be written to the databus

	uint16_t		address;			// data to be written to the addressbus
	bool			rwb;				// rw pin

	//>TODO. Here we remove SYNC
	//>TODO. VDA and VPA Perform similar function
	//>TODO. On the original code this output is implemented in the
	//>      process_end function

} output_t;

typedef struct Cpu65816_private {
	Cpu65816			intf;

	uint8_t			in_data;				// incoming data (only valid when signal rw is high)
	output_t		output;
	output_t		last_output;

	CPU_65816_STATE	state;
	int8_t			decode_cycle;			// instruction decode cycle
	uint16_t		override_pc;			// != 0: fetch next instruction from this location, overruling the current pc
	uint8_t			operand;
	addr_t			addr;
	addr_t			i_addr;
	bool			page_crossed;
	bool			nmi_triggered;

	bool			delayed_cycle;

	uint16_t		last_out_address;
} Cpu65816_private;



bool CPU_65816_at_start_of_instruction(Cpu65816 *cpu) {
	assert(cpu);
	return true;
	//>TODO SHould return a valid combination of VPA and VDA
	//>TODO IDN Why this is needed in the UI. Something related to getting the PC
}


int64_t CPU_65816_program_counter(Cpu65816 *cpu) {
	//>TODO Function copied as IS. 
	//>TODO IDN why this is needed.
	assert(cpu);
	return cpu->reg_pc;
}

//////////////////////////////////////////////////////////////////////////////
//
// interface functions
//

static void CPU_65816_destroy(Cpu65816 *cpu);
static void CPU_65816_process(Cpu65816 *cpu);

Cpu65816 *cpu_65816_create(Simulator *sim, Cpu65816Signals signals) {

	Cpu65816_private *priv = (Cpu65816_private *) dms_calloc(1, sizeof(Cpu65816_private));
	Cpu65816 *cpu = &priv->intf;

	CHIP_SET_FUNCTIONS(cpu, CPU_65816_process, CPU_65816_destroy);
	CHIP_SET_VARIABLES(cpu, sim, cpu->signals, Cpu65816_PinTypes, CHIP_65816_PIN_COUNT);

	cpu->signal_pool = sim->signal_pool;
	
	
	//> cpu->override_next_instruction_address = (CPU_OVERRIDE_NEXT_INSTRUCTION_ADDRESS) CPU_65816_override_next_instruction_address;
	cpu->is_at_start_of_instruction = (CPU_IS_AT_START_OF_INSTRUCTION) CPU_65816_at_start_of_instruction;
	//> cpu->irq_is_asserted = (CPU_IRQ_IS_ASSERTED) CPU_65816_irq_is_asserted;
	cpu->program_counter = (CPU_PROGRAM_COUNTER) CPU_65816_program_counter;
	//>TODO These are commented out. Not yet implemented... what do they do?
	//>TODO iasof and pc uncommented due to them being used in context ui
	//>TODO pointes to functions? Common interface accross CPUS?

	dms_memcpy(cpu->signals, signals, sizeof(Cpu65816Signals));

	cpu->sg_address = signal_group_create();
	cpu->sg_data = signal_group_create();

	SIGNAL_DEFINE_GROUP(AB0, address);
	SIGNAL_DEFINE_GROUP(AB1, address);
	SIGNAL_DEFINE_GROUP(AB2, address);
	SIGNAL_DEFINE_GROUP(AB3, address);
	SIGNAL_DEFINE_GROUP(AB4, address);
	SIGNAL_DEFINE_GROUP(AB5, address);
	SIGNAL_DEFINE_GROUP(AB6, address);
	SIGNAL_DEFINE_GROUP(AB7, address);
	SIGNAL_DEFINE_GROUP(AB8, address);
	SIGNAL_DEFINE_GROUP(AB9, address);
	SIGNAL_DEFINE_GROUP(AB10, address);
	SIGNAL_DEFINE_GROUP(AB11, address);
	SIGNAL_DEFINE_GROUP(AB12, address);
	SIGNAL_DEFINE_GROUP(AB13, address);
	SIGNAL_DEFINE_GROUP(AB14, address);
	SIGNAL_DEFINE_GROUP(AB15, address);

	SIGNAL_DEFINE_GROUP(DB0, data);
	SIGNAL_DEFINE_GROUP(DB1, data);
	SIGNAL_DEFINE_GROUP(DB2, data);
	SIGNAL_DEFINE_GROUP(DB3, data);
	SIGNAL_DEFINE_GROUP(DB4, data);
	SIGNAL_DEFINE_GROUP(DB5, data);
	SIGNAL_DEFINE_GROUP(DB6, data);
	SIGNAL_DEFINE_GROUP(DB7, data);

	SIGNAL_DEFINE_DEFAULT(RDY, ACTHI_ASSERT);
	SIGNAL_DEFINE_DEFAULT(IRQ_B, ACTLO_DEASSERT);
	SIGNAL_DEFINE_DEFAULT(NMI_B, ACTLO_DEASSERT);
	SIGNAL_DEFINE_DEFAULT(RWB, true);
	SIGNAL_DEFINE(PHI2);
	SIGNAL_DEFINE_DEFAULT(RES_B, ACTLO_DEASSERT);

	priv->decode_cycle = -1;
	priv->state = CS_INIT;
	priv->nmi_triggered = false;
	priv->override_pc = 0;

	return cpu;
}

static void CPU_65816_destroy(Cpu65816 *cpu) {
	assert(cpu);
	signal_group_destroy(cpu->sg_address);
	signal_group_destroy(cpu->sg_data);
	dms_free((Cpu65816_private *) cpu);
}


//>TODO CPU process. Empty for now
static void CPU_65816_process(Cpu65816 *cpu) {
	assert(cpu);

	return;
}