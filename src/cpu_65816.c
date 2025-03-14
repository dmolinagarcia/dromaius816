// cpu_65816.c - Daniel Molina - BSD-3-Clause (see LICENSE)
// based on
// cpu_6502.c - Johan Smet - BSD-3-Clause (see LICENSE)
//
// Emulation of the WDC 65816

#include "log.h"
#include "cpu_65816.h"
#include "cpu_65816_opcodes.h"
#include "simulator.h"
#include "crt.h"

#define SIGNAL_OWNER		cpu
#define SIGNAL_PREFIX		PIN_65816_

//> TODO_DMG Include timing diagram here

//> TODO_DMG Status register, stack, and return from interrupts. 
//> TODO_DMG What is pushed and restored!

//////////////////////////////////////////////////////////////////////////////
//
// internal data types
//

//> TODO_DMG PinTypes Declaration
//> TODO_DMG Known pins ported directly from the 6502
//> TODO_DMG Unknown pins commented as such. Pending review

static uint8_t Cpu65816_PinTypes[CHIP_65816_PIN_COUNT] = {
    [PIN_65816_VPB  ] = CHIP_PIN_OUTPUT,
		//> TODO_DMG Vector Pull ACTLO. Just output. Should be OK
	[PIN_65816_RDY  ] = CHIP_PIN_INPUT,
		//> TODO_DMG Should be output too?
	[PIN_65816_ABORT] = CHIP_PIN_INPUT,
		//> TODO_DMG Sames as IRQB, should be OK
		//> TODO_DMG This is a new type of Interrupt
	[PIN_65816_IRQ_B] = CHIP_PIN_INPUT,
	[PIN_65816_MLB  ] = CHIP_PIN_OUTPUT,
		//> TODO_DMG Memory Lock ACTLO. Output should be OK
	[PIN_65816_NMI_B] = CHIP_PIN_INPUT,
        //> TODO_DMG Should be trigger too??
	[PIN_65816_VPA  ] = CHIP_PIN_OUTPUT,
		//> TODO_DMG VPA output. Should be OK
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
		//> TODO_DMG Emulation status. Should be OK
	[PIN_65816_BE   ] = CHIP_PIN_INPUT | CHIP_PIN_TRIGGER,
		//> TODO_DMG Bus Enable. Asyc should should be trigger?
	[PIN_65816_PHI2 ] = CHIP_PIN_INPUT | CHIP_PIN_TRIGGER,
	[PIN_65816_MX	] = CHIP_PIN_OUTPUT,
		//> TODO_DMG MX status, toggles betweeh PIHI2 H and L
	[PIN_65816_VDA  ] = CHIP_PIN_OUTPUT,
		//> TODO_DMG VPA output. Should be OK
	[PIN_65816_RES_B] = CHIP_PIN_INPUT | CHIP_PIN_TRIGGER,
};

#define RW_READ  true
#define RW_WRITE false

typedef enum CPU_65816_CYCLE {
	CYCLE_BEGIN = 0,		// (1) in timing diagram
	CYCLE_MIDDLE = 1,		// (2) in timing diagram
	CYCLE_END = 2			// (3) in timing diagram
} CPU_65816_CYCLE;

const char* CPU_65816_PHASE_NAMES[] = {
    "CYCLE_BEGIN ---- ",
    "CYCLE_MIDDLE     ",
    "CYCLE_END        "
};
	
//> TODO_DMG. I may need more steps to emulate the CPU

typedef enum CPU_65816_STATE {
	CS_INIT = 0,
	CS_RUNNING = 1,
	CS_IN_IRQ = 2,
	CS_IN_NMI = 3
} CPU_65816_STATE;
	//> TODO_DMG. I may need more states too!

typedef enum CPU_65816_INTERRUPT_TYPE {
	INTR_RESET = 0,
	INTR_BRK = 1,
	INTR_IRQ = 2,
	INTR_NMI = 3
} CPU_65816_INTERRUPT_TYPE;
    //> TODO_DMG and for sure, at least one more interrupt time. ABORT.

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

	bool 			vpa;				// vpa PIN

	//> TODO_DMG. Here we remove SYNC and replace it with VPA
	//> TODO_DMG. VDA and VPA Perform similar function to sync
	//> TODO_DMG. On the original code this output is implemented in the
	//> TODO_DMG. process_end function
	//> TODO_DMG. Only VPA for now to provide for step instruction!

} output_t;

