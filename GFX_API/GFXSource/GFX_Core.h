#pragma once
#include "GFX_Includes.h"
#include "TuranAPI/FileSystem/FileSystem_Core.h"

#include "GFX_Display.h"
#include "Renderer/GFX_Renderer.h"

namespace GFX {
	class GFXAPI GFX_Core {
	protected:
		static unsigned int FOCUSED_WINDOW_index;
		static string GPU_Vendor; static string GPU_Model; static string GL_Version;

		virtual void Initialization() = 0;
		virtual void Check_Computer_Specs() = 0;
		virtual void Save_Monitors() = 0;
		virtual void Create_Renderer() = 0;
		GFX_Core();

		static MONITOR* Create_MonitorOBJ(void* monitor, const string& name);
		static WINDOW* Create_WindowOBJ(unsigned int width, unsigned int height, GFX_ENUM display_mode, MONITOR* display_monitor, unsigned int refresh_rate, const string& window_name, GFX_ENUM v_sync);
	public:
		static GFX_Core* SELF;
		static Renderer* RENDERER;

		//There will be only one GFX_API instance, so making vectors static allows us to use them in callbacks!
		static vector<WINDOW*> ONSCREEN_Windows;
		static vector<MONITOR*> CONNECTED_Monitors;

		//Window Operations
		virtual void Create_Window(string name) = 0;
		virtual void Close_Window(WINDOW* window) = 0;
		virtual void Change_Window_Resolution(WINDOW* window, unsigned int width, unsigned int height) = 0;
		virtual void Set_Window_Focus(WINDOW* window, bool is_focused) = 0;
		virtual void Set_Window_Callbacks() = 0;
		static vector<const WINDOW*> Get_Window_List();
		static WINDOW* Get_Window_byID(void* id);
		virtual void Bind_Window_Context(WINDOW* window) = 0;
		static void* Get_Window_EditableGPUContext(WINDOW* window);

		//Renderer Operations

		//Start to record new frame's render calls, handle current resources and rendergraphs for new frame!
		static void New_Frame();
		//Render the recorded render calls!
		static void Render();
		//Swap buffers of all windows to see latest changes of window contents!
		static void Show_ThisFrame_onWindows();
		virtual void Render_IMGUI() = 0;
		virtual void Check_GL_Errors(const string& status) = 0;

		//Load default GFX_API needed contents (Like PostProccess material type)
		virtual void Load_GFX_Files() = 0;

		//Input (Keyboard-Controller) Operations
		virtual void Take_Inputs() = 0;
		virtual void Destroy_GFX_Resources() = 0;
	};

}