cmake_minimum_required(VERSION 3.12)

#> project(dromaius) lo cambio por lo de abajo
project(dromaius816)

# options
option (ENABLE_THREADING "Enable multithreading in the Dromaius context" ON)
option (ENABLE_GTKWAVE_EXPORT "Enable dumping of signals to GTKWave" OFF)

## set(CMAKE_BUILD_TYPE Debug)

# force C11 for all targets
#  - don't do this on MSVC anymore. In march 2020 support for a fully compliant C11 preprocessor
#	 was added to MSVC, but the compiler itself isn't fully C11 compliant yet. CMake 3.18 passes the
#	 compiler flag to enable C11 mode which fails to compile code that enables C11 features not yet
#	 supported by MSVC.
if (NOT MSVC)
	set(CMAKE_C_STANDARD 11)
endif()
set(CMAKE_CXX_STANDARD 17)

# export a JSON compilation database for clangd
set (CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# platform detection
string(TOUPPER ${CMAKE_SYSTEM_NAME} PLATFORM_NAME)
string(CONCAT PLATFORM_DEF "PLATFORM_" ${PLATFORM_NAME})

# macro to turn on compiler warnings for a target
macro(target_compile_warning target)
	target_compile_options(${target} PRIVATE
		$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
		-Wall -Wextra -Wconversion -Wsign-conversion -Werror -Wno-error=unused-variable -Wno-error=unused-parameter>
		$<$<CXX_COMPILER_ID:MSVC>:
			/W4
			/wd"4200" /wd"4201" /wd"4204"	# disable specific non-standard extension warnings
			>)
endmacro()

# check system libraries
include(CheckSymbolExists)

# >> some platform require an extra link-libraries for math functions and others do not
check_symbol_exists("floor" "math.h" HAVE_FLOOR)
if (NOT HAVE_FLOOR)
	unset(HAVE_FLOOR CACHE)
	list(APPEND CMAKE_REQUIRED_LIBRARIES m)
	check_symbol_exists("floor" "math.h" HAVE_FLOOR)
	if (NOT HAVE_FLOOR)
		message(FATAL_ERROR "Unable to find math functions")
	else()
		list(APPEND LIBS_MATH m)
		list(REMOVE_ITEM CMAKE_REQUIRED_LIBRARIES m)
		unset(HAVE_FLOOR CACHE)
	endif()
endif()

	# external library - glfw
	if (NOT EMSCRIPTEN)
		set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
		set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
		set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
		set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

		add_subdirectory(libs/glfw)
	endif()

	# external library - OpenGL
	if (NOT EMSCRIPTEN)
		find_package(OpenGL REQUIRED)
	endif()

	# external library Dear Imgui
	set (IMGUI_TARGET imgui)
	add_library(${IMGUI_TARGET} STATIC)
	target_sources(${IMGUI_TARGET}
		PRIVATE
			libs/glad/glad.c
			libs/glad/glad.h
			libs/glad/KHR/khrplatform.h
			libs/imgui/imconfig.h
			libs/imgui/imgui.cpp
			libs/imgui/imgui.h
			libs/imgui/imgui_demo.cpp
			libs/imgui/imgui_draw.cpp
			libs/imgui/imgui_internal.h
			libs/imgui/imgui_tables.cpp
			libs/imgui/imgui_widgets.cpp
			libs/imgui/imstb_rectpack.h
			libs/imgui/imstb_textedit.h
			libs/imgui/imstb_truetype.h
			libs/imgui/backends/imgui_impl_glfw.cpp
			libs/imgui/backends/imgui_impl_glfw.h
			libs/imgui/backends/imgui_impl_opengl3.cpp
			libs/imgui/backends/imgui_impl_opengl3.h
			libs/imgui/misc/cpp/imgui_stdlib.cpp
			libs/imgui/misc/cpp/imgui_stdlib.h
	)
	target_include_directories(${IMGUI_TARGET} SYSTEM PUBLIC libs libs/imgui libs/glad)
	target_include_directories(${IMGUI_TARGET} PRIVATE ${OPENGL_INCLUDE_DIR})
	target_link_libraries(${IMGUI_TARGET} PUBLIC glfw)
	target_link_libraries(${IMGUI_TARGET} PRIVATE ${OPENGL_LIBRARIES})

	if (ENABLE_GTKWAVE_EXPORT)
		# external library - Zlib
		find_package(ZLIB REQUIRED)

		# external library - bzip2
		find_package(BZip2 REQUIRED)

		target_sources(${SFL_TARGET} PRIVATE
			libs/gtkwave/lxt_write.c
			libs/gtkwave/lxt_write.h
		)
	endif()

	# external library - single-file libraries
	set (SFL_TARGET stb)
	add_library(${SFL_TARGET} STATIC)
	target_sources(${SFL_TARGET}
		PRIVATE
			libs/cute/cute_files.c
			libs/cute/cute_files.h
			libs/stb/stb_ds.h
			libs/stb/stb_impl.c
	)

	# simulator library
	set (LIB_TARGET dromaius)

	add_library(${LIB_TARGET} STATIC)
	target_sources(${LIB_TARGET}
		PRIVATE
			src/sys/atomics.h
			src/sys/threads.c
			src/sys/threads.h
			src/chip.h
			src/chip_oscillator.c
			src/chip_oscillator.h		
			src/chip_poweronreset.c
			src/chip_poweronreset.h
			src/crt.h
			src/context.c
			src/context.h
			src/cpu.h
			src/cpu_6502.c
			src/cpu_6502.h
			src/cpu_65816.c
			src/cpu_65816.h
			src/device.c
			src/device.h
			src/dev_commodore_pet.c
			src/dev_commodore_pet.h
			src/dev_minimal_6502.c
			src/dev_minimal_6502.h
			src/dev_minimal_65816.c
			src/dev_minimal_65816.h
			src/signal_history.c
			src/signal_history.h
			src/signal_history_profiles.c
			src/signal_history_profiles.h
			src/signal_line.c
			src/signal_line.h
			src/signal_pool.c
			src/signal_pool.h
			src/signal_types.h		
			src/simulator.c
			src/simulator.h
			src/stopwatch.c
			src/stopwatch.h
			src/utils.c
			src/utils.h
	)

	if (ENABLE_GTKWAVE_EXPORT)
		message(STATUS "Enabling signal tracing with GTKWave")
		target_link_libraries(${LIB_TARGET} PRIVATE ZLIB::ZLIB BZip2::BZip2)
		target_compile_definitions(${LIB_TARGET} PUBLIC DMS_GTKWAVE_EXPORT)
	endif()

	target_include_directories(${LIB_TARGET} PRIVATE libs)
	target_link_libraries(${LIB_TARGET} PRIVATE ${SFL_TARGET})
	target_link_libraries(${LIB_TARGET} PRIVATE ${LIBS_MATH})
	target_compile_definitions(${LIB_TARGET} PUBLIC ${PLATFORM_DEF})
	if (NOT ENABLE_THREADING)
		target_compile_definitions(${LIB_TARGET} PUBLIC DMS_NO_THREADING)
	endif()
	target_compile_warning(${LIB_TARGET})

# gui
set (GUI_TARGET dromaius816_gui)

add_executable(${GUI_TARGET} WIN32)
target_sources(${GUI_TARGET}
	PRIVATE
	    src/gui/debug.h
		src/gui/config.cpp
		src/gui/config.h
		src/gui/imgui_ex.cpp
		src/gui/imgui_ex.h
		src/gui/main.cpp
		src/gui/panel.h
		src/gui/panel_control.cpp
		src/gui/panel_control.h
		src/gui/panel_cpu_6502.cpp
		src/gui/panel_cpu_6502.h
		src/gui/panel_cpu_65816.cpp
		src/gui/panel_cpu_65816.h
		src/gui/panel_dev_commodore_pet.cpp
		src/gui/panel_dev_commodore_pet.h
		src/gui/panel_dev_minimal_6502.cpp
		src/gui/panel_dev_minimal_6502.h
		src/gui/panel_dev_minimal_65816.cpp
		src/gui/panel_dev_minimal_65816.h
		src/gui/popup_file_selector.cpp
		src/gui/popup_file_selector.h
		src/gui/std_helper.h	
		src/gui/ui_context.cpp
		src/gui/ui_context.h
		src/gui/widgets.cpp
		src/gui/widgets.h
)

target_include_directories(${GUI_TARGET} PRIVATE libs src)
target_link_libraries(${GUI_TARGET} PRIVATE ${IMGUI_TARGET})
target_link_libraries(${GUI_TARGET} PRIVATE ${LIB_TARGET})
target_compile_warning(${GUI_TARGET})

if (WIN32)
	target_link_options(${GUI_TARGET} PRIVATE "/ENTRY:mainCRTStartup")
endif()