typedef struct Cpu65816_private {
	Cpu65816		intf;

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

	uint32_t        cycles;
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
	FLAG_SET_CLEAR_U32(cpu->reg_p, FLAG_65816_##flag, (cond))

#define CPU_REG_P \
	((cpu->reg_p >> (8*!(FLAG_IS_SET(cpu->reg_p, FLAG_65816_E))) ) & 0xFF)

	// Update CPU outputs at process end
static void process_end(Cpu65816 *cpu) {
	output_t *output = &PRIVATE(cpu)->output;
	output_t *last_output = &PRIVATE(cpu)->last_output;

	//> TODO_DMG If we are in emulation mode, Stack Pointer HI BYTE is 1
	//> TODO_DMG Also, PBR = 00
	//> TODO_DMG Also, M and X = 1
	//> TODO_DMG Is this correct here?
	if (FLAG_IS_SET(cpu->reg_p, FLAG_65816_E)) {
		cpu->reg_sp = cpu->reg_sp & 0x01FF;
		cpu->reg_pbr = 0;
		CPU_CHANGE_FLAG(M, true);
		CPU_CHANGE_FLAG(X, true);
	}

	//> TODO_DMG Also, if X is 1, high byte of X and Y must be cleared
	//> TODO_DMG Is this a good place?
	//>	TODO_DMG still pending.
    if (FLAG_IS_SET(cpu->reg_p, FLAG_65816_X)) {
		cpu->reg_x = cpu->reg_x & 0x00ff;
		cpu->reg_y = cpu->reg_y & 0x00ff;
	}

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
//> TODO_DMG SYNC pinc does not exist on 65816.
//> TODO_DMG VDA and VPA should be here

	// VPA PIN
	//> TODO_DMG. Temporary solution. VPA replicates SYNC on 6502
	//>    . Used for step instruction in control panel.
	output->vpa = PRIVATE(cpu)->decode_cycle == 0;
	if (output->vpa != last_output->vpa) {
		SIGNAL_WRITE(VPA, output->vpa);
		last_output->vpa = output->vpa;
	}		
}

	// Handle interrupt sequence
static void interrupt_sequence(Cpu65816 *cpu, CPU_65816_CYCLE phase, CPU_65816_INTERRUPT_TYPE irq_type) {

	//> TODO_DMG New interrupt types and vectors
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

	//> TODO_DMG I removed PC_INC and tweakd BRK but... should I undo that?
	//> TODO_DMG Note: PC_INC is already commented out in cpu_6502.c!

	//> TODO_DMG I need bank address here too!
	//> TODO_DMG In native mode interrupts take one more cycle
	//> TODO_DMG As PBR is also pushed!
	//> TODO_DMG RTI should take this into account too

	switch(PRIVATE(cpu)->decode_cycle) {
		case 0 :		// finish previous operation
			if (phase == CYCLE_BEGIN) {
				PRIVATE(cpu)->output.address = cpu->reg_pc;
				LOG (2, "Address bus: %04x", cpu->reg_pc);
			}
			if (phase == CYCLE_END && irq_type == INTR_RESET) {
				//> TODO_DMG Emulation is set to TRUE on reset
				//>     So are M and X
				//>     Is this ok here???
				//>     Also, if E=1, always, PBR=0!!!!
				//>     And.... what about other regs???
				//>    Nothing is done in the original code!
				CPU_CHANGE_FLAG(E, true);	
				CPU_CHANGE_FLAG(M, true);	
				CPU_CHANGE_FLAG(X, true);
			}
			break;
		case 1 :		// force a BRK instruction
			if (phase == CYCLE_BEGIN) {
				cpu->reg_ir = OP_65816_BRK;
			}
			break;
		case 2 :		// push high byte of PC
			switch (phase) {
				case CYCLE_BEGIN:
					PRIVATE(cpu)->output.address = MAKE_WORD(0x01, cpu->reg_sp);
					LOG (2, "Address bus: %04x", MAKE_WORD(0x01, cpu->reg_sp));
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
					LOG (2, "Address bus: %04x", MAKE_WORD(0x01, cpu->reg_sp));
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
					LOG (2, "Address bus: %04x", MAKE_WORD(0x01, cpu->reg_sp));					
					PRIVATE(cpu)->output.rwb = RW_WRITE | FORCE_READ[irq_type];
					break;
				case CYCLE_MIDDLE:
					//> TODO_DMG reg_p, reg_ep, how to handle this!
					//>   I think it already is handled :)
					//> TODO_DMG, or probably it is not .(
					OUTPUT_DATA(CPU_REG_P);
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
				LOG (2, "Address bus: %04x", VECTOR_LOW[irq_type]);				
			} else if (phase == CYCLE_END) {
				cpu->reg_pc = SET_LO_BYTE(cpu->reg_pc, PRIVATE(cpu)->in_data);
			}
			break;
		case 6 :		// read high byte of the reset vector
			if (phase == CYCLE_BEGIN) {
				PRIVATE(cpu)->output.address = VECTOR_HIGH[irq_type];
				LOG (2, "Address bus: %04x", VECTOR_HIGH[irq_type]);				
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
// internal memory functions
//

static inline void fetch_memory(Cpu65816 *cpu, uint16_t addr, uint8_t *dst, CPU_65816_CYCLE phase) {
	assert(cpu);
	assert(dst);

	switch (phase) {
		case CYCLE_BEGIN :
			PRIVATE(cpu)->output.address = addr;
			OUTPUT_DATA(cpu->reg_pbr);
			break;
		case CYCLE_MIDDLE:
			break;
		case CYCLE_END :
			*dst = PRIVATE(cpu)->in_data;
			break;
	}
}

	// Fetch PC into *dst
static inline void fetch_pc_memory(Cpu65816 *cpu, uint8_t *dst, CPU_65816_CYCLE phase) {
	// Fetch operation
	// CYCLE_BEGIN: PC as output address
	// CYCLE_MIDDLE: Wait
	// CYCLE_END: Read value stored into requested register

	assert(cpu);
	assert(dst);

	switch (phase) {
		case CYCLE_BEGIN :
			LOG (2, "Fetching %04x", cpu->reg_pc);
			PRIVATE(cpu)->output.address = cpu->reg_pc;
			OUTPUT_DATA(cpu->reg_pbr);
				//> TODO_DMG BANK ADDRESS?
			break;
		case CYCLE_MIDDLE:
			break;
		case CYCLE_END :
			LOG (2, "Fetched: %02x", PRIVATE(cpu)->in_data);
			*dst = PRIVATE(cpu)->in_data;
			// LOG (1, "%02x:%04x" , cpu->reg_pbr, cpu->reg_pc );
			++cpu->reg_pc;
			break;
	}
}


static inline int fetch_address_immediate(Cpu65816 *cpu, CPU_65816_CYCLE phase) {
	assert(cpu);

	if (PRIVATE(cpu)->decode_cycle == 1 || PRIVATE(cpu)->decode_cycle == 2) {
		// Cycle 1 for 8 bit fetch, cycle 2 for 16 bit fetch
		switch (phase) {
			case CYCLE_BEGIN :
				PRIVATE(cpu)->addr.full = cpu->reg_pc;
				OUTPUT_DATA(cpu->reg_pbr);
				//> TODO_DMG BANK ADDRESS?
				break;
			case CYCLE_MIDDLE:
				break;
			case CYCLE_END :
				++cpu->reg_pc;
				break;
		}
	}

	return 1;
}

//> TODO_DMG... this... theres a bunch of fetch_address not implemented
static inline int fetch_address_shortcut(Cpu65816 *cpu, ADDR_MODES_65816 mode, CPU_65816_CYCLE phase) {

	switch (mode) {
		case imme:
			// 8bit immediate
			return fetch_address_immediate(cpu, phase);
//>		case AM_65816_ZEROPAGE:
//>			return fetch_address_zeropage(cpu, phase);
//>		case AM_65816_ZEROPAGE_X:
//>			return fetch_address_zeropage_indexed(cpu, phase, cpu->reg_x);
//>		case AM_65816_ZEROPAGE_Y:
//>			return fetch_address_zeropage_indexed(cpu, phase, cpu->reg_y);
//>		case AM_65816_ABSOLUTE:
//>			return fetch_address_absolute(cpu, phase);
//>		case AM_65816_ABSOLUTE_X:
//>			return fetch_address_absolute_indexed_shortcut(cpu, phase, cpu->reg_x);
//>		case AM_65816_ABSOLUTE_Y:
//>			return fetch_address_absolute_indexed_shortcut(cpu, phase, cpu->reg_y);
//>		case AM_65816_INDIRECT:
//>			return fetch_address_indirect(cpu, phase);
//>		case AM_65816_INDIRECT_X:
//>			return fetch_address_indexed_indirect(cpu, phase);
//>		case AM_65816_INDIRECT_Y:
//>			return fetch_address_indirect_indexed_shortcut(cpu, phase);
		default:
			return 0;
	};
}


static inline bool fetch_operand(Cpu65816 *cpu, ADDR_MODES_65816 mode, CPU_65816_CYCLE phase) {

	bool result = false;

	int memop_cycle = fetch_address_shortcut(cpu, mode, phase) ;

	if (memop_cycle <= PRIVATE(cpu)->decode_cycle) {
		fetch_memory(cpu, PRIVATE(cpu)->addr.full, &PRIVATE(cpu)->operand, phase);
		result = phase == CYCLE_END;
	}

	return result;
}

static inline uint8_t stack_pop(Cpu65816 *cpu, CPU_65816_CYCLE phase) {
	//> TODO_DMG copied as is
	//>     sp is hardcoded, possible bug in native!
	//>     check all sp related access
	uint8_t result = 0;

	switch (phase) {
		case CYCLE_BEGIN:
			cpu->reg_sp++;
			PRIVATE(cpu)->output.address = MAKE_WORD(0x01, cpu->reg_sp);
			break;
		case CYCLE_MIDDLE:
			break;
		case CYCLE_END:
			result = PRIVATE(cpu)->in_data;
			break;
	}

	return result;
}



//////////////////////////////////////////////////////////////////////////////
//
// Instruction decoding
//

// Define the decode array
typedef void (*OpcodeHandler)();
OpcodeHandler opcodeTable[256];  

/* STATUS MATRIX
 [ ] 00 [ ] 01 [ ] 02 [ ] 03 [ ] 04 [ ] 05 [ ] 06 [ ] 07 [ ] 08 [ ] 09 [ ] 0A [ ] 0B [ ] 0C [ ] 0D [ ] 0E [ ] 0F
 [ ] 10 [ ] 11 [ ] 12 [ ] 13 [ ] 14 [ ] 15 [ ] 16 [ ] 17 [ ] 18 [ ] 19 [ ] 1A [ ] 1B [ ] 1C [ ] 1D [ ] 1E [ ] 1F
 [ ] 20 [ ] 21 [ ] 22 [ ] 23 [ ] 24 [ ] 25 [ ] 26 [ ] 27 [ ] 28 [ ] 29 [ ] 2A [ ] 2B [ ] 2C [ ] 2D [ ] 2E [ ] 2F
 [ ] 30 [ ] 31 [ ] 32 [ ] 33 [ ] 34 [ ] 35 [ ] 36 [ ] 37 [ ] 38 [ ] 39 [ ] 3A [ ] 3B [ ] 3C [ ] 3D [ ] 3E [ ] 3F
 [ ] 40 [ ] 41 [ ] 42 [ ] 43 [ ] 44 [ ] 45 [ ] 46 [ ] 47 [ ] 48 [ ] 49 [ ] 4A [ ] 4B [ ] 4C [ ] 4D [ ] 4E [ ] 4F
 [ ] 50 [ ] 51 [ ] 52 [ ] 53 [ ] 54 [ ] 55 [ ] 56 [ ] 57 [ ] 58 [ ] 59 [ ] 5A [ ] 5B [ ] 5C [ ] 5D [ ] 5E [ ] 5F
 [ ] 60 [ ] 61 [ ] 62 [ ] 63 [ ] 64 [ ] 65 [ ] 66 [ ] 67 [ ] 68 [ ] 69 [ ] 6A [ ] 6B [ ] 6C [ ] 6D [ ] 6E [ ] 6F
 [ ] 70 [ ] 71 [ ] 72 [ ] 73 [ ] 74 [ ] 75 [ ] 76 [ ] 77 [ ] 78 [ ] 79 [ ] 7A [ ] 7B [ ] 7C [ ] 7D [ ] 7E [ ] 7F
 [ ] 80 [ ] 81 [ ] 82 [ ] 83 [ ] 84 [ ] 85 [ ] 86 [ ] 87 [ ] 88 [ ] 89 [ ] 8A [ ] 8B [ ] 8C [ ] 8D [ ] 8E [ ] 8F
 [ ] 90 [ ] 91 [ ] 92 [ ] 93 [ ] 94 [ ] 95 [ ] 96 [ ] 97 [ ] 98 [ ] 99 [ ] 9A [ ] 9B [ ] 9C [ ] 9D [ ] 9E [ ] 9F
 [ ] A0 [ ] A1 [ ] A2 [ ] A3 [ ] A4 [ ] A5 [ ] A6 [ ] A7 [ ] A8 [x] A9 [ ] AA [ ] AB [ ] AC [ ] AD [ ] AE [ ] AF
 [ ] B0 [ ] B1 [ ] B2 [ ] B3 [ ] B4 [ ] B5 [ ] B6 [ ] B7 [ ] B8 [ ] B9 [ ] BA [ ] BB [ ] BC [ ] BD [ ] BE [ ] BF
 [ ] C0 [ ] C1 [x] C2 [ ] C3 [ ] C4 [ ] C5 [ ] C6 [ ] C7 [ ] C8 [ ] C9 [ ] CA [ ] CB [ ] CC [ ] CD [ ] CE [ ] CF
 [ ] D0 [ ] D1 [ ] D2 [ ] D3 [ ] D4 [ ] D5 [ ] D6 [ ] D7 [ ] D8 [ ] D9 [ ] DA [ ] DB [ ] DC [ ] DD [ ] DE [ ] DF
 [ ] E0 [ ] E1 [x] E2 [ ] E3 [ ] E4 [ ] E5 [ ] E6 [ ] E7 [ ] E8 [ ] E9 [x] EA [ ] EB [ ] EC [ ] ED [ ] EE [ ] EF
 [ ] F0 [ ] F1 [ ] F2 [ ] F3 [ ] F4 [ ] F5 [ ] F6 [ ] F7 [ ] F8 [ ] F9 [ ] FA [x] FB [ ] FC [ ] FD [ ] FE [ ] FF
*/

void op_BRK(Cpu65816 *cpu, CPU_65816_CYCLE phase) {
	//> TODO_DMG Copied as is
	//>     Return address is off by 1 byte it seems
	//>     Monitor the stack ant test verify
	//> Added increment to reg_pc to fix this.
	if (PRIVATE(cpu)->decode_cycle == 1 && phase == CYCLE_BEGIN) {
		CPU_CHANGE_FLAG(B, true);
		++cpu->reg_pc;
	}
	interrupt_sequence(cpu, phase, INTR_BRK);
}

void op_RTI(Cpu65816 *cpu, CPU_65816_CYCLE phase) {
	//> TODO_DMG Copied as is
	//> TODO_DMG bad recovery of status register!
	switch (PRIVATE(cpu)->decode_cycle) {
		case 1 :		// decode RTI
			fetch_pc_memory(cpu, &PRIVATE(cpu)->addr.lo_byte, phase);
			break;
		case 2 :		// "increment" stack pointer
			if (phase == CYCLE_BEGIN) {
				//> TODO_DMG another hardcoded SP!
				PRIVATE(cpu)->output.address = MAKE_WORD(0x01, cpu->reg_sp);
			}
			break;
		case 3 :		// pop status register
			cpu->reg_p = stack_pop(cpu, phase);
			if (phase == CYCLE_END) {
				CPU_CHANGE_FLAG(B, false);
			}
			break;
		case 4 :		// pop program_counter - low byte
			cpu->reg_pc = SET_LO_BYTE(cpu->reg_pc, stack_pop(cpu, phase));
			break;
		case 5 :		// pop program_counter - low byte
			cpu->reg_pc = SET_HI_BYTE(cpu->reg_pc, stack_pop(cpu, phase));
			if (phase == CYCLE_END) {
				PRIVATE(cpu)->decode_cycle = -1;
			}
			break;
	}
}

void op_NOP(Cpu65816 *cpu, CPU_65816_CYCLE phase) { 
	// Opcodes EA - NOP [EN]
	// Always two cycles. FETCH and EXECUTE
	// FETCH is always handlesd in the execute phase functions
	// SO only EXECUTE is here (Decode Cycle 1)
	// As it is a 2 cycle instruction, no decode cycle decoding is needed
	//> TODO_DMG Maybe too much information here?

	switch (phase) {
		case CYCLE_BEGIN:
			PRIVATE(cpu)->output.address = cpu->reg_pc;
			LOG (2, "Address bus: %04x", cpu->reg_pc);
			OUTPUT_DATA(cpu->reg_pbr);
				//> TODO_DMG BANK ADDRESS?
				//>     According to data sheet, pbr seems correct here
			break;
		case CYCLE_MIDDLE:
			break;
		case CYCLE_END :
			PRIVATE(cpu)->decode_cycle = -1;
			LOG (2, "NOP Opcode %02x executed", cpu->reg_ir);
			break;
	}
}

void op_XCE(Cpu65816 *cpu, CPU_65816_CYCLE phase) { 
	// Opcodes FB - XCE [EN]
	// Always two cycles. FETCH and EXECUTE
	// FETCH is always handled in the execute phase functions
	// As it is a 2 cycle instruction, no decode cycle decoding is needed
	// eXchanges Carry and Emulation

	switch (phase) {
		case CYCLE_BEGIN:
			PRIVATE(cpu)->output.address = cpu->reg_pc;
			LOG (2, "Address bus: %04x", cpu->reg_pc);
			OUTPUT_DATA(cpu->reg_pbr);
				//> TODO_DMG BANK ADDRESS?
				//>     According to data sheet, pbr seems correct here
			break;
		case CYCLE_MIDDLE:
			break;
		case CYCLE_END :
			PRIVATE(cpu)->decode_cycle = -1;
			LOG (2, "XCE Opcode %02x executed", cpu->reg_ir);
			bool keep_carry = FLAG_IS_SET(cpu->reg_p, FLAG_65816_C);
			CPU_CHANGE_FLAG(C, FLAG_IS_SET(cpu->reg_p, FLAG_65816_E));
			CPU_CHANGE_FLAG(E, keep_carry);
	}
}

void op_REP_SEP(Cpu65816 *cpu, CPU_65816_CYCLE phase, ADDR_MODES_65816 mode) {
	//> TODO_DMG My very first opcode with operand!	
	//>     SEP is always 3 cycles. Fetch, fetch operand, and action
	//>		According to the datasheet, VPA is low during last cycle
	//>     Also, I can recycle this for REP!
	//>     Does it depend on processor mode? MX, -B?
	//>     Nothing on the docs related to this. Testing necessary.

	switch (PRIVATE(cpu)->decode_cycle) {
		case 1:
			if (fetch_operand (cpu, mode, phase)) { //> TODO_DMG
					// Nothing!
					// decode cycle advances on its own!
					// SEP takes naother cycle becauase.... no reason?				
			}
			break;
		case 2:
			switch (phase) {
				case CYCLE_BEGIN:
					PRIVATE(cpu)->output.address = cpu->reg_pc;
					OUTPUT_DATA(cpu->reg_pbr);
						//> TODO_DMG BANK ADDRESS?
						//>     According to data sheet, pbr seems correct here
					break;
				case CYCLE_MIDDLE:
					break;
				case CYCLE_END :
					PRIVATE(cpu)->decode_cycle = -1;
					bool new_flag = false; // Default, if REP do not change
					if (cpu->reg_ir==OP_65816_SEP) new_flag = true;
					if (PRIVATE(cpu)->operand & 0b00000001) CPU_CHANGE_FLAG(C, new_flag);
					if (PRIVATE(cpu)->operand & 0b00000010) CPU_CHANGE_FLAG(Z, new_flag);
					if (PRIVATE(cpu)->operand & 0b00000100) CPU_CHANGE_FLAG(I, new_flag);
					if (PRIVATE(cpu)->operand & 0b00001000) CPU_CHANGE_FLAG(D, new_flag);
					if (PRIVATE(cpu)->operand & 0b00010000) CPU_CHANGE_FLAG(X, new_flag);
					if (PRIVATE(cpu)->operand & 0b00100000) CPU_CHANGE_FLAG(M, new_flag);
					if (PRIVATE(cpu)->operand & 0b01000000) CPU_CHANGE_FLAG(V, new_flag);
					if (PRIVATE(cpu)->operand & 0b10000000) CPU_CHANGE_FLAG(N, new_flag);
					break;
			}			
	}

} 

void op_LDA(Cpu65816 *cpu, CPU_65816_CYCLE phase, ADDR_MODES_65816 mode) {
	//> TODO LDA: Just imme for now
	//> Can I handle everything here

	int cycles;
	// Determine amount of cycles
	// Used on 8 vs 16 bit fetches
	if (mode == imme) cycles = 1;

		//> TODO_DMG <= maybe? for longer executions?
	if ((PRIVATE(cpu)->decode_cycle) == cycles) {
		if (fetch_operand(cpu, mode, phase)) {
			// Once fetch is done, store low byte)
			cpu->reg_a = (cpu->reg_a & 0xFF00) | (PRIVATE(cpu)->operand & 0xFF);
			if (FLAG_IS_SET(cpu->reg_p, FLAG_65816_M)) {
				// M is set. ACC is 8 bit, end process
				CPU_CHANGE_FLAG(Z, cpu->reg_a == 0);
				CPU_CHANGE_FLAG(N, BIT_IS_SET(cpu->reg_a, 7));
				PRIVATE(cpu)->decode_cycle = -1;
			} // IF M is unset, advance
		}
	} else if ((PRIVATE(cpu)->decode_cycle) == cycles + 1 ) {
		// Time to fetch next byte!
		if (fetch_operand(cpu, mode, phase)) {
			// Combinar para formar el valor de 16 bits en el acumulador.
			//> TODO_DMG Spanish!
			cpu->reg_a = (PRIVATE(cpu)->operand << 8) | (cpu->reg_a & 0x00FF);
			CPU_CHANGE_FLAG(Z, cpu->reg_a == 0);
			CPU_CHANGE_FLAG(N, (cpu->reg_a & 0x8000) != 0);
			PRIVATE(cpu)->decode_cycle = -1;
		}
	} 
}

void op_LDX(Cpu65816 *cpu, CPU_65816_CYCLE phase, ADDR_MODES_65816 mode) {
	//> TODO LDX: Just imme for now
	//> Can I handle everything here

	int cycles;
	// Determine amount of cycles
	// Used on 8 vs 16 bit fetches
	if (mode == imme) cycles = 1;

	//> TODO_DMG is == enough or should it be <= ?
	if ((PRIVATE(cpu)->decode_cycle) == cycles) {
		if (fetch_operand(cpu, mode, phase)) {
			// Once fetch is done, store low byte)
			cpu->reg_x = (cpu->reg_x & 0xFF00) | (PRIVATE(cpu)->operand & 0xFF);
			if (FLAG_IS_SET(cpu->reg_p, FLAG_65816_X)) {
				// M is set. ACC is 8 bit, end process
				CPU_CHANGE_FLAG(Z, cpu->reg_x == 0);
				CPU_CHANGE_FLAG(N, BIT_IS_SET(cpu->reg_x, 7));
				PRIVATE(cpu)->decode_cycle = -1;
			} // IF X is unset, advance
		}
	} else if ((PRIVATE(cpu)->decode_cycle) == cycles + 1 ) {
		// Time to fetch next byte!
		if (fetch_operand(cpu, mode, phase)) {
			// Combinar para formar el valor de 16 bits en el acumulador.
			//> TODO_DMG spanish!
			cpu->reg_x = (PRIVATE(cpu)->operand << 8) | (cpu->reg_x & 0x00FF);
			CPU_CHANGE_FLAG(Z, cpu->reg_x == 0);
			CPU_CHANGE_FLAG(N, (cpu->reg_x & 0x8000) != 0);
			PRIVATE(cpu)->decode_cycle = -1;
		}
	} 
}

void op_UNK(Cpu65816 *cpu, CPU_65816_CYCLE phase) { 
	//> TODO_DMG all unknowns are treated as NOPs
	switch (phase) {
		case CYCLE_BEGIN:
			PRIVATE(cpu)->output.address = cpu->reg_pc;
			LOG (2,"Address bus: %04x", cpu->reg_pc);
			OUTPUT_DATA(cpu->reg_pbr);
				//> TODO_DMG BANK ADDRESS?
			break;
		case CYCLE_MIDDLE:
			break;
		case CYCLE_END :
			PRIVATE(cpu)->decode_cycle = -1;
			LOG (2,"UNK Opcode %02x executed", cpu->reg_ir);
			break;
	}
}

static inline void CPU_65816_execute_phase(Cpu65816 *cpu, CPU_65816_CYCLE phase) {
	//> TODO_DMG Function copied as-is. I may need some changes!
	LOG (3, "%s Decode_cycle: %d IR: %x" , CPU_65816_PHASE_NAMES[phase], PRIVATE(cpu)->decode_cycle, cpu->reg_ir);

	// data-bus
	PRIVATE(cpu)->output.drv_data = false;
	PRIVATE(cpu)->in_data = SIGNAL_GROUP_READ_U8(data);

	// initialization is treated seperately
	if (PRIVATE(cpu)->state == CS_INIT) {
		execute_init(cpu, phase);
		return;
	}

	// check for interrupts between instructions
	//> How do interrupts work? When are they processed?
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
		if (phase == CYCLE_BEGIN) {
			LOG (3, "Fetching into IR");
		}
		fetch_pc_memory(cpu, &cpu->reg_ir, phase);
	} else {
		opcodeTable[cpu->reg_ir](cpu, phase, ADDRESS_MODES_MATRIX_65816[cpu->reg_ir]);
	}
}

void CPU_65816_override_next_instruction_address(Cpu65816 *cpu, uint16_t pc) {
	//> TODO_DMG Function copied as-is. I may need some changes!
	assert(cpu);
	PRIVATE(cpu)->override_pc = pc;
}

bool CPU_65816_at_start_of_instruction(Cpu65816 *cpu) {
	//> TODO_DMG Function copied as-is. I may need some changes!

	assert(cpu);
	return SIGNAL_READ_NEXT(VPA);
	//> TODO_DMG SHould return a valid combination of VPA and VDA
	//> //> 	return SIGNAL_READ_NEXT(SYNC);
	//>     This is temporary and mimics 6502 SYNC
	//>     USed on the ui for stepping instructions
}

bool CPU_65816_irq_is_asserted(Cpu65816 *cpu) {
	assert(cpu);
	return ACTLO_ASSERTED(SIGNAL_READ_NEXT(IRQ_B));
}

int64_t CPU_65816_program_counter(Cpu65816 *cpu) {
	//> TODO_DMG Function copied as IS. 
	//> TODO_DMG IDN why this is needed.
	//> TODO_DMG IDN Why this is needed in the UI. Something related to getting the PC
	assert(cpu);
	return cpu->reg_pc;
}

uint32_t cpu_65816_model_number(void *cpu) {
	assert(cpu);
	return 65816;
}

uint32_t cpu_65816_get_cycles(void *cpu) {
	assert (cpu);
	return PRIVATE(cpu)->cycles;
}

//////////////////////////////////////////////////////////////////////////////
//
// interface functions
//

static void CPU_65816_destroy(Cpu65816 *cpu);
static void CPU_65816_process(Cpu65816 *cpu);

Cpu65816 *cpu_65816_create(Simulator *sim, Cpu65816Signals signals) {

	//> TODO. What happens with SP on reset? And on native/emulation switching?

	Cpu65816_private *priv = (Cpu65816_private *) dms_calloc(1, sizeof(Cpu65816_private));
	Cpu65816 *cpu = &priv->intf;

	
	CHIP_SET_FUNCTIONS(cpu, CPU_65816_process, CPU_65816_destroy);
	CHIP_SET_VARIABLES(cpu, sim, cpu->signals, Cpu65816_PinTypes, CHIP_65816_PIN_COUNT);

	cpu->signal_pool = sim->signal_pool;
	
	//> cpu->override_next_instruction_address = (CPU_OVERRIDE_NEXT_INSTRUCTION_ADDRESS) CPU_65816_override_next_instruction_address;
	cpu->is_at_start_of_instruction = (CPU_IS_AT_START_OF_INSTRUCTION) CPU_65816_at_start_of_instruction;
	//> cpu->irq_is_asserted = (CPU_IRQ_IS_ASSERTED) CPU_65816_irq_is_asserted;
	cpu->program_counter = (CPU_PROGRAM_COUNTER) CPU_65816_program_counter;
	//> TODO_DMG These are commented out. Not yet implemented... what do they do?
	//> TODO_DMG iasof and pc uncommented due to them being used in context ui
	//> TODO_DMG pointes to functions? Common interface accross CPUS?
	cpu->model_number = (CPU_MODEL_NUMBER) cpu_65816_model_number;
	cpu->get_cycles = (CPU_GET_CYCLES) cpu_65816_get_cycles;
	
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
	SIGNAL_DEFINE(VPA);
	SIGNAL_DEFINE_DEFAULT(RWB, true);
	SIGNAL_DEFINE(PHI2);
	SIGNAL_DEFINE_DEFAULT(RES_B, ACTLO_DEASSERT);

	priv->decode_cycle = -1;
	priv->state = CS_INIT;
	priv->nmi_triggered = false;
	priv->override_pc = 0;
	priv->cycles = 0;

	//> TODO_DMG Also, unused flag in emulation mode is always set
	//>       so we SET it on CPU Creation
	CPU_CHANGE_FLAG(U, true);

	// Init opcode table
		// Default call for unknown opcodes
		for (int i = 0; i < 256; i++) opcodeTable[i] = op_UNK; 

		// Load the opcodes
		opcodeTable[OP_65816_BRK]      = op_BRK;
		opcodeTable[OP_65816_RTI]      = op_RTI;
		opcodeTable[OP_65816_NOP]      = op_NOP;
		opcodeTable[OP_65816_LDX_IMME] = op_LDX;
		opcodeTable[OP_65816_LDA_IMME] = op_LDA;
		opcodeTable[OP_65816_REP]      = op_REP_SEP;
		opcodeTable[OP_65816_SEP]      = op_REP_SEP;
		opcodeTable[OP_65816_XCE]      = op_XCE;

	//> TODO_DMG INit registers
	// how is it done in the 6502?
	
	cpu->reg_pbr = 0;
	
	return cpu;
}

static void CPU_65816_destroy(Cpu65816 *cpu) {
	assert(cpu);
	signal_group_destroy(cpu->sg_address);
	signal_group_destroy(cpu->sg_data);
	dms_free((Cpu65816_private *) cpu);
}


//> TODO_DMG CPU process. 
//> TODO_DMG We're assuming cycle is the same, but with bank register output
//>     During PHI2 low?

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
		priv->cycles++;
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