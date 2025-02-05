## 🗓️ Diario de Trabajo

---

### ✅ **03 / 02 / 2025**
> **Avance:**
> - [x] Creamos paso a paso una emulación básica.
> - [x] Mostramos la ventana por pantalla y algunos paneles.
> - [x] **Problema:** No consigo aún arrancar la simulación (resuelto más adelante).

---

### ✅ **04 / 02 / 2025**
> **Logros:**
> - [x] He conseguido montar un básico con **reloj**, **reset** y **CPU**.
> - [x] Clonado la **CPU6502** a una **CPU65816**.

---

### ✅ **05 / 02 / 2025**
> **Observaciones:**
> - [x] Los **devices** (un *device* es un emulador) originales están:
>   - [x] `PET`, `PETlite` y `Minimal6502`.
> - [x] No tienen más que el **oscilador** y la **CPU**.
> - [x] Creados:
>   - [x] `dev_nova_64` y `dev_minimal_65816` → serán las bases para construir mi emulador.
> - **Conceptos entendidos:**
>   - [x] `Single step`: el paso más pequeño de la simulación.
>   - [x] `Step clock`: medio ciclo de reloj.
> - [x] Comprendido a grandes rasgos el funcionamiento del emulador.
>   - [x] ¡Gracias ChatGPT! Documentado.

---

## ✅ **Tareas Pendientes**

> - [ ] **Oscilador y Reset:**
>   - [ ] Entender su funcionamiento. Schedule es la clave.
>   - [ ] Crear un panel para el oscilador.
>   - [ ] Crear un panel para el reset
> 
> - [ ] Añadir **Logic Analyzer**
>   - [ ] Solo lo tiene `PET`. Añadirlo y comprobar que funciona.
>   - [ ] Entender su funcionamiento y documentarlo.
>   - [ ] Añadirlo al resto de devices y documentar el proceso.
> 
> - [ ] **Schematic:**
>   - [ ] Crear un panel para `schematic` (*dependiente del dispositivo*).
>     - [ ] Como dibujar un `schematic` fijo
>     - [ ] Como colorear un `schematic` fijo
>     - [ ] Puedo autogenerar el `schematic` a partir del device?
> 
> - [ ] **Visualización:**
>   - [ ] Pintar toda la información posible:
>     - [ ] Ver la CPU.
>     - [ ] Pintar `reset` y `clock`.
>     - [ ] ¿Es posible ver el circuito completo? Evaluar esta opción.
> 
> - [ ] **Simulaciones:**
>   - [ ] Simular chips `74xx`.
> 
> - [ ] **Nuevos Chips:**
>   - [ ] Crear un chip nuevo llamado **HELIUM** (*FPGA*):
>     - [ ] Debe tener como input el **master clock** y como output un **PHI** dividido por `n` (¿4? ¿16?).
>   - [ ] Power button
>   - [ ] PSRam
>   - [ ] SD Card
> 
> - [ ] **CPU:**
>   - [ ] Entender el funcionamiento interno de la CPU.
>   - [ ] Enganchar una memoria al `minimal65816`.
>   - [ ] Conseguir ejecutar algo básico (aunque siga siendo un `6502` a pesar del nombre).
>     - [ ] Test de `NOP`.
>     - [ ] Instrucciones básicas como `LDA`.
> 
> - [ ] **Desarrollo CPU65816:**
>   - [ ] Destripar la `CPU65816`.
>   - [ ] Comenzar a añadir funcionalidades poco a poco.
 
---

> ### 🚀 **Siguientes Pasos:**
> - Priorizar la visualización y análisis del reloj y el oscilador.
> - Consolidar la documentación para facilitar futuras iteraciones.
> - Foco en pruebas básicas de la `CPU65816` antes de extender funcionalidades.
