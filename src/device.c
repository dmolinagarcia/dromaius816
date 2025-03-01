// device.c - Johan Smet - BSD-3-Clause (see LICENSE)
//
// Common 'base' for devices

#include "device.h"
#include "cpu.h"
#include "cpu_65816_trace.h"
#include <assert.h>

void device_process(Device *device) {
	assert(device);
	if ((device->get_cpu(device))->model_number(device->get_cpu(device)) == 65816) cpu_65816_trace(device);
	simulator_simulate_timestep(device->simulator);
}


