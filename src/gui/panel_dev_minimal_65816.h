// gui/panel_dev_minimal_65816.h - Daniel Molina - BSD-3-Clause (see LICENSE)
// based on
// gui/panel_dev_minimal_6502.h - Johan Smet - BSD-3-Clause (see LICENSE)

#ifndef DROMAIUS_GUI_PANEL_DEV_MINIMAL_65816_H
#define DROMAIUS_GUI_PANEL_DEV_MINIMAL_65816_H

#include "panel.h"

Panel::uptr_t panel_dev_minimal_65816_create(class UIContext *ctx, struct ImVec2 pos, struct DevMinimal65816 *device);

#endif // DROMAIUS_GUI_PANEL_DEV_MINIMAL_65816_H
