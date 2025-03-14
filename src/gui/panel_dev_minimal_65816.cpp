// gui/panel_dev_minimal_65816.cpp - Daniel Molina - BSD-3-Clause (see LICENSE)
// based on
// gui/panel_dev_minimal_6502.cpp - Johan Smet - BSD-3-Clause (see LICENSE)

#include "panel_dev_minimal_65816.h"

#include <dev_minimal_65816.h>
#include "ui_context.h"
#include "widgets.h"

//> COUT_DMG #include "panel_chip_6520.h"
//> COUT_DMG #include "panel_chip_hd44780.h"
#include "panel_cpu_65816.h"
#include "panel_chip_oscillator.h"
#include "panel_chip_poweronreset.h"
#include "panel_logic_analyzer.h"
//> COUT_DMG #include "panel_input_keypad.h"
#include "panel_memory.h"
//> COUT_DMG #include "panel_monitor.h"
//> COUT_DMG #include "panel_signals.h"
#include "popup_file_selector.h"

namespace {

static std::string binary_path = "runtime/minimal_65816";

static inline std::string path_for_binary(const std::string & filename) {
	std::string result = binary_path;
	result += "/";
	result += filename;

	return result;
}

} // unnamed namespace

class PanelDevMinimal65816 : public Panel {
public:

	PanelDevMinimal65816(UIContext *ctx, ImVec2 pos, DevMinimal65816 *dev) :
		Panel(ctx),
		position(pos),
		device(dev) {

		panel_id="Minimal65816";

//> COUT_DMG 		// load default rom image
//> COUT_DMG 		auto roms = rom_selection->construct_directory_listing();
//> COUT_DMG 		if (!roms.empty()) {
//> COUT_DMG 			rom_last_loaded = roms[0];
//> COUT_DMG 			dev_minimal_65816_rom_from_file(device, path_for_binary(roms[0]).c_str());
//> COUT_DMG 		}
//> COUT_DMG 
		// construct hardware list
		auto &cat_memory = hardware_list.add_category("Memory");
		cat_memory.add_leaf("RAM (32k)")
					.add_action("View", [&]() {
						ui_context->panel_add(panel_memory_create(ui_context, {2, 120}, ui_context->unique_panel_id("RAM").c_str(), 0x0000, 0xFFFF, device->cpu));
					})
					.add_action("Load", [&]() {
						load_ram = true;
					});
//> COUT_DMG 		cat_memory.add_leaf("ROM (16k)")
//> COUT_DMG 					.add_action("View", [&]() {
//> COUT_DMG 						ui_context->panel_add(panel_memory_create(ui_context, {442, 120}, ui_context->unique_panel_id("ROM").c_str(), 0xc000, 0x4000));
//> COUT_DMG 					})
//> COUT_DMG 					.add_action("Load", [&]() {
//> COUT_DMG 						load_rom = true;
//> COUT_DMG 					});
//> COUT_DMG 
		auto &cat_tmr = hardware_list.add_category("TIMER & CONTROL");
		cat_tmr.add_leaf("Main Clock")
					.add_action("View", [&]() {
						ui_context->panel_add(panel_chip_oscillator_create(ui_context, {2, 342}, device->oscillator, "Y1"));
					});
		cat_tmr.add_leaf("Power on Reset")
					.add_action("View", [&]() {
						ui_context->panel_add(panel_chip_poweronreset_create(ui_context, {2, 342}, device->poweronreset, "R1"));
					});


		auto &cat_cpu = hardware_list.add_category("CPU");
		cat_cpu.add_leaf("WDC 65c816")
					.add_action("View", [&]() {
						ui_context->panel_add(panel_cpu_65816_create(ui_context, {2, 342}, device->cpu));
					});

//> COUT_DMG 		auto &cat_support = hardware_list.add_category("Support Chips");
//> COUT_DMG 		cat_support.add_leaf("PIA (6520)")
//> COUT_DMG 					.add_action("View", [&]() {
//> COUT_DMG 						ui_context->panel_add(panel_chip_6520_create(ui_context, {420, 342}, device->pia));
//> COUT_DMG 					});

		auto &cat_tools = hardware_list.add_category("Tools");
		cat_tools.add_leaf("Logic Analyzer")
					.add_action("Open", [&]() {
 						ui_context->panel_add(panel_logic_analyzer_create(ui_context, {340, 310}));
					});

//> COUT_DMG 		cat_tools.add_leaf("Monitor")
//> COUT_DMG 					.add_action("Open", [&]() {
//> COUT_DMG 						ui_context->panel_add(panel_monitor_create(ui_context, {340, 310}));
//> COUT_DMG 					});
//> COUT_DMG 		cat_tools.add_leaf("Signal Debugger")
//> COUT_DMG 					.add_action("Open", [&]() {
//> COUT_DMG 						ui_context->panel_add(panel_signals_create(ui_context, {340, 310}));
//> COUT_DMG 					});

//> COUT_DMG		auto &cat_sch = hardware_list.add_category("SCHEMATIC");

	}

