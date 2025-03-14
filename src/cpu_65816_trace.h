// cpu_65816_opcodes.h - Daniel Molina - BSD-3-Clause (see LICENCE)
// based on
// cpu_6502_opcodes.h - Johan Smet - BSD-3-Clause (see LICENSE)
//
// list all support 65816 opcodes

//> TODO_DMG translation needed!

#ifndef DROMAIUS_CPU_65816_TRACE_H
#define DROMAIUS_CPU_65816_TRACE_H

// Includes for CPU Trace
#include "cpu_65816.h"
#include "device.h"
#include "types.h"
#include "filt_65816_asm.h"
#include "log.h"

const char* get_reg_p_status(uint32_t reg_p) {
    static char buf[16];
    
    // Extraemos el bit E (bit 16)
    uint8_t e = (reg_p >> 16) & 1;
    
    // Construimos la parte "E=0" o "E=1"
    buf[0] = 'E';
    buf[1] = '=';
    buf[2] = e ? '1' : '0';
	buf[3] = ' ';
    buf[4] = 'P';
    buf[5] = '=';
    
    // Elegimos la máscara y la parte de bits a evaluar según E:
    // Si E=0, se usan bits 15..8 con la máscara "NVMXDIZC"
    // Si E=1, se usan bits 7..0  con la máscara "NVUBDIZC"
    const char *mapping = e ? "NV.BDIZC" : "NVMXDIZC";
    uint8_t bits = e ? (reg_p & 0xFF) : ((reg_p >> 8) & 0xFF);
    
    // Se escribe la representación de los 8 bits a partir de la posición 3 del buffer.
    // Se recorre de bit 7 (más significativo) a bit 0 (menos significativo)
    char *p = &buf[6];
    for (int i = 7; i >= 0; i--) {
        // El índice en mapping es: para bit7 se toma mapping[0], para bit6 mapping[1], etc.
        char letter = mapping[7 - i];
        // Si el bit está a 1, se muestra la letra; si no, se muestra '.'
        *p++ = (bits & (1 << i)) ? letter : '.';
    }
    *p = '\0';
    
    return buf;
}

const char* format_reg_value(uint16_t value, bool last_two_in_brackets, char *buf) {
    if (last_two_in_brackets) {
        sprintf(buf, "%02X[%02X]", value >> 8, value & 0xFF);
    } else {
        sprintf(buf, "[%04X]", value);
    }
    return buf;
}

void cpu_65816_trace (Device *device) {
    static bool prev_start = false;      
    bool start;

    uint32_t model_number = (device->get_cpu(device))->model_number(device->get_cpu(device));
    if (model_number == 65816) {
        struct Cpu65816 *cpu = (struct Cpu65816 *) device->get_cpu(device);
        start = cpu->is_at_start_of_instruction(cpu);
        if (start && !prev_start) {
            // Definimos el tamaño máximo de la instrucción a leer
            #define MAX_INSTR_BYTES 8
            uint8_t buffer[MAX_INSTR_BYTES];
            // Leemos MAX_INSTR_BYTES a partir de la dirección actual del PC
            device->read_memory(device, cpu->reg_pc, MAX_INSTR_BYTES, buffer);

            // Preparamos un buffer para almacenar la línea desensamblada.
            // Se asume un tamaño máximo de 256 caracteres (ajustable si es necesario).
            // Usamos un puntero auxiliar para pasar a la función (podrías usar también asm_line directamente si tu función lo permite)
            char *asm_line = NULL;

            // Las banderas m y x se extraen del registro de estado
            bool m_flag = (cpu->reg_p & FLAG_65816_M) != 0;
            bool x_flag = (cpu->reg_p & FLAG_65816_X) != 0;

            // Llamamos a la función de desensamblado.
            // Se pasa:
            // - buffer: los bytes leídos de memoria.
            // - MAX_INSTR_BYTES: cantidad total de bytes disponibles.
            // - 0: índice dentro del buffer (se empieza en el primer byte).
            // - cpu->reg_pc: se usa como offset para imprimir la dirección absoluta.
            // - &line_ptr: dirección del puntero donde se escribirá la línea.
            // - m_flag y x_flag: para ajustar el tamaño de operando en modo inmediato.
            size_t bytes_consumed = filt_65816_asm_line(buffer, MAX_INSTR_BYTES, 0, cpu->reg_pc, &asm_line, m_flag, x_flag);

            (void) bytes_consumed; // En caso de que quieras usar el valor, por ahora se ignora.

            // Read the stack
            #define STACK_BYTES 9
            uint8_t stack[STACK_BYTES];
            // Leemos MAX_INSTR_BYTES a partir de la dirección actual del PC
            device->read_memory(device, cpu->reg_sp+1, STACK_BYTES, stack);


            // Formateamos los valores de los registros para la traza
            char reg_a[10], reg_x[10], reg_y[10], reg_sp[10];
            format_reg_value(cpu->reg_a , cpu->reg_p & FLAG_65816_M, reg_a );
            format_reg_value(cpu->reg_x , cpu->reg_p & FLAG_65816_X, reg_x );
            format_reg_value(cpu->reg_y , cpu->reg_p & FLAG_65816_X, reg_y );
            format_reg_value(cpu->reg_sp, cpu->reg_p & FLAG_65816_E, reg_sp);

            // Imprimimos la traza incluyendo la línea desensamblada completa
            LOG(1, "%-35s    %s A=%s X=%s Y=%s DP=%04X SP=%s DBR=%02x s>%02x %02x %02x %02x %02x %02x %02x %02x| CYCLE=%d", 
                asm_line,
                get_reg_p_status(cpu->reg_p),
                reg_a,
                reg_x,
                reg_y,
                cpu->reg_dp,
                reg_sp,
                cpu->reg_dbr,
                stack[0], stack[1], stack[2], stack[3], stack[4], stack[5], stack[6], stack[7], 
                cpu->get_cycles(cpu)
            );
        }
    }
    prev_start = start;
}


#endif // DROMAIUS_CPU_65816_TRACE_H
