// filter_65816_asm.h - Johan Smet - BSD-3-Clause (see LICENSE)
// based on
// filter_6502_asm.h - Johan Smet - BSD-3-Clause (see LICENSE)
//
// Dissambler for 65816 assembly

#ifndef DROMAIUS_FILTER_65816_ASM_H
#define DROMAIUS_FILTER_65816_ASM_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

size_t  filt_65816_asm_line(const uint8_t *binary, size_t bin_size, size_t bin_index, size_t bin_offset, char **line);
size_t  filt_65816_asm_count_instruction(const uint8_t *binary, size_t bin_size, size_t from, size_t until);
uint8_t filt_65816_asm_instruction_length(uint8_t opcode, uint32_t reg_p);

uint8_t filt_65816_asm_instruction_length(uint8_t opcode, uint32_t reg_p);
const char* filt_65816_get_opcode(uint8_t opcode);


#ifdef __cplusplus
}
#endif

#endif // DROMAIUS_FILTER_65816_ASM_H
