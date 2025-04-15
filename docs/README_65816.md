# Guide for Emulating the 65816 Processor

This document outlines the steps and recommendations for creating a file similar to `cpu_6502.c` but designed to emulate a 65816 processor. It follows the same philosophy of cycle-based emulation, adapting it to the specific features and improvements of the 65816 architecture. A task list is also provided to managed the upgrade from the 6502 to the 65816.

---

## Task List

  - [x] Dissect the `CPU65816` completely. Remove any functionality and everything from its panel.
  - [x] Create PINOUT for the CPU
  - [x] Understand procesor cycle
    - [x] Timing and bus behavior. I need to implement the BankAddress.
    - [x] Should I change process phases?. I may need new steps
  - [x] Understand status register and how it changes between Emulation and Native
    - [x] Implement Status Register
  - [x] Implement OPCODES Phase 1
    - [x] Understand processor phases
    - [x] Initially, everything is a NOP. Advance PC and continue
    - [x] Add a catch all that triggers some warning (Illegal Opcode)
    - [x] Add NOP. Run a free NOP
    - [x] emu816-like logging needs to be implemented here
  - [ ] Explore JMPs
    - [ ] Difference between JMP and JMP
    - [ ] Different addresing modes in jumps
    - [ ] Is there some overlapping?
  - [x] Understand remaining processor registers
    - [x] When and how registers change size?
    - [ ] How does changing size affect the values stored?
    - [ ] Implement remaining registers.
    - [ ] When is an operation 16bit or 8bit? Check BDD's post!
    - [ ] Variable length Instructions
      - [ ] The length of some instructions depends on E,M and X. Understand this
      - [ ] Modify the disassembler to take this into account
      - [ ] Adapt the AJTracer to use the dissassembler to get the instruction, as it will return the operand too.
  - [ ] Implement OPCODES Phase 2
    - [ ] XCE. CLC. SEC. Switch emulation/native
    - [ ] Test status register change on switch
    - [x] Implement decoding logic,
  - [ ] Implement OPCODES Phase 3
    - [ ] Implement instructions that change M/X. REP & SEP
    - [ ] Implement register size changes and how their values change
  - [ ] Understand addresing modes
  - [ ] Implement OPCODES Phase 4
    - [ ] Implement all implied OPCODES
    - [ ] Test Emulation and Native modes
  - [ ] Implement OPCODES Phase 5
    - [ ] Implement all immediate OPCODES
    - [ ] Everything else
  - [ ] Automate testing. https://github.com/SingleStepTests/ProcessorTests/tree/main/65816
  - [ ] IRQ/NMI behaviour
  - [ ] Special PINS.
    - [ ] VDA / VPA
    - [ ] ABORT
    - [ ] E / MX
    - [ ] OThers

## Execution Cycle

Execution cycle is divided in 3 steps. Right after PHI2 falls, the cycle begins with phase CYCLE_BEGIN. Here, AB is prepared and outputs the proper value. Then, CYCLE_MIDDLE starts when PHI2 rises. CPU starts outputting data on the data bus. Then, at PHI2 negative edge, CYCLE_END happends. Instruction is fetched into IR at decode_cycle 0, or any necesary stepts are executed afterwards.

As PHI2 negative edge happens only once, after CYCLE_END a scheduled events is prepared for the next simulation tick, so CYCLE_BEGIN is executed 1 tick after PHI2 negative edge.

Each instruction increments PC, or the decode_cycle as needed. When an instruction ends, decode_cycle is set to -1. During CYCLE_BEGIN it is incremented so next opcode will be fetched here.

As of 02/18/2025 bank address output is not implemented!
 
## Instruction decoder

Although there seems to be some kind of logic built withing the instruction set (There has to be!) the original Dromaius barely manages to group instructions, and the decode function is a massive switch case based on the opcode. I am trying to find some logic within the instruction set, but I can't seem to find any.

ChatGPT suggests building an opcode function matrix. Sounds good but it is untested. Different addresing modes can still call same function, with further decoding within the function.

    ```c
    #include <iostream>
    #include <cstdint>

    // Define the array
    typedef void (*OpcodeHandler)();
    OpcodeHandler opcodeTable[256];  

    // Function to process each opcode

    void op_NOP() { std::cout << "Ejecutando NOP\n"; }
    void op_UNK() { std::cout << "Opcode desconocido\n"; }

    // Init opcode table
    void initOpcodeTable() {
        // Default call for unknown opcodes
        for (int i = 0; i < 256; i++) opcodeTable[i] = op_UNK; 

        // Load the opcodes
        opcodeTable[0b11101010] = op_NOP;  // NOP (No Operation)
    }

    // Execure opcode call
	  if (PRIVATE(cpu)->decode_cycle == 0) {
  	  	fetch_pc_memory(cpu, &cpu->reg_ir, phase);
	  } else {
	    	opcodeTable[cpu->reg_ir](cpu, phase);
	  }
    ```