	void display() override {

		ImGui::SetNextWindowPos(position, ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);

		load_rom = false;
		load_ram = false;

		if (ImGui::Begin(title)) {

			hardware_list.display();

//> COUT_DMG 			if (!rom_last_loaded.empty() || !ram_last_loaded.empty()) {
//> COUT_DMG 				ImGui::Spacing();
//> COUT_DMG 				ImGui::Separator();
//> COUT_DMG 				if (!rom_last_loaded.empty()) {
//> COUT_DMG 					ImGui::Text("Loaded ROM: %s", rom_last_loaded.c_str());
//> COUT_DMG 				}
//> COUT_DMG 				if (!ram_last_loaded.empty()) {
//> COUT_DMG 					ImGui::Text("Loaded RAM: %s", ram_last_loaded.c_str());
//> COUT_DMG 				}
//> COUT_DMG 			}
		}

		ImGui::End();

		rom_selection->define_popup();
		if (load_rom) {
			rom_selection->display_popup([&](std::string selected_file) {
				dev_minimal_65816_rom_from_file(device, path_for_binary(selected_file).c_str());
				rom_last_loaded = selected_file;
			});
		}

		ram_selection->define_popup();
		if (load_ram) {
			ram_selection->display_popup([&](std::string selected_file) {
				dev_minimal_65816_ram_from_file(device, path_for_binary(selected_file).c_str());
				ram_last_loaded = selected_file;
			});
		}
	}

private:
	ImVec2			position;
	DevMinimal65816 *device;
	const ImVec2			size = {330, 0};

	UITree			hardware_list;

	bool			load_rom;
	bool			load_ram;
	std::string		rom_last_loaded = "";
	std::string		ram_last_loaded = "";

	PopupFileSelector::uptr_t rom_selection = PopupFileSelector::make_unique(ui_context, "runtime/minimal_65816", ".bin", "rom_");
	PopupFileSelector::uptr_t ram_selection = PopupFileSelector::make_unique(ui_context, "runtime/minimal_65816", ".bin", "ram_");

	constexpr static const char *title = "Device - Minimal 65816";
};

Panel::uptr_t panel_dev_minimal_65816_create(UIContext *ctx, ImVec2 pos, DevMinimal65816 *device) {

//> COUT_DMG 	// always create an output panel
//> COUT_DMG 	auto lcd_pnl = panel_chip_hd44780_create(ctx, {340, 0}, device->lcd);
//> COUT_DMG 	ctx->panel_add(std::move(lcd_pnl));
//> COUT_DMG 
//> COUT_DMG 	// a keypad panel is always useful
//> COUT_DMG 	auto keypad_pnl = panel_input_keypad_create(ctx, {340, 200}, device->keypad);
//> COUT_DMG 	ctx->panel_add(std::move(keypad_pnl));

	// create panel for the minimal_65816
	return std::make_unique<PanelDevMinimal65816>(ctx, pos, device);
}
