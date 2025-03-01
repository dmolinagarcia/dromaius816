// cpu_65816_opcodes.h - Daniel Molina - BSD-3-Clause (see LICENCE)
// based on
// cpu_6502_opcodes.h - Johan Smet - BSD-3-Clause (see LICENSE)
//
// list all support 65816 opcodes

#ifndef DROMAIUS_CPU_65816_TRACE_H
#define DROMAIUS_CPU_65816_TRACE_H

// Includes for CPU Trace
#include "cpu_65816.h"
#include "device.h"
#include "filt_65816_asm.h"
#include "log.h"

const char* get_reg_p_status(uint32_t reg_p) {
    // Buffer para "E=? " (4 caracteres) + 8 caracteres para flags + terminador = 13 bytes (uso 16 para mayor seguridad)
    static char buf[16];
    
    // Extraemos el bit E (bit 16)
    uint8_t e = (reg_p >> 16) & 1;
    
    // Construimos la parte "E=0" o "E=1"
    buf[0] = 'E';
    buf[1] = '=';
    buf[2] = e ? '1' : '0';
	buf[3] = ' ';
    
    // Elegimos la máscara y la parte de bits a evaluar según E:
    // Si E=0, se usan bits 15..8 con la máscara "NVMXDIZC"
    // Si E=1, se usan bits 7..0  con la máscara "NVUBDIZC"
    const char *mapping = e ? "NV.BDIZC" : "NVMXDIZC";
    uint8_t bits = e ? (reg_p & 0xFF) : ((reg_p >> 8) & 0xFF);
    
    // Se escribe la representación de los 8 bits a partir de la posición 3 del buffer.
    // Se recorre de bit 7 (más significativo) a bit 0 (menos significativo)
    char *p = &buf[4];
    for (int i = 7; i >= 0; i--) {
        // El índice en mapping es: para bit7 se toma mapping[0], para bit6 mapping[1], etc.
        char letter = mapping[7 - i];
        // Si el bit está a 1, se muestra la letra; si no, se muestra '.'
        *p++ = (bits & (1 << i)) ? letter : '.';
    }
    *p = '\0';
    
    return buf;
}


void cpu_65816_trace (Device *device) {
    static bool prev_start = false;      
    bool start;

    //>TODO Complete CPU logging.
    uint32_t model_number = (device->get_cpu(device))->model_number(device->get_cpu(device));
    if (model_number == 65816) {
        struct Cpu65816 *cpu = (struct Cpu65816 *) device->get_cpu(device);
        start = cpu->is_at_start_of_instruction(cpu);
        if (start && !prev_start) {
            uint8_t mem_pc_0;
            device->read_memory(device, cpu->reg_pc, 1, &mem_pc_0);
            LOG (1, "%02X:%04X %02X          %s {dd:hhll} %s", 
                                        cpu->reg_pbr, cpu->reg_pc,  		// PC
                                            mem_pc_0, 							// Instruction OPCODE
                                        filt_65816_get_opcode(mem_pc_0),	// Instruction MNEMONIC
                                        get_reg_p_status(cpu->reg_p)        // Processor status register
            );
        }
    }
    prev_start = start;
}


#endif // DROMAIUS_CPU_65816_TRACE_H
