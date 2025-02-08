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
	PanelChipOscillator(UIContext *ctx, ImVec2 pos, Oscillator *tmr) :
		Panel(ctx),
		position(pos),
		tmr(tmr) {
		title = ui_context->unique_panel_id("Oscillator");
	}

	void display() override {

//>>		SignalValue sig_values[8];

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

//>>			if (ImGui::CollapsingHeader("Ports", ImGuiTreeNodeFlags_DefaultOpen)) {
//>>
//>>				if (ui_bit_array_table_start("databus", 8, true, "Data  ")) {
//>>
//>>					SIGNAL_GROUP_VALUE(data, sig_values);
//>>					ui_bit_array_table_row("Bus", 8, sig_values);
//>>
//>>					SIGNAL_GROUP_VALUE_AT_CHIP(data, sig_values);
//>>					ui_bit_array_table_row("Out", 8, sig_values);
//>>
//>>					ImGui::EndTable();
//>>				}
//>>
//>>				if (ui_bit_array_table_start("port_a", 8, true, "Port-A")) {
//>>
//>>					SIGNAL_GROUP_VALUE(port_a, sig_values);
//>>					ui_bit_array_table_row("Bus", 8, sig_values);
//>>
//>>					SIGNAL_GROUP_VALUE_AT_CHIP(port_a, sig_values);
//>>					ui_bit_array_table_row("Out", 8, sig_values);
//>>
//>>					ImGui::EndTable();
//>>				}
//>>
//>>				if (ui_bit_array_table_start("port_b", 8, true, "Port-B")) {
//>>
//>>					SIGNAL_GROUP_VALUE(port_b, sig_values);
//>>					ui_bit_array_table_row("Bus", 8, sig_values);
//>>
//>>					SIGNAL_GROUP_VALUE_AT_CHIP(port_b, sig_values);
//>>					ui_bit_array_table_row("Out", 8, sig_values);
//>>
//>>					ImGui::EndTable();
//>>				}
//>>			}
//>>
//>>			if (ImGui::CollapsingHeader("Signals", ImGuiTreeNodeFlags_DefaultOpen)) {
//>>				if (ImGui::BeginTable("signals_a", 7, ImGuiTableFlags_Borders | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_SizingFixedFit)) {
//>>					ImGui::TableSetupColumn("##type");
//>>					ImGui::TableSetupColumn("CA1");
//>>					ImGui::TableSetupColumn("CA2");
//>>					ImGui::TableSetupColumn("CB1");
//>>					ImGui::TableSetupColumn("CB2");
//>>					ImGui::TableSetupColumn("/IRQA");
//>>					ImGui::TableSetupColumn("/IRQB");
//>>					ImGui::TableHeadersRow();
//>>
//>>					ImGui::TableNextRow();
//>>					ImGui::TableNextColumn();	ImGui::Text("Bus");
//>>					ImGui::TableNextColumn();	ui_signal_short(SIGNAL_VALUE(CA1));
//>>					ImGui::TableNextColumn();	ui_signal_short(SIGNAL_VALUE(CA2));
//>>					ImGui::TableNextColumn();	ui_signal_short(SIGNAL_VALUE(CB1));
//>>					ImGui::TableNextColumn();	ui_signal_short(SIGNAL_VALUE(CB2));
//>>					ImGui::TableNextColumn();	ui_signal_short(SIGNAL_VALUE(IRQA_B));
//>>					ImGui::TableNextColumn();	ui_signal_short(SIGNAL_VALUE(IRQB_B));
//>>
//>>					ImGui::TableNextRow();
//>>					ImGui::TableNextColumn();	ImGui::Text("Out");
//>>					ImGui::TableNextColumn();
//>>					ImGui::TableNextColumn();	ui_signal_short(SIGNAL_VALUE_AT_CHIP(CA2));
//>>					ImGui::TableNextColumn();
//>>					ImGui::TableNextColumn();	ui_signal_short(SIGNAL_VALUE_AT_CHIP(CB2));
//>>					ImGui::TableNextColumn();	ui_signal_short(SIGNAL_VALUE_AT_CHIP(IRQA_B));
//>>					ImGui::TableNextColumn();	ui_signal_short(SIGNAL_VALUE_AT_CHIP(IRQB_B));
//>>
//>>					ImGui::EndTable();
//>>				}
//>>
//>>				if (ImGui::BeginTable("signals_b", 7, ImGuiTableFlags_Borders | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_SizingFixedFit)) {
//>>
//>>					ImGui::TableSetupColumn("##type");
//>>					ImGui::TableSetupColumn("/RES");
//>>					ImGui::TableSetupColumn("CS0");
//>>					ImGui::TableSetupColumn("CS1");
//>>					ImGui::TableSetupColumn("/CS2");
//>>					ImGui::TableSetupColumn("RS0");
//>>					ImGui::TableSetupColumn("RS1");
//>>					ImGui::TableHeadersRow();
//>>
//>>					ImGui::TableNextColumn();	ImGui::Text("Bus");
//>>					ImGui::TableNextColumn();	ui_signal_short(SIGNAL_VALUE(RESET_B));
//>>					ImGui::TableNextColumn();	ui_signal_short(SIGNAL_VALUE(CS0));
//>>					ImGui::TableNextColumn();	ui_signal_short(SIGNAL_VALUE(CS1));
//>>					ImGui::TableNextColumn();	ui_signal_short(SIGNAL_VALUE(CS2_B));
//>>					ImGui::TableNextColumn();	ui_signal_short(SIGNAL_VALUE(RS0));
//>>					ImGui::TableNextColumn();	ui_signal_short(SIGNAL_VALUE(RS1));
//>>
//>>					ImGui::EndTable();
//>>				}
//>>			}
//>>
//>>			if (ImGui::CollapsingHeader("Status", ImGuiTreeNodeFlags_DefaultOpen)) {
//>>				bool selected = SIGNAL_READ(CS0) && SIGNAL_READ(CS1) && !SIGNAL_READ(CS2_B);
//>>				ImGui::Text("Chip selected  : %s", (selected) ? "true": "false");
//>>				ImGui::Text("Register select: %s", selected_register());
//>>			}
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


Panel::uptr_t panel_chip_oscillator_create(UIContext *ctx, struct ImVec2 pos, struct Oscillator *tmr) {
	return std::make_unique<PanelChipOscillator>(ctx, pos, tmr);
}

