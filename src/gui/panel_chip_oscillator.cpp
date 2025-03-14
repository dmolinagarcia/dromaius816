// gui/panel_chip_oscillator.c - Daniel Molina - BSD-3-Clause (see LICENSE)
//
// UI panel to display information about the oscillator

#include "panel_chip_oscillator.h"
#include "chip_oscillator.h"
#include "context.h"
#include "utils.h"
#include "widgets.h"
#include "ui_context.h"

#include <imgui.h>
#include <stb/stb_ds.h>
#include <assert.h>

#define SIGNAL_PREFIX		CHIP_OSCILLATOR_
#define SIGNAL_OWNER		tmr

class PanelChipOscillator : public Panel {
public:
	PanelChipOscillator(UIContext *ctx, ImVec2 pos, Oscillator *tmr, std::string id) :
		Panel(ctx),
		position(pos),
		tmr(tmr) {
		title = ui_context->unique_panel_id("Oscillator");
		panel_id="Oscillator " + id;
	}

	void display() override {

		ImGui::SetNextWindowPos(position, ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);

		if (ImGui::Begin(title.c_str(), &stay_open)) {
			if (ImGui::BeginTable("signals", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_SizingFixedFit)) {
					ImGui::TableSetupColumn("Signal");
					ImGui::TableSetupColumn("Value");
					ImGui::TableHeadersRow();
					ImGui::TableNextColumn();	ImGui::Text("CLK_OUT");
					ImGui::TableNextColumn();	ui_signal_short(SIGNAL_VALUE(CLK_OUT));
					ImGui::EndTable();
				}			
		}

		ImGui::End();
	}

private:
	const ImVec2			size = {330, 0};

private:
	ImVec2				position;
	std::string			title;

	Oscillator *		tmr;
};


Panel::uptr_t panel_chip_oscillator_create(UIContext *ctx, struct ImVec2 pos, struct Oscillator *tmr, std::string id) {
	return std::make_unique<PanelChipOscillator>(ctx, pos, tmr, id);
}

