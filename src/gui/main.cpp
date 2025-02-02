// main application entry point (native + wasm), based upon
// dear imgui: standalone example application for GLFW + OpenGL 3, using programmable pipeline

// Librerias gráficas para gestion de ventanas

	#include "imgui.h"

	#include <backends/imgui_impl_glfw.h>
	#include <backends/imgui_impl_opengl3.h>


// ARGument Handler
// https://github.com/adishavit/argh/blob/master/argh.h
    #include <argh/argh.h>

// Funciones apoyo C++
	#include <cstdio>
	#include <memory>

// Y mas funciones graficas. No se por que no estan juntas, pero bueno

	#include <glad/glad.h>  // Initialize with gladLoadGL()

	// Include glfw3.h after our OpenGL definitions
	#include <GLFW/glfw3.h>



// Namespace Privado
namespace {

    // ARGument Handler
    // Definimos primero la lista de argumentos
	    using argh_list_t = std::initializer_list<const char *const>;
	    static constexpr argh_list_t ARG_MACHINE = {"-m", "--machine"};
	    static constexpr argh_list_t ARG_HELP = {"-h", "--help"};

    // Y una función para mostrar la ayuda si no se proporcionan 
    // los adecuados
	void print_help() {
		auto format_argh_list = [](const argh_list_t &args) -> auto {
			std::string result;
			const char *sepa = "";

			for (const auto &a : args) {
				result.append(sepa);
				result.append(a);
				sepa = ", ";
			}

			return result;
		};

		printf("Usage:\n\n");
		printf("dromaius816_gui [options]\n\n");
		printf("Options\n");
		printf(" %-25s specify machine to emulate (commodore-pet, commodore-pet-lite, minimal-6502).\n",
				format_argh_list(ARG_MACHINE).c_str());
		printf(" %-25s display this help message and stop execution.\n",
				format_argh_list(ARG_HELP).c_str());
	}

	} // unnamed namespace


int main(int argc, char **argv)
{
	// parse command-line arguments
		argh::parser cmd_line;
		cmd_line.add_params(ARG_MACHINE);
		cmd_line.add_params(ARG_HELP);
		cmd_line.parse(argc, argv);

		if (cmd_line[ARG_HELP]) {
			print_help();
			exit(EXIT_FAILURE);
		}

	exit(EXIT_SUCCESS);
}

