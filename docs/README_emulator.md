## 🚀 Emulator Execution Flow Summary

### 🧙️ **1. Graphical Interface (UI)**

- The user interacts with the graphical interface (e.g., pressing the "single step" button):  
  ```c
  if (ImGui::Button(txt_step_single)) {
      dms_single_step(ui_context->dms_ctx);
  }
  ```

### 🔄 **2. State Change in `dms_single_step`**

- `dms_single_step()` is called with the simulation context (`dms_ctx`).
- If the simulator is paused (`DS_WAIT`), it changes the state to `DS_SINGLE_STEP`:

  ```c
  void dms_single_step(DmsContext *dms) {
      assert(dms);
      if (dms->config_usr.state == DS_WAIT) {
          change_state(dms, DS_SINGLE_STEP);
      }
  }
  ```

### 🧕 **3. Simulation Thread (Context Background Thread)**

- In a separate thread, the main simulation loop continuously calls `context_execute()` if the state is not `DS_WAIT`:

  ```c
  while (dms->running) {
      if (dms->config_usr.state != DS_WAIT) {
          context_execute(dms);
      }
  }
  ```

### ⚙️ **4. Context Execution (`context_execute`)**

- Calls `dms->device->process(dms->device)` to process the emulated device:

  ```c
  dms->device->process(dms->device);
  ```

- Then checks the state to decide whether to continue or pause:

  ```c
  switch (dms->config.state) {
      case DS_SINGLE_STEP:
          context_change_state(dms, DS_WAIT);
          break;
      case DS_RUN:
          if (context_check_breakpoints(dms, cpu)) {
              context_change_state(dms, DS_WAIT);
          }
          break;
  }
  ```

### 📦 **5. Device Processing (`device_process`)**

- `process` points to `device_process`, a common function for all devices:

  ```c
  void device_process(Device *device) {
      assert(device);
      simulator_simulate_timestep(device->simulator);
  }
  ```

### ⏱️ **6. Simulation Timestep (`simulator_simulate_timestep`)**

- This function is the **core of the simulation**, advancing one simulation cycle:

  ```c
  void simulator_simulate_timestep(Simulator *sim) {
      assert(sim);
      if (PRIVATE(sim)->dirty_chips > 0) {
          ++sim->current_tick;
      } else {
          sim->current_tick = simulator_next_scheduled_event_timestamp(PRIVATE(sim));
      }
      sim_handle_event_schedule(PRIVATE(sim));
      sim_process_sequential(PRIVATE(sim), PRIVATE(sim)->dirty_chips);
      PRIVATE(sim)->dirty_chips = signal_pool_cycle(sim->signal_pool);
  }
  ```

#### 🔑 **Key Tasks in `simulator_simulate_timestep`**

1. **Advance the simulation clock (`current_tick`).**
2. **Handle scheduled events (interrupts, timers).**
3. **Process "dirty" devices (CPU, GPU, etc.).**
4. **Detect signal changes for the next cycle.**
5. **Log signal history if enabled.**

### 🗙️ **7. Complete Cycle**

- After processing a cycle:
  - If the state is `DS_SINGLE_STEP`, it returns to `DS_WAIT`.
  - If the state is `DS_RUN`, execution continues until a breakpoint is reached.

---

### 🚀 **Flow Summary**

1. **Graphical interface:** User interaction.
2. **State change:** Pause → Single Step (`DS_SINGLE_STEP`).
3. **Simulation thread:** Calls `context_execute`.
4. **Device processing:** Through `device_process`.
5. **Simulation cycle:** In `simulator_simulate_timestep`.
6. **State update:** Pause or continue, depending on the state.

---

**Created with the help of ChatGPT-4o**