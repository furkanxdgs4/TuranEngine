#pragma once
#include "GFX_Includes.h"

#include "GFX_ENUMs.h"

namespace GFX {
	//I have no function to do something on monitor, so this is a storage struct
	struct GFXAPI MONITOR {
		friend class GFX_Core;

		void* ID;
		const string NAME;
		unsigned int WIDTH, HEIGHT, COLOR_BITES, REFRESH_RATE;
		int PHYSICAL_WIDTH, PHYSICAL_HEIGHT;	//milimeters
		GFX_ENUM DESKTOP_MODE;

		void Set_Physical_Size(unsigned int physical_width, unsigned int physical_height);
		MONITOR(void* monitor, string name);
	public:

		string Get_Monitor_Name();
		//Return WIDTH (x), HEIGHT (y) and REFRESH_RATE (<) as one vec3
		vec3 Get_Monitor_VidMode();

		void Set_Monitor_VidMode(unsigned int width, unsigned int height, unsigned int color_bites, unsigned int refrest_rate);
	};



	//Note: Window ID created by GFX API specific library stored as void* WINDOW
	//If you want to access it, you want to access it with equal library class
	//Window resolution is independent from render resolution
	//After rendering has done, render resolution
	struct GFXAPI WINDOW {
	private:
		friend class GFX_Core;

		unsigned int WIDTH, HEIGHT, REFRESH_RATE;
		MONITOR* DISPLAY_MONITOR;
		GFX_ENUM DISPLAY_MODE;
		string WINDOW_NAME;
		GFX_ENUM VSYNC_MODE;
		void* GPU_CONTEXT;

		void* Get_Window_GPUContext();

		WINDOW(unsigned int width, unsigned int height, GFX_ENUM display_mode, MONITOR* display_monitor, unsigned int refresh_rate, const string& window_name, GFX_ENUM v_sync);
	public:
		//SETTERs

		//	1) Change the window's display mode (Fullscreen, Windowed, Borderless Window etc.)
		//	2) Or change to intended refresh rate or monitor
		//	3) No resolution changing in this function, window's active resolution is used
		void Change_DisplayMode(GFX_ENUM display_mode, const MONITOR* display_monitor, unsigned int refresh_rate);
		void Set_Focus(bool focus_active);
		void Set_Window_GPUContext(void* context_ptr);
		void Window_Settings(GFX_ENUM vsync_type, bool window_resizable);
		void Change_Width_Height(unsigned int width, unsigned int height);

		//GETTERs

		const void* Get_Window_ConstGPUContext() const;
		string Get_Window_Name() const;

		//Return WIDTH (x), HEIGHT (y) and REFRESH_RATE (<) as one vec3
		vec3 Get_Window_Mode();
	};

}