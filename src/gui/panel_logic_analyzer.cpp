// gui/panel_logic_analyzer.cpp - Johan Smet - BSD-3-Clause (see LICENSE)
//
// visualize signal state

#include "panel_logic_analyzer.h"
#include "signal_history.h"
#include "context.h"
#include "ui_context.h"
#include "device.h"

#include "imgui_ex.h"
#include <imgui.h>
#include <vector>
#include <algorithm>

class PanelLogicAnalyzer : public Panel {
public:
	PanelLogicAnalyzer(UIContext *ctx, ImVec2 pos) :
		Panel(ctx),
		position(pos) {
		input[0] = '\0';
		title = ui_context->unique_panel_id("LogicAnalyzer");
		panel_id="LogicAnalyzer";
	}

	void init() override {
		// 20 pixels = 1 timebase (default: 1 µs = 1000 ns == 1 MHz)
		time_scale = (1000000.0f / (float) time_base) / (20.0f * (float) ui_context->device->simulator->tick_duration_ps);
		enable_history = ui_context->device->simulator->signal_history->capture_active;
	}

	void display() override {

		ImGui::SetNextWindowPos(position, ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);

		if (ImGui::Begin(title.c_str(), &stay_open)) {

			auto sim = ui_context->device->simulator;

			// header

			// >> enable history keeping
			if (ImGui::Checkbox("Enable", &enable_history)) {
				if (enable_history) {
					signal_history_process_start(sim->signal_history);
				} else {
					signal_history_process_stop(sim->signal_history);
					enable_gtkwave = false;
				}
			}

			// >> enable GtkWave output
#ifdef DMS_GTKWAVE_EXPORT
			ImGui::SameLine();

			if (ImGui::Checkbox("GtkWave export", &enable_gtkwave)) {
				if (enable_gtkwave) {
					signal_history_gtkwave_enable(	sim->signal_history, "dromaius.lxt",
													arrlenu(diagram_data.signals),
													diagram_data.signals,
													sim->signal_pool->signals_name);
				} else {
					signal_history_gtkwave_disable(sim->signal_history);
				}
			}
#endif // DMS_GTKWAVE_EXPORT

			// >> choose profile
			ImGui::SameLine(0, 12);
			ImGui::Text("Profile");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(256);
			ImGui::Combo("##profiles", &current_profile,
						 signal_history_profile_names(sim->signal_history),
						 (int) signal_history_profile_count(sim->signal_history));

			ImGui::SameLine();
			ImGui::BeginDisabled(enable_gtkwave);
			if (ImGui::Button(" Add ##add_profile")) {
				add_profile_signals();
			}
			ImGui::EndDisabled();

			// >> add new signal
			if (ImGui::IsWindowFocused() && !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0)) {
				ImGui::SetKeyboardFocusHere(0);
			}

			ImGui::SameLine(0, 12);
			ImGui::Text("Signal");
			ImGui::SameLine();

			ImGui::SetNextItemWidth(128);
			if (ImGui::InputText("##input", input, sizeof(input) - 1, ImGuiInputTextFlags_EnterReturnsTrue)) {
				add_input_signal();
			}

			ImGui::SameLine();

			ImGui::BeginDisabled(valid_input_signal());
			if (ImGui::Button(" Add ##add")) {
				add_input_signal();
			}
			ImGui::EndDisabled();

			ImGui::SameLine(0, 12);
			ImGui::Text("Scale");
			ImGui::SameLine();

			// Scale controller
			ImGui::SetNextItemWidth(64);
			if (ImGui::DragFloat("##base", &time_base, 0.1f, 1.0f, 32.0f, "%.1f")) {
    			time_scale = (1000000.0f / time_base) / (20.0f * (float) ui_context->device->simulator->tick_duration_ps);
			}
			handle_mouse_scroll();

			// divider between header and body
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			// body
			auto region = ImGui::GetContentRegionAvail();

			// Body size is calcualted based on signal number.
			// This needs to be adjusted, based on buses presence so... we calculate buses first.

				// Identify buses

			    std::vector<int> address_bus_index(16, -1);
			    for (size_t i = 0; i < signal_names.size(); ++i) {
			        for (size_t bit = 0; bit < 16; ++bit) {
			            std::string expected_name = "AB" + std::to_string(bit);
			            if (signal_names[i] == expected_name) {
			                address_bus_index[bit] = static_cast<int>(i);
			                break;
			            }
			        }
			    }

			    std::vector<int> data_bus_index(8, -1);
			    for (size_t i = 0; i < signal_names.size(); ++i) {
			        for (size_t bit = 0; bit < 8; ++bit) {
			            std::string expected_name = "DB" + std::to_string(bit);
			            if (signal_names[i] == expected_name) {
			                data_bus_index[bit] = static_cast<int>(i);
			                break;
			            }
			        }
			    }

			    // Have we found the whole bus?
			    bool address_bus_complete = std::all_of(address_bus_index.begin(), address_bus_index.end(), [](int idx) { return idx != -1; });
			    bool data_bus_complete = std::all_of(data_bus_index.begin(), data_bus_index.end(), [](int idx) { return idx != -1; });			

			// Adjust region size based on number of signals, minus buses
			ImGui::SetNextWindowContentSize({region.x, (float) (signal_names.size()-(data_bus_complete*7)-(address_bus_complete*15)) * 40.0f});
			ImGui::BeginChild("display", region, false, 0);

			// >> calculate what time interval to fetch data for
			int64_t available_time = (int64_t) ((region.x - (BORDER_WIDTH * 2.0f)) * time_scale);
			diagram_data.time_end = sim->current_tick;
			diagram_data.time_begin = diagram_data.time_end - available_time;
			if (diagram_data.time_begin < 0) {
				diagram_data.time_begin = 0;
				diagram_data.time_end = available_time;
			}

			// >> fetch data
			signal_history_diagram_data(ui_context->device->simulator->signal_history, &diagram_data);

			// >> context menu
			context_menu();

			// >> display data

			//> Daniel Molina
			//> Bus representacion starts here

				// signal_post represents location in window. 

				size_t signal_pos=0;

				if (address_bus_complete) {
					// If bus is complete, draw it
					draw_bus("AB", address_bus_index, signal_pos);
					signal_pos++;
				} else {
					// Bus is incomplete, draw individual signals
					address_bus_index.clear();
				}

				if (data_bus_complete) {
					// If bus is complete, draw it
					// draw_bus(data, signal_pos)
					draw_bus("DB", data_bus_index, signal_pos);
					signal_pos++;
				} else {
					// Bus is incomplete, draw individual signals
					data_bus_index.clear();
				}

			// END of BUSSES
			for (size_t si = 0; si < arrlenu(diagram_data.signals); ++si) {
				if (std::find(address_bus_index.begin(), address_bus_index.end(), static_cast<int>(si)) == address_bus_index.end() &&
				    std::find(data_bus_index.begin(), data_bus_index.end(), static_cast<int>(si)) == data_bus_index.end() 
					) {
					// Draw signal if it is not part of a bus
    				draw_signal(si, signal_pos);
					signal_pos++;
				}
			}

			// >> vertical line at cursor
			draw_vertical_guide();

			ImGui::EndChild();

		}
		ImGui::End();
	}

