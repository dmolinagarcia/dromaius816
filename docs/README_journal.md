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
> Added `panel_id` attribute to panels, to avoid opening duplicated panels. Added SliderFloat to control target simulation speed. SliderScalar allows to customize inner text, but I haven't managed to make it work. Added to tasks.
> Also, total simulation time has been added to Control panel. As a bonus, I've learnt that the speed modifier does not affect the base speed of the clock, it is just a speed ratio, or simulation ratio. 
> Also, I've found out why the Full Pet2001 runs 4x faster than everything else. PHI2 is not connected, so the CPU is not running. Actually, nothing is running in this device. Mystery solved
> On next day, SliderFloat has been changed to a SliderScalar with custom label. Done! 


---

### ✅ **11 / FEB / 2025**
> **Logic Analyzer**
> Added to PET and it is working. I had an issue with `assert(signal < history->signal_count);` as signal went from 199 to 256. I don't know why. This assert is there to prevent a buffer overflow. I added a condition so the function where the assert is is not called if the index is exceeded. 
> To use it, each device must have, and call, its create_history function in signal_history. 
>
> I have added logic analyzer to Minimal 65816, seems to work. Biggest next achievement is to add buses as a single line in the analyzer.
>

---

### ✅ **14 / FEB / 2025**
> **Logic Analyzer**
> This took a while!. Buses are added to the logic analyzer. If all signals of a bus (DB0:7 or AB0:15) are added into the LA, they get compacted into a single bus. On each transition, the value of the bus in hexadecimal is plotted. Right now, busses are hardcoded. This could be enhanced in the future. 
>
> Zooming has been fixed, so smaller adjusts are possible. A first try at scrolling has failed. Will dig further.

---

### ✅ **15 / FEB / 2025**
> **Logic Analyzer**
> Horizontal movement has been implemented as click and drag. Zooming needs to be perfected, but is good enough as is. Time to move on. Next week, we'll work on the schematic

---

### ✅ **W08 / 2025**
> Moved to a Wnn / YYYY format for the journal. Bigger tasks now, so slower progress. (Week Number and Year)
> **CPU**
> cpu_65816 has been striped out of all functions. Just the minimun to run the simulation are still there. PINs have been renamed, and an empty process is implemented. Now it's time to investigate the inner workings of the CPU.
>
> Startup sequence seems correct, and, as all decode logic has been removed, any opcode is treatad as a NOP. A NOP free loop is running, apparently OK. No addressing modes, no decoding, no nothing. 

---

### ✅ **W09 / 2025**
> **Memory**
> A basic structure for a 65816 disassembler is implemented. It will need E, M and X values. 
>
> **CPU**
> Basic fetch and decode is in place. Only NOP is decoded, but the decoding routine is set up. CPU pins have been modified to reflect 65816. P Register is fully implemented. Memory is connected to all devices and running fine. Bank address output is implemented, although it is a hardcoded value for now.
>
> `Execute instruction` depends on the CPU signaling the start of instruction. The 6502 uses its `SYNC` output for this. The 65816 uses `VPA` and `VDA`. `VPA` has been modified to mimic 6502's `SYNC` output and the single stepping now works. `VPA` and `VDA` need more work though.
>
> `XCE` is implemented. 2 out of 256 opcodes done.
>
> Device logger is implemented in device_process. We get the and print relevant registers. We can access device memory from here and call the dissasembler to get the OPCODEs and instruction length. Partially implemented. The dissasembler has been extended to handle 65816 opcodes, but more work is needed. 


### ✅ **W09-W10 / 2025**
> **Memory**
> Disassembler is completed. Current M and X are used to display 8 or 16 bit operands. It can be done better, but it is good enough for now. Disassembler is now used for the AJtracer improving its output
>
> **CPU**
> Added immediate addresing, and with it some memory fetches and decoding. `fetch_memory` fetches pc into operand. LDA immediate (0xA9) has bene implemented and tested. It responds properly to Accumulator size changes. Now for X and Y, some tweaks to `reg_x` and `reg_y` on `FLAG_X` changes may be needed.

---

## ✅ **Pending Tasks**

> - [ ] **Development:**
>   - [ ] Configure a build task within vscode
>   - [ ] Configure a debug setup within vscode
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
>  - [ ] Tracer needs a few more fields. Stack and operand address.
>  - [ ] Disassembler is not calculating relative jumps. Fix this. 
>
>  - [ ] **65816 emulated computer**
>    - [ ] Implement bigger RAM. (16MB)
> 
> - [ ] **Simulations:**
>   - [ ] Simulate `74xx` chips.
> 
> - [ ] **Simulator:**
>   - [ ] Invetigate tick speed effects on simulation speed.
>
> - [ ] **New Chips:**
>   - [ ] Create a new chip called **HELIUM** (*FPGA*):
>     - [ ] It should take the **master clock** as input and output a **PHI** divided by `n` (4? 16?).
>   - [ ] Power button.
>   - [ ] PSRam.
>   - [ ] SD Card.
> 
> - [ ] Add **Logic Analyzer**
>   - [x] Only `PET` currently has it. (Both lite and full). Add it and verify that it works.
>   - [x] Understand how does it work and document it.
>   - [x] Add it to the other devices and document the process. Make it as universal as possible
>   - [x] Add buses.
>   - [x] Add scrolling.
>     -  [ ] Zooming while scrolled. Keep point under mouse pointer static
>   - [ ] Custom buses. 