### How to add a new opcode

- Add the opcode in `cpu_65816_opcodes.h`
- In `cpu_65816.c` add the opcode function to the opcode matrix: `opcodeTable[OP_65816_XCE] = op_XCE`
- Add the decoding function, like `op_XCE`

---

## Status Register

The Processor status register (P) is an 8-bit register that holds information about the current status of the CPU. In a 6502/65c02 the 8 bits are :

- N: Negative Flag. 1 = negative
- V: oVerflow Flag. 1 = overflow
- 1: Always reads back as 1
- B: BRK Command. 1 = BRK / 0 = IRQ
- D: Decimal Mode. 1 = true
- I: IRQB Disable. 1 = disable
- Z: Zero. 1 = True
- C: Carry. 1 = Trye

In a 65c816, when the CPU starts, it does so in emulation mode. Here, (P) reads the same, When in native mode however there are some changes

- N: Negative Flag. 1 = negative
- V: oVerflow Flag. 1 = overflow
- M: Memory Select. 1 = 8bit 0 = 16bit. Controls the size of the Accumulator
- X: Index Register Select. 1 = 8bit 0 = 16bit. Controls the size of X and Y registers, only in emulation mode.
- D: Decimal Mode. 1 = true
- I: IRQB Disable. 1 = disable
- Z: Zero. 1 = True
- C: Carry. 1 = Trye

(E)mulation flag. This flag indicates if the CPU is in emulation (1) or native (0) modes. This flag is hidden and can be exchanged with the Carry bit with the instruction XCE.

In the emulator, the status register is implemented as a 17 bits register

Bits 0-7 represent the status register in emulation mode. Bits 8-15 represent the status register in native mode. Bit 16 is the emulation flag. Bits present in "both" status register, have a mask with both bits active like `FLAG_65816_CARRY = ((uint32_t) 0b00000000100000001)`, while bits present only on one register have only their correponding bit set like `FLAG_65816_MEMORY = ((uint32_t) 0b00010000000000000)`. As flag test are a boolean check against the flag values, they keep working as before. To get the current (P) register value, a macro is defined: `#define CPU_REG_P ((cpu->reg_p >> (8*!(FLAG_IS_SET(cpu->reg_p, FLAG_65816_E))) ) & 0xFF)`. Anywhere in the cpu emulation code where status register is needed, such on JSR opcodes, when status register is pushed into the stak, this macro has to be invoked.

When in Emulation mode (E=1) both M and X are forced to 1. This has some effects on both the accumulator and the index registers, which we will discuss later. If M or X are cleared, and then we go back into emulation Mode, both are then set.

## Accumulator and index registers

When the 65816 boots up, we already know it starts in Emulation mode, forcing flags M and X to 1, therefore making both the accumulator and the index registers 8 bits wide. It is only after switching to Native mode that we can clear M and/or X, making the registers 16 bits wide.

The registers width will directly affect the CPU behaviour. A LDA immediate, when M is set (Accumulator is 8 bits wide) will only read one more byte, and will load it into A. However, if M is clear, the first byte read will be loaded in to A (Accumulator LO byte) and then, a second byte will be fetched into B (Accumulator HI byte).

This means both the instruction length and the cycles it takes to execute changes. Instructions related to X and Y depend on the X flag in the same way.

In the original dromaius, each addressing mode had a fuction that returned `true` the moment the operand was fetched. To implement this varying behaviour in dromaius816, if the instruction has to read a 16 bit operand, the decoding cycle is not reset, and a second fetch is done, by calling the same fetch function.

Switching A from 16 to 8 bits, keeps the value stored in the HI Byte, which can be acceses with the XBA instruction. Switching X and Y from 16 to 8 bits, zeroes out the HI byte on both. 

Behaviour of instructions involving both registers, such as TXA, usually depend on the target register width. For example TXA when x is 16 bits and A is 8 bits, copies only the LO byte from X to A, keeping B intact. More testing is needed on this matter.

## Interrupts

### Software vs hardware interrupts
> I need to talk about BRK here

### Emulation vs Native mode interrupts
> Different vector and emulator implementation

## Docs

http://6502.org/tutorials/65c816opcodes.html
https://www.westerndesigncenter.com/wdc/documentation/w65c816s.pdf
https://the-dreams.de/aay64.txt

--- 

## CHATGPT Docs start here!

## Analyze the Differences and Similarities

- **Compatibility and Operating Modes:**  
  The 65816 is an extension of the 6502 and includes:
  - **Emulation Mode:** Similar to the 6502.
  - **Native Mode:** Uses 16-bit registers (such as the accumulator and, in some cases, the index registers).  
  Additionally, it introduces new registers such as:
  - **Direct Page (DP)**
  - **Data Bank (DB)**
  - **Program Bank (PB)**

