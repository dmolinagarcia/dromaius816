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

	// Emscripten requires full control of the main loop. Store GLFW book-keeping variables globally.
		GLFWwindow *g_window = nullptr;

    // GLFW Callback
    static void glfw_error_callback(int error,
        const char * description) {
        fprintf(stderr, "Glfw Error %d: %s\n", error, description);
    }

    // ARGument Handler
    // Definimos primero la lista de argumentos
    using argh_list_t = std::initializer_list < const char * const > ;
    static constexpr argh_list_t ARG_MACHINE = {"-m", "--machine" };
    static constexpr argh_list_t ARG_HELP = {"-h", "--help" };

    // Y una función para mostrar la ayuda si no se proporcionan 
    // los adecuados
    void print_help() {
        auto format_argh_list = [](const argh_list_t & args) -> auto {
            std::string result;
            const char * sepa = "";

            for (const auto & a: args) {
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

///////////////////////////////////////////////////////////////////////////////
//
// interface
//

void main_loop(void *arg);

int main(int argc, char ** argv) {

	double width = 1200;
	double height = 800;

    // parse command-line arguments
    argh::parser cmd_line;
    cmd_line.add_params(ARG_MACHINE);
    cmd_line.add_params(ARG_HELP);
    cmd_line.parse(argc, argv);

    if (cmd_line[ARG_HELP]) {
        print_help();
        exit(EXIT_FAILURE);
    }

    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

	// Setup OpenGL version
	    // Decide GL+GLSL versions
	#if __APPLE__
	    // GL 3.2 + GLSL 150
	    const char* glsl_version = "#version 150";
	    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
	#elif __EMSCRIPTEN__
	    const char* glsl_version = "#version 100";
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
		glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
		emscripten_get_element_css_size("canvas", &width, &height);
	#else
	    // GL 3.0 + GLSL 130
	    const char* glsl_version = "#version 130";
	    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
	#endif		

    // Create window with graphics context
    g_window = glfwCreateWindow(static_cast<int>(width), static_cast<int>(height), "Dromaius", NULL, NULL);
    if (g_window == NULL)
        return 1;
    glfwMakeContextCurrent(g_window);	

	// Initialize OpenGL loader
		bool err = gladLoadGL() == 0;
	    if (err)
	    {
	        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
	        return 1;
	    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
	// io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;			// Enable Docking

	// Main loop
	while (!glfwWindowShouldClose(g_window)) {
		main_loop(nullptr);
	}

    glfwDestroyWindow(g_window);
    glfwTerminate();

    exit(EXIT_SUCCESS);
}

	void main_loop([[maybe_unused]] void *arg) {

		// Poll and handle events (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		glfwPollEvents();
		glfwSwapBuffers(g_window);
	}

