// device.c - Johan Smet - BSD-3-Clause (see LICENSE)
//
// Common 'base' for devices

#include "device.h"
#include <assert.h>

#include "cpu_65816.h"
#include "log.h"

void device_process(Device *device) {
	assert(device);

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
			LOG (1, "%02X:%04X %02X", cpu->reg_pbr, cpu->reg_pc,  
				      			  		mem_pc_0
			);
		}
	}
	prev_start = start;
	
	simulator_simulate_timestep(device->simulator);
}
