# Guide for Emulating the 65816 Processor

This document outlines the steps and recommendations for creating a file similar to `cpu_6502.c` but designed to emulate a 65816 processor. It follows the same philosophy of cycle-based emulation, adapting it to the specific features and improvements of the 65816 architecture. A task list is also provided to managed the upgrade from the 6502 to the 65816.

---

## Task List

  - [x] Dissect the `CPU65816` completely. Remove any functionality and everything from its panel.
  - [ ] Study the 65816 and identify differences with the 6502
  - [x] Create PINOUT for the CPU
  - [x] Understand procesor cycle
    - [ ] Timing and bus behavior. I need to implement the BankAddress.
    - [ ] Should I change process phases?. I may need new steps
  - [ ] CPU Registers
    - [ ] Understand status register and how it changed between Emulation and Native
      - [ ] Implement Status Register
    - [ ] Understand remaining processor registers
      - [ ] When and how registers change size?
      - [ ] How does changing size affect the values stored¿?
      - [ ] Implement remaining registers.
    - [ ] Implement OPCODES Phase 1
      - [ ] Understand processor phases
      - [ ] Initially, everything is a NOP. Advance PC and continue
      - [ ] Add a catch all that triggers some warning (Illegal Opcode)
      - [ ] Add NOP. Run a free NOP
    - [ ] Understand addresing modes
    - [ ] Implement OPCODES Phase 2
      - [ ] XCE. Switch emulation/native
      - [ ] Implement decoding logic. How to?
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

    // Function table for each opcode
    OpcodeHandler opcodeTable[256];  

    // Function to process each opcode
    void op_ADC() { std::cout << "Ejecutando ADC\n"; }
    void op_LDA() { std::cout << "Ejecutando LDA\n"; }
    void op_STA() { std::cout << "Ejecutando STA\n"; }
    void op_JMP() { std::cout << "Ejecutando JMP\n"; }
    void op_BEQ() { std::cout << "Ejecutando BEQ\n"; }
    void op_BNE() { std::cout << "Ejecutando BNE\n"; }
    void op_NOP() { std::cout << "Ejecutando NOP\n"; }
    void op_UNK() { std::cout << "Opcode desconocido\n"; }

    // Init opcode table
    void initOpcodeTable() {
        for (int i = 0; i < 256; i++) opcodeTable[i] = op_UNK; 
        // Default call for unknown opcodes

        // Load the opcodes
        opcodeTable[0b01101001] = op_ADC;  // ADC #imm
        opcodeTable[0b10101001] = op_LDA;  // LDA #imm
        opcodeTable[0b10001001] = op_STA;  // STA $addr
        opcodeTable[0b01001100] = op_JMP;  // JMP $addr
        opcodeTable[0b11110000] = op_BEQ;  // BEQ (Branch if Equal)
        opcodeTable[0b11010000] = op_BNE;  // BNE (Branch if Not Equal)
        opcodeTable[0b11101010] = op_NOP;  // NOP (No Operation)
    }

    // Execure opcode call
    void executeOpcode(uint8_t opcode) {
        opcodeTable[opcode]();  
        // Llama a la función correspondiente en O(1)
    }

    // Test  
    int main() {
        initOpcodeTable();  
        return 0;
    }
    ```

---

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

#### (P) Processor status register

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
---

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