private:
	static constexpr float BORDER_WIDTH = 10;
	static constexpr  auto COLOR_SIGNAL = IM_COL32(0, 175, 0, 255);
	static constexpr  auto COLOR_TEXT = IM_COL32(80, 30, 80, 255);
	static constexpr  auto COLOR_WHITE = IM_COL32(255, 255, 255, 255);
	static constexpr  auto COLOR_LABEL = IM_COL32(150, 150, 150, 200);
	static constexpr  auto COLOR_HOVERED = IM_COL32(150, 150, 50, 200);
	static constexpr ImU32 COLOR_BACKGROUND[2] = {IM_COL32(30, 30, 30, 128), IM_COL32(60, 60, 60, 128)};
	static constexpr  auto COLOR_GUIDE = IM_COL32(255, 0, 0, 128);
	static constexpr float TRUE_OFFSET = -20.0f;

	void draw_signal(size_t signal_idx, size_t signal_pos) {

		auto draw_list = ImGui::GetWindowDrawList();
		auto origin = ImGui::GetCursorScreenPos();
		auto region = ImGui::GetContentRegionAvail();
		auto sim = ui_context->device->simulator;

		float signal_y = origin.y + ((float) signal_pos * 40.0f) + 30.0f;

		draw_list->AddRectFilled({origin.x, signal_y - 30.0f}, {origin.x + region.x, signal_y + 10.0f}, COLOR_BACKGROUND[signal_pos % 2]);


		auto diagram_x = [&](int64_t t) -> float {
			return origin.x + BORDER_WIDTH + (float) (t - diagram_data.time_begin) / time_scale;
		};

		// draw from right to left (starting at current position)
		if (diagram_data.signal_start_offsets[signal_idx] != diagram_data.signal_start_offsets[signal_idx + 1]) {
			size_t sample = diagram_data.signal_start_offsets[signal_idx];
			draw_list->PathClear();
			draw_list->PathLineTo({diagram_x(sim->current_tick), signal_y + (diagram_data.samples_value[sample] * TRUE_OFFSET)});

			for (; sample < diagram_data.signal_start_offsets[signal_idx+1]; ++sample) {
				float signal_x = diagram_x(diagram_data.samples_time[sample]);
				draw_list->PathLineTo({signal_x, signal_y + (diagram_data.samples_value[sample] * TRUE_OFFSET)});
				draw_list->PathLineTo({signal_x, signal_y + (!diagram_data.samples_value[sample] * TRUE_OFFSET)});
			}

			draw_list->PathStroke(COLOR_SIGNAL, 0, 2.0f);
		}

		// signal-name
		const char *label = signal_names[signal_idx].c_str();
		const ImVec2 label_pos = {origin.x + BORDER_WIDTH, signal_y - 20};
		const float label_padding = 2.0f;
		auto text_size = ImGui::CalcTextSize(label);
		const bool is_hovered = (int) signal_pos == selected_signal;

		draw_list->AddRectFilled({label_pos.x - label_padding, label_pos.y - label_padding},
								 {label_pos.x + text_size.x + label_padding, label_pos.y + text_size.y + label_padding},
								 (is_hovered) ? COLOR_HOVERED : COLOR_LABEL,
								 4.0f);
		draw_list->AddText(label_pos, COLOR_TEXT, label);
	}

    void draw_bus(const std::string& bus_name, const std::vector<int>& bus_indices, size_t signal_pos) {
    
        auto draw_list = ImGui::GetWindowDrawList();
        auto origin = ImGui::GetCursorScreenPos();
        auto region = ImGui::GetContentRegionAvail();
        auto sim = ui_context->device->simulator;
    
        float signal_y = origin.y + ((float) signal_pos * 40.0f) + 30.0f;
    
        // Alternating background
        draw_list->AddRectFilled({origin.x, signal_y - 30.0f}, {origin.x + region.x, signal_y + 10.0f}, COLOR_BACKGROUND[signal_pos % 2]);
    
        // Convert time into x coordinate
        auto diagram_x = [&](int64_t t) -> float {
            return origin.x + BORDER_WIDTH + (float)(t - diagram_data.time_begin) / time_scale;
        };
    
        // Obtain all changes
        std::vector<int64_t> change_times;
        for (int signal_index : bus_indices) {
            size_t sample_start = diagram_data.signal_start_offsets[signal_index];
            size_t sample_end = diagram_data.signal_start_offsets[signal_index + 1];
    
            for (size_t sample = sample_start; sample < sample_end; ++sample) {
                change_times.push_back(diagram_data.samples_time[sample]);
            }
        }

        std::sort(change_times.begin(), change_times.end());
        change_times.erase(std::unique(change_times.begin(), change_times.end()), change_times.end());

		// Start at left to plot the line
		float plot_x_start = origin.x + BORDER_WIDTH;

        // Plot for each change
        for (int64_t time : change_times) {
            float signal_x = diagram_x(time);
            int bus_value = 0; 
    
            // For each bit, find current value at current time
            for (size_t bit = 0; bit < bus_indices.size(); ++bit) {
                int signal_index = bus_indices[bit];
                if (signal_index != -1) {
                    size_t sample_start = diagram_data.signal_start_offsets[signal_index];
                    size_t sample_end = diagram_data.signal_start_offsets[signal_index + 1];
    
                    int last_valid_value = 0; 
    
                    for (size_t sample = sample_end - 1 ; sample >= sample_start; --sample) {  
    					if (diagram_data.samples_time[sample] > time) break;
                        last_valid_value = static_cast<int>(diagram_data.samples_value[sample]);
                    }
    
                    bus_value |= (last_valid_value << bit);
                }
            }
    
            // Convert to HEX
            char hex_value[5]; 
            if (bus_indices.size() == 16) {
                snprintf(hex_value, sizeof(hex_value), "%04X", bus_value);  
            } else {
                snprintf(hex_value, sizeof(hex_value), "%02X", bus_value); 
            }

			// Value label
            ImVec2 text_pos = {signal_x+10.0f, signal_y - 15.0f};
            draw_list->AddText(text_pos, COLOR_WHITE, hex_value);
    
            // Plot line
            draw_list->AddLine({plot_x_start, signal_y + TRUE_OFFSET}, {signal_x-2, signal_y  + TRUE_OFFSET}, COLOR_SIGNAL, 2.0f);
            draw_list->AddLine({plot_x_start, signal_y },              {signal_x-2, signal_y }, COLOR_SIGNAL, 2.0f);
			draw_list->AddLine({signal_x-2, signal_y  + TRUE_OFFSET} , {signal_x+2, signal_y }, COLOR_SIGNAL, 2.0f);
			draw_list->AddLine({signal_x-2, signal_y  } , {signal_x+2, signal_y + TRUE_OFFSET}, COLOR_SIGNAL, 2.0f);
			plot_x_start = signal_x+2;
        }


		// Plot from last time until current time
		float signal_x = diagram_x(sim->current_tick);
        draw_list->AddLine({plot_x_start, signal_y + TRUE_OFFSET}, {signal_x, signal_y  + TRUE_OFFSET}, COLOR_SIGNAL, 2.0f);
        draw_list->AddLine({plot_x_start, signal_y },              {signal_x, signal_y }, COLOR_SIGNAL, 2.0f);

		// signal-name
		const char *label = bus_name.c_str();
		const ImVec2 label_pos = {origin.x + BORDER_WIDTH, signal_y - 20};
		const float label_padding = 2.0f;
		auto text_size = ImGui::CalcTextSize(label);
		const bool is_hovered = (int) signal_pos == selected_signal;

		draw_list->AddRectFilled({label_pos.x - label_padding, label_pos.y - label_padding},
								 {label_pos.x + text_size.x + label_padding, label_pos.y + text_size.y + label_padding},
								 (is_hovered) ? COLOR_HOVERED : COLOR_LABEL,
								 4.0f);
		draw_list->AddText(label_pos, COLOR_TEXT, label);
    }
    
	void draw_vertical_guide() {
		// Draws a vertical guide on the logic Analyzer
		// Selected tick is written on the bottom of the screen
		
	    auto draw_list = ImGui::GetWindowDrawList();
	    auto origin = ImGui::GetCursorScreenPos();
	    auto region = ImGui::GetContentRegionAvail();
	    auto mouse = ImGui::GetMousePos();

	    // Dibujar la línea vertical en la posición del mouse
	    draw_list->AddLine({mouse.x, origin.y}, {mouse.x, origin.y + region.y}, COLOR_GUIDE);

	    // Convertir mouse.x a un valor de tiempo
	    int64_t time_at_cursor = (int64_t)((mouse.x - origin.x - BORDER_WIDTH) * time_scale + (float)diagram_data.time_begin);

	    // Convertir el valor de tiempo a string
	    char position_text[32];
	    snprintf(position_text, sizeof(position_text), "%" PRId64, time_at_cursor);

	    // Obtener el tamaño de la ventana para colocar el texto en la parte inferior visible
	    ImVec2 window_pos = ImGui::GetWindowPos();   // Posición de la ventana
	    ImVec2 window_size = ImGui::GetWindowSize(); // Tamaño de la ventana

	    // Posicionar el texto en la parte inferior visible de la ventana
	    ImVec2 text_pos = {mouse.x + 5, window_pos.y + window_size.y - 20}; 

	    // Dibujar el texto con la posición del tiempo usando COLOR_WHITE
	    draw_list->AddText(text_pos, COLOR_WHITE, position_text);
	}

	void handle_mouse_scroll() {
	    // Obtener la posición del mouse
	    ImVec2 mouse_pos = ImGui::GetMousePos();
	    ImVec2 window_pos = ImGui::GetWindowPos();
	    ImVec2 window_size = ImGui::GetWindowSize();

	    // Verificar si el mouse está dentro del panel
	    if (mouse_pos.x >= window_pos.x && mouse_pos.x <= window_pos.x + window_size.x &&
	        mouse_pos.y >= window_pos.y && mouse_pos.y <= window_pos.y + window_size.y) {
			
	        // Obtener la dirección del scroll
	        float scroll_delta = ImGui::GetIO().MouseWheel;

	        if (scroll_delta != 0.0f) {
	            // Ajustar `time_base` en pasos de 0.1
	            time_base += scroll_delta * 0.1f;

	            // Limitar `time_base` al rango permitido (1.0 - 32.0)
	            time_base = ImClamp(time_base, 1.0f, 32.0f);

	            // Recalcular `time_scale`
	            time_scale = (1000000.0f / time_base) / (20.0f * (float) ui_context->device->simulator->tick_duration_ps);
	        }
	    }
	}

	void context_menu() {
		if (!ImGui::IsPopupOpen("signal_context")) {
			auto origin = ImGui::GetCursorScreenPos();
			auto mouse = ImGui::GetMousePos();
			selected_signal = (int) ((mouse.y - origin.y) / 40.0f);
			if (selected_signal < 0 || (size_t) selected_signal >= signal_names.size()) {
				selected_signal = -1;
			}
		}

		if (selected_signal < 0) {
			return;
		}

		if (ImGui::BeginPopupContextWindow("signal_context")) {
			// remove signal
			if (ImGui::MenuItem("Remove")) {
				remove_signal(selected_signal);
				selected_signal = -1;
			}

			// breakpoint
			const char *bp_text = dms_breakpoint_signal_is_set(ui_context->dms_ctx, diagram_data.signals[selected_signal]) ?
									"Disable breakpoint" : "Enable breakpoint";
			if (ImGui::MenuItem(bp_text)) {
				dms_toggle_signal_breakpoint(ui_context->dms_ctx, diagram_data.signals[selected_signal]);
			}
			ImGui::EndPopup();
		}
	}

