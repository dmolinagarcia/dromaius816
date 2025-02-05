// gui/panel_cpu_65816.c - Daniel Molina - BSD-3-Clause (see LICENSE)
// based on
// gui/panel_cpu_6502.c - Johan Smet - BSD-3-Clause (see LICENSE)
//
// UI panel to display information about a MOS 65816

#include "panel_cpu_65816.h"

#include "widgets.h"
#include "cpu_65816.h"
#include "ui_context.h"
#include <imgui.h>

#define SIGNAL_OWNER		cpu
#define SIGNAL_PREFIX		PIN_65816_

class PanelCpu65816 : public Panel {
public:
	PanelCpu65816(UIContext *ctx, ImVec2 pos, Cpu65816 *cpu) :
		Panel(ctx),
		position(pos),
		cpu(cpu) {
		title = ui_context->unique_panel_id("CPU - MOS 65816");
	}

	void display() override {
		ImGui::SetNextWindowPos(position, ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);

		if (ImGui::Begin(title.c_str(), &stay_open)) {

			if (ImGui::CollapsingHeader(txt_header_registers, ImGuiTreeNodeFlags_DefaultOpen)) {
				ui_register_8bit(8, "Accumulator", cpu->reg_a);
				ui_register_8bit(8, "index-X", cpu->reg_x);
				ui_register_8bit(8, "index-Y", cpu->reg_y);
				ui_register_8bit(8, "Stack Pointer", cpu->reg_sp);
				ui_register_16bit(8, "Program Counter", cpu->reg_pc);
				ui_register_8bit(8, "Instruction", cpu->reg_ir);
			}

			if (ImGui::CollapsingHeader(txt_header_flags, ImGuiTreeNodeFlags_DefaultOpen)) {

				if (ImGui::BeginTable("flags", 9, ImGuiTableFlags_Borders | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_SizingFixedFit)) {

					// header
					ImGui::TableSetupColumn(ImGuiEx::string_format("$%.2x", cpu->reg_p).c_str());
					ImGui::TableSetupColumn("N");
					ImGui::TableSetupColumn("V");
					ImGui::TableSetupColumn("-");
					ImGui::TableSetupColumn("B");
					ImGui::TableSetupColumn("D");
					ImGui::TableSetupColumn("I");
					ImGui::TableSetupColumn("Z");
					ImGui::TableSetupColumn("C");
					ImGui::TableHeadersRow();

					// values
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::TableNextColumn();	ImGui::Text("%d", FLAG_IS_SET(cpu->reg_p, FLAG_65816_N));
					ImGui::TableNextColumn();	ImGui::Text("%d", FLAG_IS_SET(cpu->reg_p, FLAG_65816_V));
					ImGui::TableNextColumn();	ImGui::Text("%d", FLAG_IS_SET(cpu->reg_p, FLAG_65816_E));
					ImGui::TableNextColumn();	ImGui::Text("%d", FLAG_IS_SET(cpu->reg_p, FLAG_65816_B));
					ImGui::TableNextColumn();	ImGui::Text("%d", FLAG_IS_SET(cpu->reg_p, FLAG_65816_D));
					ImGui::TableNextColumn();	ImGui::Text("%d", FLAG_IS_SET(cpu->reg_p, FLAG_65816_I));
					ImGui::TableNextColumn();	ImGui::Text("%d", FLAG_IS_SET(cpu->reg_p, FLAG_65816_Z));
					ImGui::TableNextColumn();	ImGui::Text("%d", FLAG_IS_SET(cpu->reg_p, FLAG_65816_C));

					ImGui::EndTable();
				}
			}

			if (ImGui::CollapsingHeader(txt_header_signals, ImGuiTreeNodeFlags_DefaultOpen)) {

				// address bus
				ImGui::Text("Address Bus: $%.4x", SIGNAL_GROUP_READ_U16(address));
				if (ui_bit_array_table_start("address_bits", 16, false)) {
					SignalValue sig_addr[16];

					SIGNAL_GROUP_VALUE(address, sig_addr);
					ui_bit_array_table_row(nullptr, 16, sig_addr);

					ui_bit_array_table_end();
				}

				// data bus
				ImGui::Text("Data Bus: $%.2x", SIGNAL_GROUP_READ_U8(data));

				if (ui_bit_array_table_start("data_bits", 8, true)) {

					SignalValue sig_data[8];

					SIGNAL_GROUP_VALUE(data, sig_data);
					ui_bit_array_table_row("Bus", 8, sig_data);

					SIGNAL_GROUP_VALUE_AT_CHIP(data, sig_data);
					ui_bit_array_table_row("Out", 8, sig_data);

					ImGui::EndTable();
				}

				// remaining signals
				ImGui::Text("Remaining Signals");

				if (ImGui::BeginTable("signals", 8, ImGuiTableFlags_Borders | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_SizingFixedFit)) {
					ImGui::TableSetupColumn("##type");
					ImGui::TableSetupColumn("/RES");
					ImGui::TableSetupColumn("/IRQ");
					ImGui::TableSetupColumn("/NMI");
					ImGui::TableSetupColumn("RDY");
					ImGui::TableSetupColumn("SYNC");
					ImGui::TableSetupColumn("R/W");
					ImGui::TableSetupColumn("PHI2");
					ImGui::TableHeadersRow();

					ImGui::TableNextRow();
					ImGui::TableNextColumn();	ImGui::Text("Bus");
					ImGui::TableNextColumn();	ui_signal_short(SIGNAL_VALUE(RES_B));
					ImGui::TableNextColumn();	ui_signal_short(SIGNAL_VALUE(IRQ_B));
					ImGui::TableNextColumn();	ui_signal_short(SIGNAL_VALUE(NMI_B));
					ImGui::TableNextColumn();	ui_signal_short(SIGNAL_VALUE(RDY));
					ImGui::TableNextColumn();	ui_signal_short(SIGNAL_VALUE(SYNC));
					ImGui::TableNextColumn();	ui_signal_short(SIGNAL_VALUE(RW));
					ImGui::TableNextColumn();	ui_signal_short(SIGNAL_VALUE(CLK));

					ImGui::TableNextRow();
					ImGui::TableNextColumn();	ImGui::Text("Out");
					ImGui::TableNextColumn();
					ImGui::TableNextColumn();
					ImGui::TableNextColumn();
					ImGui::TableNextColumn();
					ImGui::TableNextColumn();	ui_signal_short(SIGNAL_VALUE_AT_CHIP(SYNC));
					ImGui::TableNextColumn();	ui_signal_short(SIGNAL_VALUE_AT_CHIP(RW));
					ImGui::TableNextColumn();	

					ImGui::EndTable();
				}
			}
		}

		ImGui::End();
	}

private:
	const ImVec2			size = {330, 0};

	static constexpr const char *txt_header_registers = "Registers";
	static constexpr const char *txt_header_flags = "Status Flags";
	static constexpr const char *txt_header_signals = "Signals";

	static constexpr const char *table_header_digits[] = {
		"0", "1", "2", "3", "4", "5", "6", "7",
		"8", "9", "A", "B", "C", "D", "E", "F"
	};


private:
	ImVec2				position;
	std::string			title;

	Cpu65816	*			cpu;
};

Panel::uptr_t panel_cpu_65816_create(UIContext *ctx, struct ImVec2 pos, struct Cpu65816 *cpu) {
	return std::make_unique<PanelCpu65816>(ctx, pos, cpu);
}

