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

//////////////////////////////////////////////////////////////////////////////
//
// internal functions
//

#define PRIVATE(cpu)	((Cpu65816_private *) (cpu))

#define OUTPUT_DATA(d)						\
	PRIVATE(cpu)->output.data = (uint8_t) (d);	\
	PRIVATE(cpu)->output.drv_data = true;

#define CPU_CHANGE_FLAG(flag, cond)			\
	FLAG_SET_CLEAR_U8(cpu->reg_p, FLAG_65816_##flag, (cond))

#define CPU_CHANGE_EFLAG(flag, cond)			\
	FLAG_SET_CLEAR_U8(cpu->reg_ep, FLAG_65816_##flag, (cond))


static void process_end(Cpu65816 *cpu) {
	// Update CPU outputs at process end
	output_t *output = &PRIVATE(cpu)->output;
	output_t *last_output = &PRIVATE(cpu)->last_output;

	//>TODO If we are in emulation mode, Stack Pointer HI BYTE is 1
	//>TODO Is this correct?
		cpu->reg_sp = cpu->reg_sp & 0x01FF;

	// address bus
	if (output->address != last_output->address) {
		SIGNAL_GROUP_WRITE(address, output->address);
		last_output->address = output->address;
	}

	// data bus (tri-state)
	if (output->drv_data) {
		// cpu should be active - write if data changed or if cpu just became active
		if (output->data != last_output->drv_data || !last_output->drv_data) {
			SIGNAL_GROUP_WRITE(data, output->data);
			last_output->data = output->data;
			last_output->drv_data = output->drv_data;
		}
	} else if (last_output->drv_data) {
		// cpu was active last time but not anymore
		SIGNAL_GROUP_NO_WRITE(data);
		last_output->drv_data = false;
	}

	// rw pin
	if (output->rwb != last_output->rwb) {
		SIGNAL_WRITE(RWB, output->rwb);
		last_output->rwb = output->rwb;
	}

	// sync pin
//> 	output->sync = PRIVATE(cpu)->decode_cycle == 0;
//> 	if (output->sync != last_output->sync) {
//> 		SIGNAL_WRITE(SYNC, output->sync);
//> 		last_output->sync = output->sync;
//> 	}
//>TODO SYNC pinc does not exist on 65816.
//>TODO VDA and VPA should be here
}

static void interrupt_sequence(Cpu65816 *cpu, CPU_65816_CYCLE phase, CPU_65816_INTERRUPT_TYPE irq_type) {

	//>TODO New interrupt types and vectors
	static const bool FORCE_READ[] = {
		true,		// reset
		false,		// BRK-instruction
		false,		// regular IRQ
		false		// non-maskable IRQ
	};

	static const uint16_t VECTOR_LOW[] = {
		0xfffc,		// reset
		0xfffe,		// BRK-instruction
		0xfffe,		// regular IRQ
		0xfffa		// non-maskable IRQ
	};

	static const uint16_t VECTOR_HIGH[] = {
		0xfffd,		// reset
		0xffff,		// BRK-instruction
		0xffff,		// regular IRQ
		0xfffb		// non-maskable IRQ
	};

/*	static const uint16_t PC_INC[] = {
		1,			// reset
		1,			// BRK-instruction
		0,			// regular IRQ
		0			// non-maskable IRQ
	}; */

	switch(PRIVATE(cpu)->decode_cycle) {
		case 0 :		// finish previous operation
			if (phase == CYCLE_BEGIN) {
				PRIVATE(cpu)->output.address = cpu->reg_pc;
			}
			break;
		case 1 :		// force a BRK instruction
			if (phase == CYCLE_BEGIN) {
				cpu->reg_ir = OP_65816_BRK;
			}
			//>TODO Emulation is set to TRUE on startup
			//>     Is this ok here???
			CPU_CHANGE_EFLAG(E, true);
			break;
		case 2 :		// push high byte of PC
			switch (phase) {
				case CYCLE_BEGIN:
					PRIVATE(cpu)->output.address = MAKE_WORD(0x01, cpu->reg_sp);
					PRIVATE(cpu)->output.rwb = RW_WRITE | FORCE_READ[irq_type];
					break;
				case CYCLE_MIDDLE:
					OUTPUT_DATA(HI_BYTE(cpu->reg_pc));
					break;
				case CYCLE_END:
					cpu->reg_sp--;
					PRIVATE(cpu)->output.rwb = RW_READ;
					break;
			}
			break;
		case 3 :		// push low byte of PC
			switch (phase) {
				case CYCLE_BEGIN:
					PRIVATE(cpu)->output.address = MAKE_WORD(0x01, cpu->reg_sp);
					PRIVATE(cpu)->output.rwb = RW_WRITE | FORCE_READ[irq_type];
					break;
				case CYCLE_MIDDLE:
					OUTPUT_DATA(LO_BYTE(cpu->reg_pc));
					break;
				case CYCLE_END:
					cpu->reg_sp--;
					PRIVATE(cpu)->output.rwb = RW_READ;
					break;
			}
			break;
		case 4 :		// push processor state
			switch (phase) {
				case CYCLE_BEGIN:
					PRIVATE(cpu)->output.address = MAKE_WORD(0x01, cpu->reg_sp);
					PRIVATE(cpu)->output.rwb = RW_WRITE | FORCE_READ[irq_type];
					break;
				case CYCLE_MIDDLE:
					//>TODO reg_p, reg_ep, how to handle this!
					OUTPUT_DATA(cpu->reg_p);
					break;
				case CYCLE_END:
					cpu->reg_sp--;
					PRIVATE(cpu)->output.rwb = RW_READ;
					break;
			}
			break;
		case 5 :		// read low byte of the reset vector
			if (phase == CYCLE_BEGIN) {
				PRIVATE(cpu)->output.address = VECTOR_LOW[irq_type];
			} else if (phase == CYCLE_END) {
				cpu->reg_pc = SET_LO_BYTE(cpu->reg_pc, PRIVATE(cpu)->in_data);
			}
			break;
		case 6 :		// read high byte of the reset vector
			if (phase == CYCLE_BEGIN) {
				PRIVATE(cpu)->output.address = VECTOR_HIGH[irq_type];
			} else if (phase == CYCLE_END) {
				cpu->reg_pc = SET_HI_BYTE(cpu->reg_pc, PRIVATE(cpu)->in_data);
				CPU_CHANGE_FLAG(I, true);
				PRIVATE(cpu)->state = CS_RUNNING;
				PRIVATE(cpu)->decode_cycle = -1;
			}
			break;
	}
}

static void execute_init(Cpu65816 *cpu, CPU_65816_CYCLE phase) {
	interrupt_sequence(cpu, phase, INTR_RESET);
}


//////////////////////////////////////////////////////////////////////////////
//
// internal fetch functions
//


static inline void fetch_pc_memory(Cpu65816 *cpu, uint8_t *dst, CPU_65816_CYCLE phase) {
	// Fetch operation
	// CYCLE_BEGIN: PC as output address
	// CYCLE_MIDDLE: Wait
	// CYCLE_END: Read value stored into requested register

	assert(cpu);
	assert(dst);

	switch (phase) {
		case CYCLE_BEGIN :
			PRIVATE(cpu)->output.address = cpu->reg_pc;
			break;
		case CYCLE_MIDDLE:
			break;
		case CYCLE_END :
			*dst = PRIVATE(cpu)->in_data;
			++cpu->reg_pc;
			break;
	}
}

static inline void decode_instruction(Cpu65816 *cpu, CPU_65816_CYCLE phase) {
	// Decode adn execute
	//>TODO Everything!
	//> Added NOP execution as a catch all. Let's try

	switch (phase) {
		case CYCLE_BEGIN:
			PRIVATE(cpu)->output.address = cpu->reg_pc;
			break;
		case CYCLE_MIDDLE:
			break;
		case CYCLE_END :
			PRIVATE(cpu)->decode_cycle = -1;
			break;
	}	

}


static inline void CPU_65816_execute_phase(Cpu65816 *cpu, CPU_65816_CYCLE phase) {
	//>TODO Function copied as-is. I may need some changes!

	// data-bus
	PRIVATE(cpu)->output.drv_data = false;
	PRIVATE(cpu)->in_data = SIGNAL_GROUP_READ_U8(data);

	// initialization is treated seperately
	if (PRIVATE(cpu)->state == CS_INIT) {
		execute_init(cpu, phase);
		return;
	}


	// check for interrupts between instructions
	if (PRIVATE(cpu)->decode_cycle == 0 && phase == CYCLE_BEGIN) {
		if (PRIVATE(cpu)->nmi_triggered) {
			PRIVATE(cpu)->state = CS_IN_NMI;
			PRIVATE(cpu)->nmi_triggered = false;
		}
		if (ACTLO_ASSERTED(SIGNAL_READ(IRQ_B)) && !FLAG_IS_SET(cpu->reg_p, FLAG_65816_INTERRUPT_DISABLE)) {
			PRIVATE(cpu)->state = CS_IN_IRQ;
		}
		if (PRIVATE(cpu)->override_pc) {
			cpu->reg_pc = PRIVATE(cpu)->override_pc;
			PRIVATE(cpu)->override_pc = 0;
		}
	}

	// irq starting sequence is handled seperately
	if (PRIVATE(cpu)->state == CS_IN_IRQ) {
		interrupt_sequence(cpu, phase, INTR_IRQ);
		return;
	}

	if (PRIVATE(cpu)->state == CS_IN_NMI) {
		interrupt_sequence(cpu, phase, INTR_NMI);
		return;
	}

	// first cycle is always filling the instruction register with the new opcode
	// IF cycle = 0 (SYNC on 6502 is true) we fetch PC memory into IR
	// Otherwise, we are executing
	if (PRIVATE(cpu)->decode_cycle == 0) {
		fetch_pc_memory(cpu, &cpu->reg_ir, phase);
	} else {
		decode_instruction(cpu, phase);
	}

}


void CPU_65816_override_next_instruction_address(Cpu65816 *cpu, uint16_t pc) {
	//>TODO Function copied as-is. I may need some changes!

	assert(cpu);
	PRIVATE(cpu)->override_pc = pc;
}






bool CPU_65816_at_start_of_instruction(Cpu65816 *cpu) {
	//>TODO Function copied as-is. I may need some changes!

	assert(cpu);
	return true;
	//>TODO SHould return a valid combination of VPA and VDA
	//> //> 	return SIGNAL_READ_NEXT(SYNC);
	//>TODO IDN Why this is needed in the UI. Something related to getting the PC
}


bool CPU_65816_irq_is_asserted(Cpu65816 *cpu) {
	assert(cpu);
	return ACTLO_ASSERTED(SIGNAL_READ_NEXT(IRQ_B));
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


//>TODO CPU process. 
//>TODO We're assuming cycle is the same, but with bank register output
//>     During PHI2 high?

static void CPU_65816_process(Cpu65816 *cpu) {
	assert(cpu);
	Cpu65816_private *priv = (Cpu65816_private *) cpu;

	// check for changes in the reset line
	bool reset_b = SIGNAL_READ(RES_B);

	if (SIGNAL_CHANGED(RES_B)) {
		if (ACTLO_ASSERTED(reset_b)) {
			// reset was just asserted
			priv->output.address = 0;
			priv->output.rwb = RW_READ;
		} else {
			// reset was just deasserted - start initialization sequence
			priv->state = CS_INIT;
			priv->decode_cycle = -1;
			priv->delayed_cycle = false;
		}
	}

	// do nothing:
	//  - if reset is asserted or
	//  - if rdy is not asserted
	if (ACTLO_ASSERTED(reset_b) || !ACTHI_ASSERTED(SIGNAL_READ(RDY))) {
		process_end(cpu);
		return;
	}


	// always check for an nmi request
	if (SIGNAL_CHANGED(NMI_B)) {
		priv->nmi_triggered = priv->nmi_triggered || !SIGNAL_READ(NMI_B);
	}

	// normal processing
	bool clock = SIGNAL_READ(PHI2);
	bool clock_changed = SIGNAL_CHANGED(PHI2);

	if (priv->delayed_cycle) {
		priv->delayed_cycle = false;
		++priv->decode_cycle;
		CPU_65816_execute_phase(cpu, CYCLE_BEGIN);
	} else if (clock && clock_changed) {
		// a positive going clock marks the halfway point of the cycle
		CPU_65816_execute_phase(cpu, CYCLE_MIDDLE);
	} else if (!clock && clock_changed) {
		// a negative going clock ends the previous cycle and starts a new cycle
		CPU_65816_execute_phase(cpu, CYCLE_END);

		// ask to be woken up next timestep to process the delayed part of the negative edge
		priv->delayed_cycle = true;
		cpu->schedule_timestamp = cpu->simulator->current_tick + 1;
	}


	// Update outputs
		process_end(cpu);
	return;
}