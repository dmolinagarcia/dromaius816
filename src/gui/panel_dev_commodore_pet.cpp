// gui/panel_dev_commodore_pet.cpp - Johan Smet - BSD-3-Clause (see LICENSE)

#include "panel_dev_commodore_pet.h"

#include <dev_commodore_pet.h>
//> COUT_DMG  #include <perif_pet_crt.h>

#include "ui_context.h"
#include "widgets.h"
#include "imgui_ex.h"

//> COUT_DMG  #include "panel_chip_6520.h"
//> COUT_DMG  #include "panel_chip_6522.h"
#include "panel_cpu_6502.h"
//> COUT_DMG #include "panel_memory.h"
//> COUT_DMG #include "panel_monitor.h"
//> COUT_DMG #include "panel_input_pet.h"
//> COUT_DMG #include "panel_display_rgba.h"
//> COUT_DMG #include "panel_signals.h"
#include "panel_logic_analyzer.h"
//> COUT_DMG #include "panel_datassette.h"
//> COUT_DMG #include "panel_disk_2031.h"

class PanelDevCommodorePet : public Panel {
public:
	PanelDevCommodorePet(UIContext *ctx, ImVec2 pos, DevCommodorePet *dev) :
		Panel(ctx),
		position(pos),
		device(dev) {

			panel_id = "CommodorePet";

//> COUT_DMG  		auto &cat_memory = pet_hardware.add_category("Memory");
//> COUT_DMG  		cat_memory.add_leaf("Main Ram")
//> COUT_DMG  					.add_action("View", [&]() {
//> COUT_DMG  						ui_context->panel_add(panel_memory_create(ui_context, {220, 120}, ui_context->unique_panel_id("RAM").c_str(), 0x0000, 0x8000));
//> COUT_DMG  					});
//> COUT_DMG  		cat_memory.add_leaf("Video Ram")
//> COUT_DMG  					.add_action("View", [&]() {
//> COUT_DMG  						ui_context->panel_add(panel_memory_create(ui_context, {220, 120}, ui_context->unique_panel_id("VRAM").c_str(), 0x8000, 40*25));
//> COUT_DMG  					});
//> COUT_DMG  		cat_memory.add_leaf("Basic Rom")
//> COUT_DMG  					.add_action("View", [&]() {
//> COUT_DMG  						ui_context->panel_add(panel_memory_create(ui_context, {442, 120}, ui_context->unique_panel_id("ROM").c_str(), 0xb000, 0x3000));
//> COUT_DMG  					});
//> COUT_DMG  		cat_memory.add_leaf("Editor Rom")
//> COUT_DMG  					.add_action("View", [&]() {
//> COUT_DMG  						ui_context->panel_add(panel_memory_create(ui_context, {442, 120}, ui_context->unique_panel_id("ROM").c_str(), 0xe000, 0x0800));
//> COUT_DMG  					});
//> COUT_DMG  		cat_memory.add_leaf("Kernal Rom")
//> COUT_DMG  					.add_action("View", [&]() {
//> COUT_DMG  						ui_context->panel_add(panel_memory_create(ui_context, {442, 120}, ui_context->unique_panel_id("ROM").c_str(), 0xf000, 0x1000));
//> COUT_DMG  					});

		auto &cat_cpu = pet_hardware.add_category("CPU");
		cat_cpu.add_leaf("MOS Technology 6502")
					.add_action("View", [&]() {
						ui_context->panel_add(panel_cpu_6502_create(ui_context, {220, 360}, device->cpu));
					});

//> COUT_DMG  		auto &cat_support = pet_hardware.add_category("Support Chips");
//> COUT_DMG  		cat_support.add_leaf("PIA (6520) - IEEE-488")
//> COUT_DMG  					.add_action("View", [&]() {
//> COUT_DMG  						ui_context->panel_add(panel_chip_6520_create(ui_context, {420, 342}, device->pia_1));
//> COUT_DMG  					});

//> COUT_DMG  		cat_support.add_leaf("PIA (6520) - Keyboard")
//> COUT_DMG  					.add_action("View", [&]() {
//> COUT_DMG  						ui_context->panel_add(panel_chip_6520_create(ui_context, {420, 342}, device->pia_2));
//> COUT_DMG  					});
//> COUT_DMG  		cat_support.add_leaf("VIA (6522)")
//> COUT_DMG  					.add_action("View", [&]() {
//> COUT_DMG  						ui_context->panel_add(panel_chip_6522_create(ui_context, {420, 342}, device->via));
//> COUT_DMG  					});

//> COUT_DMG  		auto &cat_perif = pet_hardware.add_category("Peripherals");
//> COUT_DMG  		cat_perif.add_leaf("Datassette")
//> COUT_DMG  					.add_action("Open", [&]() {
//> COUT_DMG  						ui_context->panel_add(panel_datassette_create(ui_context, {340, 310}, device->datassette));
//> COUT_DMG  					});
//> COUT_DMG  		cat_perif.add_leaf("Floppy Disk")
//> COUT_DMG  					.add_action("Open", [&]() {
//> COUT_DMG  						ui_context->panel_add(panel_fd2031_create(ui_context, {340, 310}, device->disk_2031));
//> COUT_DMG  					});

		auto &cat_tools = pet_hardware.add_category("Tools");
//> COUT_DMG  		cat_tools.add_leaf("Monitor")
//> COUT_DMG  					.add_action("Open", [&]() {
//> COUT_DMG  						ui_context->panel_add(panel_monitor_create(ui_context, {340, 310}));
//> COUT_DMG  					});
		cat_tools.add_leaf("Logic Analyzer")
					.add_action("Open", [&]() {
 						ui_context->panel_add(panel_logic_analyzer_create(ui_context, {340, 310}));
					});
//> COUT_DMG  		cat_tools.add_leaf("Signal Debugger")
//> COUT_DMG  					.add_action("Open", [&]() {
//> COUT_DMG  						ui_context->panel_add(panel_signals_create(ui_context, {340, 310}));
//> COUT_DMG  					});

		diag_mode = device->diag_mode;
	}

	void display() override {

		ImGui::SetNextWindowPos(position, ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);

		if (ImGui::Begin(panel_title)) {

			pet_hardware.display();

 			if (ImGui::CollapsingHeader("Advanced")) {
 				if (ImGui::Checkbox("Assert Diagnostice Sense Line", &diag_mode)) {
 					dev_commodore_pet_diag_mode(device, diag_mode);
 				}
 			}
		}

		ImGui::End();
	}

private:
	ImVec2			position;
	const ImVec2	size = {330, 0};
	DevCommodorePet *device;

	UITree			pet_hardware;
	bool			diag_mode = false;

	constexpr static const char *panel_title = "Device - Commodore PET 2001N";
};

Panel::uptr_t panel_dev_commodore_pet_create(UIContext *ctx, ImVec2 pos, DevCommodorePet *device) {

	// a keyboard panel is always useful
//> COUT_DMG  	auto keyboard_pnl = panel_input_pet_create(ctx, {340, 512}, device->keypad);
//> COUT_DMG  	ctx->panel_add(std::move(keyboard_pnl));

	// display panel
//> COUT_DMG  	auto display_pnl = panel_display_rgba_create(ctx, {340, 10}, device->screen);
//> COUT_DMG 	ctx->panel_add(std::move(display_pnl));

	// create panel for the commodore pet
	return std::make_unique<PanelDevCommodorePet>(ctx, pos, device);
}
