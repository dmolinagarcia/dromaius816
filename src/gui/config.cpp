// gui/config.cpp - Daniel Molina - BSD-3-Clause (see LICENSE)
//
// extends
//
// gui/config.cpp - Johan Smet - BSD-3-Clause (see LICENSE)

#include "config.h"

bool Config::set_machine_type(const std::string_view &text) {

	if (text.compare("commodore-pet") == 0) {
		machine_type = MachineType::CommodorePet;
	} else if (text.compare("commodore-pet-lite") == 0) {
		machine_type = MachineType::CommodorePetLite;
	} else if (text.compare("minimal-6502") == 0) {
		machine_type = MachineType::Minimal6502;
	} else if (text.compare("minimal-65816") == 0) {
		machine_type = MachineType::Minimal65816;
	} else if (text.compare("nova64") == 0) {
		machine_type = MachineType::Nova64;
	} else {
		return false;
	}

	return true;
}