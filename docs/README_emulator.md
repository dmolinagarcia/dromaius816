## 🚀 Resumen del Flujo de Funcionamiento del Emulador

### 🧩 **1. Interfaz Gráfica (UI)**

- El usuario interactúa con la interfaz gráfica (por ejemplo, al pulsar el botón de "paso único"):  
  ```c
  if (ImGui::Button(txt_step_single)) {
      dms_single_step(ui_context->dms_ctx);
  }
  ```

### 🔄 **2. Cambio de Estado en `dms_single_step`**

- Se llama a `dms_single_step()` con el contexto de la simulación (`dms_ctx`).
- Si el simulador está en pausa (`DS_WAIT`), cambia el estado a `DS_SINGLE_STEP`:

  ```c
  void dms_single_step(DmsContext *dms) {
      assert(dms);
      if (dms->config_usr.state == DS_WAIT) {
          change_state(dms, DS_SINGLE_STEP);
      }
  }
  ```

### 🧵 **3. Hilo de Simulación (Context Background Thread)**

- En un hilo independiente, el bucle principal de la simulación llama continuamente a `context_execute()` si el estado no es `DS_WAIT`:

  ```c
  while (dms->running) {
      if (dms->config_usr.state != DS_WAIT) {
          context_execute(dms);
      }
  }
  ```

### ⚙️ **4. Ejecución del Contexto (`context_execute`)**

- Se llama a `dms->device->process(dms->device)` para procesar el dispositivo emulado:

  ```c
  dms->device->process(dms->device);
  ```

- Luego, se verifica el estado para decidir si continuar o pausar:

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

### 📦 **5. Procesamiento del Dispositivo (`device_process`)**

- `process` apunta a `device_process`, que es una función común para todos los dispositivos:

  ```c
  void device_process(Device *device) {
      assert(device);
      simulator_simulate_timestep(device->simulator);
  }
  ```

### ⏱️ **6. Simulación del Timestep (`simulator_simulate_timestep`)**

- Esta función es el **corazón de la simulación**, avanzando un ciclo del simulador:

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

#### 🔑 **Tareas Clave en `simulator_simulate_timestep`**

1. **Avanzar el reloj de la simulación (`current_tick`).**
2. **Gestionar eventos programados (interrupciones, temporizadores).**
3. **Procesar los dispositivos "sucios" (CPU, GPU, etc.).**
4. **Detectar cambios en las señales para el siguiente ciclo.**
5. **Registrar el historial de señales si está habilitado.**

### 🗺️ **7. Ciclo Completo**

- Tras procesar un ciclo:
  - Si el estado es `DS_SINGLE_STEP`, se vuelve a `DS_WAIT`.
  - Si el estado es `DS_RUN`, se sigue ejecutando hasta alcanzar un breakpoint.

---

### 🚀 **Resumen del Flujo**

1. **Interfaz gráfica:** Interacción del usuario.
2. **Cambio de estado:** Pausa → Paso único (`DS_SINGLE_STEP`).
3. **Hilo de simulación:** Llama a `context_execute`.
4. **Procesamiento del dispositivo:** A través de `device_process`.
5. **Simulación de un ciclo:** En `simulator_simulate_timestep`.
6. **Actualización del estado:** Pausa o continuar, según el estado.

---

✅ **Con esto tienes una visión completa del flujo de ejecución del emulador.**
Si necesitas profundizar en alguna parte específica, ¡estaré aquí para ayudarte! 🚀

