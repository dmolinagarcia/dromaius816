## Diario
### 03 / 02 / 2025

>> Creamos paso a paso una emuacion básica. Mostrando la ventana por pantalla y algunos paneles. No consigo aun arrancar simulacion.

### 04 / 02 / 2025

>> He conseguido montar un básico con reloj, reset y cpu. Tambíen he clonado la CPU6502 a una CPU65816. 

### 05 / 02 / 2025

>> Los devices (un devices es un emulador) originales están. PET, PETlite y Minimal6502
>> No tienen nada más que el OSCilador y la CPU
>> Creados un dev_nova_64 y un dev_minimal_65816. Seran las bases donde construiré mi emulador
>> Single step es el paso más pequeño de la simulacion. Step clock es half clock
>> Comprendido a grandes rasgos el funcionamiento del emulador. Gracias ChatGPT. Documentado

### Pendientes

Añadir logic analyzer. Solo lo tiene PET. Añadirlo al PET. Y comprobar que funciona. Entenderlo y documentarlo.
Despues, añadirlo al resto. Documentar el proceso.
Entender el reloj, oscilador.
Crear un panel para el oscilador
Crear un panel para schematic (device dependant)
Siguientes pasos son. Como pintar toda la información posible. Veo la CPU pero... quiero ver circuito? Es posible?
Al menos, pintar reset, clock, por ahora.
Crear un chip nuevo. HELIUM. FPGA. De momento... quiero que tenga como input el master clock y output un PHI partido por n (4? 16?)
Simular chips 74xx
Además, quiero entender la CPU.
También debo enganchar una memoria. Al minimal65816.
Conseguir ejecutar algo básico (teniendo en cuenta que sigo siendo un 6502 a pesar del nombre)
NOP test
LDAs, cosas muy basicas vaya
Tras esto, destripar la cpu65816 y comenzar a añadir cosas de poco a poco.
