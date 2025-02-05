// gui/panel_dev_nova_64.h - Daniel Molina - BSD-3-Clause (see LICENSE)
// extending
// gui/panel_dev_minimal_65816.h - Daniel Molina - BSD-3-Clause (see LICENSE)
// based on
// gui/panel_dev_minimal_6502.h - Johan Smet - BSD-3-Clause (see LICENSE)

#ifndef DROMAIUS_GUI_PANEL_DEV_NOVA_64_H
#define DROMAIUS_GUI_PANEL_DEV_NOVA_64_H


#include "panel.h"

Panel::uptr_t panel_dev_nova_64_create(class UIContext *ctx, struct ImVec2 pos, struct DevNova64 *device);

#endif // #ifndef DROMAIUS_GUI_PANEL_DEV_NOVA_64_H

