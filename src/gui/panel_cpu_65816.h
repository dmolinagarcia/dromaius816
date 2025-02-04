// gui/panel_cpu_65816.h - Daniel Molina - BSD-3-Clause (see LICENSE)
// based on
// gui/panel_cpu_6502.h - Johan Smet - BSD-3-Clause (see LICENSE)
//
// UI panel to display information about a MOS 65816

#ifndef DROMAIUS_GUI_PANEL_CPU_65816_H
#define DROMAIUS_GUI_PANEL_CPU_65816_H

#include "panel.h"

// functions
Panel::uptr_t panel_cpu_65816_create(class UIContext *ctx, struct ImVec2 pos, struct Cpu65816 *cpu);

#endif // DROMAIUS_GUI_PANEL_CPU_65816_H
