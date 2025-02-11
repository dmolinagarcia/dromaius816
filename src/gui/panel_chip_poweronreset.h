// gui/panel_chip_poweronreset.h - Daniel Molina - BSD-3-Clause (see LICENSE)
//
// UI panel to display information for poweronreset circuit

#ifndef DROMAIUS_GUI_PANEL_CHIP_POWERONRESET_H
#define DROMAIUS_GUI_PANEL_CHIP_POWERONRESET_H

#include "panel.h"

// functions
Panel::uptr_t panel_chip_poweronreset_create(class UIContext *ctx, struct ImVec2 pos, struct PowerOnReset *por, std::string id);

#endif // DROMAIUS_GUI_PANEL_CHIP_POWERONRESET_H