private:
	bool valid_input_signal() {
		if (enable_gtkwave) {
			return false;
		}
		auto signal = signal_by_name(ui_context->device->simulator->signal_pool, input);
		return signal_is_undefined(signal);
	}

	void add_input_signal() {
		auto signal = signal_by_name(ui_context->device->simulator->signal_pool, input);

		if (!signal_is_undefined(signal)) {
			signal_names.push_back(input);
			arrpush(diagram_data.signals, signal);

			input[0] = '\0';
			ImGui::SetItemDefaultFocus();
		}
	}

	void add_profile_signals() {
		Simulator *sim = ui_context->device->simulator;
		Signal *signals = signal_history_profile_signals(sim->signal_history, (uint32_t) current_profile);
		const char **aliases = signal_history_profile_signal_aliases(sim->signal_history, (uint32_t) current_profile);

		for (size_t i = 0; i < arrlenu(signals); ++i) {
			auto sig_name = signal_get_name(sim->signal_pool, signals[i]);
			if (aliases[i] && strcmp(sig_name, aliases[i])) {
				std::string full_name = aliases[i];
				full_name += " (";
				full_name += sig_name;
				full_name += ")";
				signal_names.push_back(full_name);
			} else {
				signal_names.push_back(sig_name);
			}

			arrpush(diagram_data.signals, signals[i]);
		}
	}

	void remove_signal(ptrdiff_t index) {
		arrdel(diagram_data.signals, (size_t) index);
		signal_names.erase(signal_names.begin() + index);
	}

private:
	ImVec2			position;
	const ImVec2	size = {815, 400};
	std::string		title;

	bool						enable_history = false;
	bool						enable_gtkwave = false;
	int							current_profile = 0;

	float						time_base = 1;
	float						time_scale = 0;

	int							selected_signal = -1;

	std::vector<std::string>	signal_names;
	SignalHistoryDiagramData	diagram_data = {};
	char						input[256];
};

Panel::uptr_t panel_logic_analyzer_create(UIContext *ctx, struct ImVec2 pos) {
	return std::make_unique<PanelLogicAnalyzer>(ctx, pos);
}
