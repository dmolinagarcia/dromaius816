// gui/panel_chip_poweronreset.c - Daniel Molina - BSD-3-Clause (see LICENSE)
//
// UI panel to display information about poweronreset

#include "panel_chip_poweronreset.h"
#include "chip_poweronreset.h"
#include "context.h"
#include "utils.h"
#include "widgets.h"
#include "ui_context.h"

#include <imgui.h>
#include <stb/stb_ds.h>
#include <assert.h>

#define SIGNAL_PREFIX		CHIP_POR_
#define SIGNAL_OWNER		por

class PanelChipPowerOnReset : public Panel {
public:
	PanelChipPowerOnReset(UIContext *ctx, ImVec2 pos, PowerOnReset *por, std::string id) :
		Panel(ctx),
		position(pos),
		por(por) {
		panel_title = ui_context->unique_panel_id("PowerOnReset");
		panel_id = "PowerOnReset " + id;
	}

	void display() override {

		ImGui::SetNextWindowPos(position, ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);

		if (ImGui::Begin(panel_title.c_str(), &stay_open)) {

			if (ImGui::BeginTable("signals", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_SizingFixedFit)) {
					ImGui::TableSetupColumn("Signal");
					ImGui::TableSetupColumn("Value");
					ImGui::TableHeadersRow();

					ImGui::TableNextColumn();	ImGui::Text("CHIP_POR_TRIGGER_B");
					ImGui::TableNextColumn();	ui_signal_short(SIGNAL_VALUE(TRIGGER_B));
					ImGui::TableNextRow();

					ImGui::TableNextColumn();	ImGui::Text("CHIP_POR_RESET_B");
					ImGui::TableNextColumn();	ui_signal_short(SIGNAL_VALUE(RESET_B));
					ImGui::EndTable();
				}			

		}

		ImGui::End();
	}

private:
	const ImVec2			size = {330, 0};

private:
	ImVec2				position;
	std::string			panel_title;

	PowerOnReset *		por;
};


Panel::uptr_t panel_chip_poweronreset_create(UIContext *ctx, struct ImVec2 pos, struct PowerOnReset *por, std::string id) {
	return std::make_unique<PanelChipPowerOnReset>(ctx, pos, por, id);
}

