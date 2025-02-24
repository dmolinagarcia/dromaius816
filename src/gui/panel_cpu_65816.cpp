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
		panel_title = ui_context->unique_panel_id("CPU - WDC 65c816");
		panel_id="CPU65816";
	}

	void display() override {
		ImGui::SetNextWindowPos(position, ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);

		if (ImGui::Begin(panel_title.c_str(), &stay_open)) {

			//>TODO adjust register witdh!

			if (ImGui::CollapsingHeader(txt_header_registers, ImGuiTreeNodeFlags_DefaultOpen)) {
				ui_register_16bit(8, "Accumulator", cpu->reg_a);
				ui_register_16bit(8, "index-X", cpu->reg_x);
				ui_register_16bit(8, "index-Y", cpu->reg_y);
				ui_register_16bit(8, "Stack Pointer", cpu->reg_sp);
				ui_register_16bit(8, "Program Counter", cpu->reg_pc);
				ui_register_8bit(8, "Instruction", cpu->reg_ir);
				ui_register_8bit(8, "Data Bank", cpu->reg_dbr);
				ui_register_8bit(8, "Prog Bank", cpu->reg_pbr);
			}

			if (ImGui::CollapsingHeader(txt_header_flags, ImGuiTreeNodeFlags_DefaultOpen)) {

				//> Adjust registers based on emulation mode
				// Emulation Mode
				if (ImGui::BeginTable("flags", 11, ImGuiTableFlags_Borders | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_SizingFixedFit)) {

					// header
					ImGui::TableSetupColumn(ImGuiEx::string_format("$%.2x", (cpu->reg_p) & 0xFF).c_str());
					ImGui::TableSetupColumn("N");
					ImGui::TableSetupColumn("V");
					ImGui::TableSetupColumn("-");
					ImGui::TableSetupColumn("B");
					ImGui::TableSetupColumn("D");
					ImGui::TableSetupColumn("I");
					ImGui::TableSetupColumn("Z");
					ImGui::TableSetupColumn("C");
					ImGui::TableSetupColumn("|");
					ImGui::TableSetupColumn("E");
					ImGui::TableHeadersRow();

					// values
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::TableNextColumn();	ImGui::Text("%d", FLAG_IS_SET(cpu->reg_p, FLAG_65816_N));
					ImGui::TableNextColumn();	ImGui::Text("%d", FLAG_IS_SET(cpu->reg_p, FLAG_65816_V));
					ImGui::TableNextColumn();	ImGui::Text("%d", FLAG_IS_SET(cpu->reg_p, FLAG_65816_U));
					ImGui::TableNextColumn();	ImGui::Text("%d", FLAG_IS_SET(cpu->reg_p, FLAG_65816_B));
					ImGui::TableNextColumn();	ImGui::Text("%d", FLAG_IS_SET(cpu->reg_p, FLAG_65816_D));
					ImGui::TableNextColumn();	ImGui::Text("%d", FLAG_IS_SET(cpu->reg_p, FLAG_65816_I));
					ImGui::TableNextColumn();	ImGui::Text("%d", FLAG_IS_SET(cpu->reg_p, FLAG_65816_Z));
					ImGui::TableNextColumn();	ImGui::Text("%d", FLAG_IS_SET(cpu->reg_p, FLAG_65816_C));
					ImGui::TableNextColumn();	ImGui::Text("|");
					ImGui::TableNextColumn();	ImGui::Text("%d", FLAG_IS_SET(cpu->reg_p, FLAG_65816_E));

					ImGui::EndTable();
				}


				// cpu->reg_p * ((bool) ((cpu->reg_p) & (FLAG_65816_E))) * 8
				// (cpu->reg_p >> (8*(((bool) ((cpu->reg_p) & (FLAG_65816_E))))) ) & 0xFF
				// (cpu->reg_p >> (8*(((bool) ((cpu->reg_p) & (FLAG_65816_E))))) )               

				// Native Mode
				if (ImGui::BeginTable("flags", 11, ImGuiTableFlags_Borders | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_SizingFixedFit)) {

					// header
					ImGui::TableSetupColumn(ImGuiEx::string_format("$%.2x", (cpu->reg_p >> 8 ) & 0xFF).c_str());
					ImGui::TableSetupColumn("N");
					ImGui::TableSetupColumn("V");
					ImGui::TableSetupColumn("M");
					ImGui::TableSetupColumn("X");
					ImGui::TableSetupColumn("D");
					ImGui::TableSetupColumn("I");
					ImGui::TableSetupColumn("Z");
					ImGui::TableSetupColumn("C");
					ImGui::TableSetupColumn("|");
					ImGui::TableSetupColumn("E");
					ImGui::TableHeadersRow();

					// values
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::TableNextColumn();	ImGui::Text("%d", FLAG_IS_SET(cpu->reg_p, FLAG_65816_N));
					ImGui::TableNextColumn();	ImGui::Text("%d", FLAG_IS_SET(cpu->reg_p, FLAG_65816_V));
					ImGui::TableNextColumn();	ImGui::Text("%d", FLAG_IS_SET(cpu->reg_p, FLAG_65816_M));
					ImGui::TableNextColumn();	ImGui::Text("%d", FLAG_IS_SET(cpu->reg_p, FLAG_65816_X));
					ImGui::TableNextColumn();	ImGui::Text("%d", FLAG_IS_SET(cpu->reg_p, FLAG_65816_D));
					ImGui::TableNextColumn();	ImGui::Text("%d", FLAG_IS_SET(cpu->reg_p, FLAG_65816_I));
					ImGui::TableNextColumn();	ImGui::Text("%d", FLAG_IS_SET(cpu->reg_p, FLAG_65816_Z));
					ImGui::TableNextColumn();	ImGui::Text("%d", FLAG_IS_SET(cpu->reg_p, FLAG_65816_C));
					ImGui::TableNextColumn();	ImGui::Text("|");
					ImGui::TableNextColumn();	ImGui::Text("%d", FLAG_IS_SET(cpu->reg_p, FLAG_65816_E));

					ImGui::EndTable();
				}

				// Current Mode
				if (ImGui::BeginTable("flags", 11, ImGuiTableFlags_Borders | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_SizingFixedFit)) {
					// header
					ImGui::TableSetupColumn(ImGuiEx::string_format("$%.2x", (cpu->reg_p >> (8*!(FLAG_IS_SET(cpu->reg_p, FLAG_65816_E))) ) & 0xFF).c_str());
					ImGui::TableSetupColumn("N");
					ImGui::TableSetupColumn("V");
					ImGui::TableSetupColumn(FLAG_IS_SET(cpu->reg_p, FLAG_65816_E) ? "-" : "M");
					ImGui::TableSetupColumn(FLAG_IS_SET(cpu->reg_p, FLAG_65816_E) ? "B" : "X");
					ImGui::TableSetupColumn("D");
					ImGui::TableSetupColumn("I");
					ImGui::TableSetupColumn("Z");
					ImGui::TableSetupColumn("C");
					ImGui::TableSetupColumn("|");
					ImGui::TableSetupColumn("E");
					ImGui::TableHeadersRow();
						// values
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::TableNextColumn();	ImGui::Text("%d", FLAG_IS_SET(cpu->reg_p, FLAG_65816_N));
					ImGui::TableNextColumn();	ImGui::Text("%d", FLAG_IS_SET(cpu->reg_p, FLAG_65816_V));
					ImGui::TableNextColumn();	ImGui::Text("%d", FLAG_IS_SET(cpu->reg_p, FLAG_65816_E) ? (FLAG_IS_SET(cpu->reg_p, FLAG_65816_U)) : (FLAG_IS_SET(cpu->reg_p, FLAG_65816_M)));
					ImGui::TableNextColumn();	ImGui::Text("%d", FLAG_IS_SET(cpu->reg_p, FLAG_65816_E) ? (FLAG_IS_SET(cpu->reg_p, FLAG_65816_B)) : (FLAG_IS_SET(cpu->reg_p, FLAG_65816_X)));
					ImGui::TableNextColumn();	ImGui::Text("%d", FLAG_IS_SET(cpu->reg_p, FLAG_65816_D));
					ImGui::TableNextColumn();	ImGui::Text("%d", FLAG_IS_SET(cpu->reg_p, FLAG_65816_I));
					ImGui::TableNextColumn();	ImGui::Text("%d", FLAG_IS_SET(cpu->reg_p, FLAG_65816_Z));
					ImGui::TableNextColumn();	ImGui::Text("%d", FLAG_IS_SET(cpu->reg_p, FLAG_65816_C));
					ImGui::TableNextColumn();	ImGui::Text("|");
					ImGui::TableNextColumn();	ImGui::Text("%d", FLAG_IS_SET(cpu->reg_p, FLAG_65816_E));
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

				if (ImGui::BeginTable("signals", 7, ImGuiTableFlags_Borders | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_SizingFixedFit)) {
					ImGui::TableSetupColumn("##type");
					ImGui::TableSetupColumn("/RES");
					ImGui::TableSetupColumn("/IRQ");
					ImGui::TableSetupColumn("/NMI");
					ImGui::TableSetupColumn("RDY");
					ImGui::TableSetupColumn("R/W");
					ImGui::TableSetupColumn("PHI2");
					ImGui::TableHeadersRow();

					ImGui::TableNextRow();
					ImGui::TableNextColumn();	ImGui::Text("Bus");
					ImGui::TableNextColumn();	ui_signal_short(SIGNAL_VALUE(RES_B));
					ImGui::TableNextColumn();	ui_signal_short(SIGNAL_VALUE(IRQ_B));
					ImGui::TableNextColumn();	ui_signal_short(SIGNAL_VALUE(NMI_B));
					ImGui::TableNextColumn();	ui_signal_short(SIGNAL_VALUE(RDY));
					ImGui::TableNextColumn();	ui_signal_short(SIGNAL_VALUE(RWB));
					ImGui::TableNextColumn();	ui_signal_short(SIGNAL_VALUE(PHI2));

					ImGui::TableNextRow();
					ImGui::TableNextColumn();	ImGui::Text("Out");
					ImGui::TableNextColumn();
					ImGui::TableNextColumn();
					ImGui::TableNextColumn();
					ImGui::TableNextColumn();
					ImGui::TableNextColumn();	ui_signal_short(SIGNAL_VALUE_AT_CHIP(RWB));
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
	std::string			panel_title;

	Cpu65816	*			cpu;
};

Panel::uptr_t panel_cpu_65816_create(UIContext *ctx, struct ImVec2 pos, struct Cpu65816 *cpu) {
	return std::make_unique<PanelCpu65816>(ctx, pos, cpu);
}