- **Address Space:**  
  While the 6502 operates with a 16-bit address bus, the 65816 uses a 24-bit address bus. This affects the implementation of memory access routines and internal address representation.

- **Cycles and Synchronization:**  
  Like the 6502, the 65816 can be emulated using a cycle-based approach (dividing each cycle into phases like `CYCLE_BEGIN`, `CYCLE_MIDDLE`, and `CYCLE_END`). However, the number of cycles and behaviors will vary depending on the 65816 instructions and potential page crossings.

---

## 65c816 inner working

### Operation modes

Emulation vs Native

### Processor registers



## Reset sequence

http://forum.6502.org/download/file.php?id=21142&mode=view


## Bus Timing



## Definition of Data Structures and Pins

- **Internal Structure:**  
  Create a structure similar to `Cpu6502_private` but extended to include all registers of the 65816. For example:
  - **Data Registers:**  
    - Accumulator (A): In native mode, 16-bit; in emulation mode, 8-bit.
    - Index Registers (X and Y)
    - Stack Pointer (SP)
    - Program Counter (PC) with 24-bit addressing (stored in a 32-bit variable).
  - **Additional Registers:**  
    - Direct Page (DP)
    - Data Bank (DB)
    - Program Bank (PB)
  - **Processor Flags:**  
    Some flags may have different behaviors, especially regarding decimal mode or register width.

- **Definition of Pins and Signals:**  
  Define a table similar to `Cpu6502_PinTypes` for the 65816 (e.g., `Cpu65816_PinTypes`), specifying address, data, control (reset, IRQ, NMI, etc.), and clock pins. Depending on the simulator, additional signals may be required to handle the 24-bit bus.

---

## Implementation of Memory Access and Cycle Management Functions

- **Fetch and Store Functions:**  
  Reuse the idea of functions like `fetch_memory()` and `store_memory()`, adapting them for 24-bit addressing and new addressing modes.

- **Cycle Phase Division:**  
  Implement a function similar to `cpu_6502_execute_phase()` (e.g., `cpu_65816_execute_phase()`) that:
  - Reads bus signals.
  - Sets address and data outputs.
  - Updates CPU state and manages interrupts if necessary.

- **Handling Extended Cycles:**  
  Some 65816 instructions require additional cycles (e.g., in native mode operations or page crossings). Ensure that instruction decoding functions account for these extra cycles.

---

## Instruction Decoder Design

- **Separation by Instructions and Addressing Modes:**  
  Follow the same approach as in `cpu_6502.c`:
  - Create functions like `decode_adc()`, `decode_and()`, etc., adapted to 65816 semantics (considering, for example, whether the accumulator is in 8-bit or 16-bit mode or if decimal mode is active).
  - Include legacy instructions (for emulation mode) and 65816-specific instructions (for native mode).

- **Auxiliary Functions for Fetching Operands and Addresses:**  
  Develop versions of functions like `fetch_address()` and `fetch_operand()` that correctly handle the extended address bus and new addressing modes (e.g., banked modes or Direct Page offsets).

- **Flag Updates:**  
  You can reuse macros or functions (such as `CPU_CHANGE_FLAG()`) to update flags, but verify if adjustments are needed for new modes or data sizes.

---

## Interrupt Handling and CPU States

- **Interrupts and Reset:**  
  Adapt the interrupt sequence (IRQ, NMI, and BRK) for the 65816, considering interrupt vectors and possible mode switches (emulation vs. native).

- **CPU States:**  
  Implement a state machine similar to the 6502 (e.g., states like `CS_INIT`, `CS_RUNNING`, `CS_IN_IRQ`, `CS_IN_NMI`) and include additional states if needed to manage mode changes or special conditions.

---

## Integration into the Simulator

- **Interface Functions:**  
  As with the 6502, define creation, destruction, and processing functions, such as:
  - `Cpu65816 *cpu_65816_create(Simulator *sim, Cpu65816Signals signals);`
  - `static void cpu_65816_process(Cpu65816 *cpu);`
  - `static void cpu_65816_destroy(Cpu65816 *cpu);`

- **Signal Groups:**  
  Configure signal groups for the address and data bus using macros and functions similar to `SIGNAL_DEFINE_GROUP()` and `SIGNAL_WRITE()`, adapting them to the 65816's bus width and pin count.

---

## Example Skeleton

(A simplified example of code to start implementing the 65816 emulator.)

---

## Testing and Validation

- **Documentation:**
  Refer to the official 65816 documentation and specialized tutorials.

- **Tests:**
  Create test programs to verify behavior in both emulation and native modes, ensuring proper updates of registers and flags.

- **Test Suite**
  https://github.com/SingleStepTests/ProcessorTests/tree/main/65816

---

