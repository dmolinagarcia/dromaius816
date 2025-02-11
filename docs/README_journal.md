## 🗓️ Work Diary

---

### ✅ **03 / FEB / 2025**
> **Progress:**
> - [x] Step by step, we created a basic emulation.
> - [x] We displayed the window on screen and some panels.
> - [x] **Issue:** I still can’t start the simulation (resolved later).

---

### ✅ **04 / FEB / 2025**
> **Achievements:**
> - [x] I managed to set up a basic system with **clock**, **reset**, and **CPU**.
> - [x] Cloned the **CPU6502** to a **CPU65816**.

---

### ✅ **05 / FEB / 2025**
> **Observations:**
> - [x] The original **devices** (a *device* is an emulator) are:
>   - [x] `PET`, `PETlite`, and `Minimal6502`.
> - [x] They only include the **oscillator** and the **CPU**.
> - [x] Created:
>   - [x] `dev_nova_64` and `dev_minimal_65816` → these will serve as the foundations to build my emulator.
> - **Concepts Understood:**
>   - [x] `Single step`: the smallest step in the simulation.
>   - [x] `Step clock`: half a clock cycle.
> - [x] Gained a broad understanding of how the emulator works.
>   - [x] Thanks ChatGPT! Documented.

---

### ✅ **07 / FEB / 2025**
> **Understanding chips and the simulator**
> Each of the chips is defined within the Chip Struct. One of its properties is `schedule_timestamp` that represents the next time the chip has to perform an action. This is used to implement delays (e.g. a signal change after a clock edge, or e clock change after half a cycle).
> The Oscillator uses this to schedule a clock flip afetr half a cycle. PowerOnReset schedules e resert DEASSERT after its predefined delay
> On each step of the simulator, signal changed are calculated. Any chips affected by changed signals is marked as dirty. On process, all dirty chips and all scheduled actions in the past are executed.

---

### ✅ **08 / FEB / 2025**
> **Panel for the oscillator**
> I've managed to create a new panel for the oscillator. A new panel for the reset should be easy now.

---

### ✅ **09 / FEB / 2025**
> **Panel for the power on reset**
> I've managed to create a new panel for the poweronreset. The soft reset button on the control panel signals the reset function within the device. In the minimal 65816 this toggles the in_reset attribute for the device. Then, the glue logic signals de TRIGGER_B pin in the power on reset circuit. Righit now this is not working as the glue logic is not implemented. I am creating a new task to investigate how the glue logic works.

---

### ✅ **10 / FEB / 2025**
> **Dev Glue Logic**
> GLUE Logic for a device is a chip that handles internal control signals. It's like a PLA. PINs are defined within the device, along it's behaviour (process). Soft Reset calls a device_reset function. In the Minimal device, this toggles a variable (in_reset) that is connectd to the RESET_BTN_B signal. This triggers the device to reset. Signals are assigned in order to the GLUELOGIC chip, so any defined signal must be connected.

> **Signals**
> In the dev create function. Signals are created and defined. Calls to `SIGNAL_GROUP_NEW_N`, `SIGNAL_DEFINE_DEFAULT`, `SIGNAL_DEFINE` etc, create and assign the functions. Signal values can be accesed via device->simualtor->signal_pool. In here, signals_value is a 64bit integer that maps to each of the signals. So... is 64 the maximum number of signals I can use? The PET seems to have 100+. Maybe blocks and layers play a role I do not understand yet. No reference to layer or blocks exists on the device. Is it transparent?

---

### ✅ **11 / FEB / 2025**
> **GUI**
> Added `panel_id` attribute to panels, to avoid opening duplicated panels.

## ✅ **Pending Tasks**

> - [ ] **GUI:**
>   - [ ] Add modifiers to panels. Oscillator and reset are good choices
>   - [ ] Add total simulation time display to control panel
> 
> - [ ] Add **Logic Analyzer**
>   - [ ] Only `PET` currently has it. Add it and verify that it works.
>   - [ ] Understand how does it work and document it.
>   - [ ] Add it to the other devices and document the process. Make it as universal as possible
> 
> - [ ] **Schematic:**
>   - [ ] Create a panel for the `schematic` (*dependent on the device*).
>     - [ ] How to draw a fixed `schematic`
>     - [ ] How to color a fixed `schematic`
>     - [ ] Can I auto-generate the `schematic` from the device?
>   - [ ] Emscripten. How does it work?
> 
> - [ ] **Visualization:**
>   - [ ] Display all available information:
>     - [ ] View the CPU.
>     - [ ] Display `reset` and `clock`.
>     - [ ] Is it possible to see the complete circuit? Evaluate this option.
> 
> - [ ] **Simulations:**
>   - [ ] Simulate `74xx` chips.
> 
> - [ ] **New Chips:**
>   - [ ] Create a new chip called **HELIUM** (*FPGA*):
>     - [ ] It should take the **master clock** as input and output a **PHI** divided by `n` (4? 16?).
>   - [ ] Power button.
>   - [ ] PSRam.
>   - [ ] SD Card.
> 
> - [ ] **CPU:**
>   - [ ] Understand the internal workings of the CPU.
>     - [ ] Differences between the 65c02 and 65c816 Processor Status Register
>   - [ ] Connect memory to the `minimal65816`.
>   - [ ] Get something basic running (even though it’s still a `6502` despite the name).
>     - [ ] `NOP` test.
>     - [ ] Basic instructions such as `LDA`.
> 
> - [ ] **CPU65816 Development:**
>   - [ ] Dissect the `CPU65816` completely. Remove any functionality and everything from its pannel
>   - [ ] Begin adding functionalities gradually.
>     - [ ] Create PINOUT for the CPU
>     - [ ] Understand status register and how it changed between Emulation and Native
>       - [ ] Implement Status Register
>     - [ ] Understand remaining processor registeers
>       - [ ] Implement remaining registers.
>     - [ ] Timing and bus behavior. I need to implement the BankAddress.
>     - [ ] Initially, only advance the PC each cycle.
>     - [ ] Understand procesor cycle
>     - [ ] Implement OPCODES
>       - [ ] Add NOP for a NOP free loop.
