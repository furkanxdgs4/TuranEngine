#include "GFX_Display.h"

namespace GFX {
	void MONITOR::Set_Physical_Size(unsigned int physical_width, unsigned int physical_height) {
		PHYSICAL_WIDTH = physical_width;
		PHYSICAL_HEIGHT = physical_height;
	}

	MONITOR::MONITOR(void* monitor, string name) : ID(monitor), NAME(name) {

	}

	string MONITOR::Get_Monitor_Name() {
		return NAME;
	}

	//Return WIDTH (x), HEIGHT (y) and REFRESH_RATE (<) as one vec3
	vec3 MONITOR::Get_Monitor_VidMode() {
		return vec3(WIDTH, HEIGHT, REFRESH_RATE);
	}

	void MONITOR::Set_Monitor_VidMode(unsigned int width, unsigned int height, unsigned int color_bites, unsigned int refrest_rate) {
		WIDTH = width;
		HEIGHT = height;
		COLOR_BITES = color_bites;
		REFRESH_RATE = refrest_rate;
		TuranAPI::LOG_STATUS("Monitor's Video Mode is set!");
	}


	WINDOW::WINDOW(unsigned int width, unsigned int height, GFX_ENUM display_mode, MONITOR* display_monitor, unsigned int refresh_rate, const string& window_name, GFX_ENUM v_sync)
		: WIDTH(width), HEIGHT(height), DISPLAY_MODE(display_mode), REFRESH_RATE(refresh_rate), DISPLAY_MONITOR(display_monitor), WINDOW_NAME(window_name), VSYNC_MODE(v_sync)
	{}


	string WINDOW::Get_Window_Name() const {
		return WINDOW_NAME;
	}

	//Return WIDTH (x), HEIGHT (y) and REFRESH_RATE (z) as one vec3
	vec3 WINDOW::Get_Window_Mode() {
		return vec3(WIDTH, HEIGHT, REFRESH_RATE);
	}

	//	1) Change the window's display mode (Fullscreen, Windowed, Borderless Window etc.)
	//	2) Or change to intended refresh rate or monitor
	//	3) No resolution changing in this function, window's active resolution is used
	void WINDOW::Change_DisplayMode(GFX_ENUM display_mode, const MONITOR* display_monitor, unsigned int refresh_rate) {}

	void WINDOW::Set_Focus(bool focus_active) {
		cout << "Error: Set focus isn't coded, don't use it!\n";
	}

	//Settings of window
	void WINDOW::Window_Settings(GFX_ENUM vsync_type, bool window_resizable) {
		TuranAPI::LOG_NOTCODED("WINDOW::Window_Settings isn't coded!", false);
	}

	void WINDOW::Set_Window_GPUContext(void* context_ptr) {
		GPU_CONTEXT = context_ptr;
		TuranAPI::LOG_STATUS("Window: " + WINDOW_NAME + "'s GPU Context is set!");
	}

	const void* WINDOW::Get_Window_ConstGPUContext() const {
		return GPU_CONTEXT;
	}

	void* WINDOW::Get_Window_GPUContext() {
		return GPU_CONTEXT;
	}

	void WINDOW::Change_Width_Height(unsigned int width, unsigned int height) {
		WIDTH = width;
		HEIGHT = height;
		TuranAPI::LOG_STATUS("Window: " + WINDOW_NAME + "'s Width and Height is changed!");
	}
}