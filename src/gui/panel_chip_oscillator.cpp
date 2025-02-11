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

//>>#define CR_FLAG(r,f)	FLAG_IS_SET((r), FLAG_6520_##f)
//>>	const char *selected_register() {
//>>		int reg_addr = (SIGNAL_READ(RS1) << 1) | static_cast<int>(SIGNAL_READ(RS0));
//>>
//>>		switch (reg_addr) {
//>>			case 0:		return (CR_FLAG(pia->reg_cra, DDR_OR_SELECT)) ? "ORA" : "DDRA";
//>>			case 1:		return "CRA";
//>>			case 2:		return (CR_FLAG(pia->reg_crb, DDR_OR_SELECT)) ? "ORB" : "DDRB";
//>>			case 3:		return "CRB";
//>>			default:	return "Unknown";
//>>		}
//>>	}

private:
	ImVec2				position;
	std::string			title;

	Oscillator *		tmr;
};


Panel::uptr_t panel_chip_oscillator_create(UIContext *ctx, struct ImVec2 pos, struct Oscillator *tmr, std::string id) {
	return std::make_unique<PanelChipOscillator>(ctx, pos, tmr, id);
}

