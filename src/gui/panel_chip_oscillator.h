// gui/panel_chip_oscillator.h - Daniel Molina - BSD-3-Clause (see LICENSE)
//
// UI panel to display information about a oscillator

#ifndef DROMAIUS_GUI_PANEL_CHIP_OSCILLATOR_H
#define DROMAIUS_GUI_PANEL_CHIP_OSCILLATOR_H

#include "panel.h"

// functions
Panel::uptr_t panel_chip_oscillator_create(class UIContext *ctx, struct ImVec2 pos, struct Oscillator *tmr, std::string id);

#endif // DROMAIUS_GUI_PANEL_CHIP_OSCILLATOR_H
