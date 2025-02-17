# Guide for Emulating the 65816 Processor

This document outlines the steps and recommendations for creating a file similar to `cpu_6502.c` but designed to emulate a 65816 processor. It follows the same philosophy of cycle-based emulation, adapting it to the specific features and improvements of the 65816 architecture.

---

## 1. Analyze the Differences and Similarities

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

## 2. 65c816 inner working

### 2.1. Operation modes

Emulation vs Native

### 2.2. Processor registers

#### 2.2.1. (P) Processor status register

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

## 3. Reset sequence

http://forum.6502.org/download/file.php?id=21142&mode=view

## 4. Definition of Data Structures and Pins

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

## 5. Implementation of Memory Access and Cycle Management Functions

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

## 6. Instruction Decoder Design

- **Separation by Instructions and Addressing Modes:**  
  Follow the same approach as in `cpu_6502.c`:
  - Create functions like `decode_adc()`, `decode_and()`, etc., adapted to 65816 semantics (considering, for example, whether the accumulator is in 8-bit or 16-bit mode or if decimal mode is active).
  - Include legacy instructions (for emulation mode) and 65816-specific instructions (for native mode).

- **Auxiliary Functions for Fetching Operands and Addresses:**  
  Develop versions of functions like `fetch_address()` and `fetch_operand()` that correctly handle the extended address bus and new addressing modes (e.g., banked modes or Direct Page offsets).

- **Flag Updates:**  
  You can reuse macros or functions (such as `CPU_CHANGE_FLAG()`) to update flags, but verify if adjustments are needed for new modes or data sizes.

---

## 7. Interrupt Handling and CPU States

- **Interrupts and Reset:**  
  Adapt the interrupt sequence (IRQ, NMI, and BRK) for the 65816, considering interrupt vectors and possible mode switches (emulation vs. native).

- **CPU States:**  
  Implement a state machine similar to the 6502 (e.g., states like `CS_INIT`, `CS_RUNNING`, `CS_IN_IRQ`, `CS_IN_NMI`) and include additional states if needed to manage mode changes or special conditions.

---

## 8. Integration into the Simulator

- **Interface Functions:**  
  As with the 6502, define creation, destruction, and processing functions, such as:
  - `Cpu65816 *cpu_65816_create(Simulator *sim, Cpu65816Signals signals);`
  - `static void cpu_65816_process(Cpu65816 *cpu);`
  - `static void cpu_65816_destroy(Cpu65816 *cpu);`

- **Signal Groups:**  
  Configure signal groups for the address and data bus using macros and functions similar to `SIGNAL_DEFINE_GROUP()` and `SIGNAL_WRITE()`, adapting them to the 65816's bus width and pin count.

---

## 9. Example Skeleton

(A simplified example of code to start implementing the 65816 emulator.)

---

## 10. Testing and Validation

- **Documentation:**
  Refer to the official 65816 documentation and specialized tutorials.

- **Tests:**
  Create test programs to verify behavior in both emulation and native modes, ensuring proper updates of registers and flags.

- **Test Suite**
  https://github.com/SingleStepTests/ProcessorTests/tree/main/65816

---

