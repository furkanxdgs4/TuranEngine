#include "GFX_Core.h"
#include "TuranAPI/API_Logger.h"

#include "Renderer/GFX_APICommands.h"
#include "GFX_FileSystem.h"

namespace GFX {
	vector<WINDOW*> GFX_Core::ONSCREEN_Windows = vector<WINDOW*>();
	vector<MONITOR*> GFX_Core::CONNECTED_Monitors = vector<MONITOR*>();

	GFX_Core* GFX_Core::SELF = nullptr;
	Renderer* GFX_Core::RENDERER = nullptr;
	unsigned int GFX_Core::FOCUSED_WINDOW_index = 0;
	string GFX_Core::GPU_Vendor, GFX_Core::GPU_Model, GFX_Core::GL_Version;


	GFX_Core::GFX_Core() {
		TuranAPI::LOG_STATUS("GFX Core systems are starting!");

		GFX_FileSystem::SELF = new GFX_FileSystem;



		TuranAPI::LOG_STATUS("GFX Core systems are started!");
	}




	//WINDOW OPERATIONs

	vector<const WINDOW*> GFX_Core::Get_Window_List() {
		vector<const WINDOW*> const_window_list;
		for (WINDOW* window : ONSCREEN_Windows) {
			const WINDOW* const_window = window;
			const_window_list.push_back(const_window);
		}
		return const_window_list;
	}

	WINDOW* GFX_Core::Get_Window_byID(void* id) {
		for (WINDOW* window : ONSCREEN_Windows) {
			if (window->Get_Window_ConstGPUContext() == id) {
				return window;
			}
		}
		TuranAPI::LOG_ERROR("Intended window isn't be found!");
		return nullptr;
	}

	MONITOR* GFX_Core::Create_MonitorOBJ(void* monitor, const string& name) { return new MONITOR(monitor, name); TuranAPI::LOG_STATUS("Monitor: " + name + " is created!"); }
	WINDOW* GFX_Core::Create_WindowOBJ(unsigned int width, unsigned int height, GFX_ENUM display_mode, MONITOR* display_monitor, unsigned int refresh_rate, const string& window_name, GFX_ENUM v_sync) {
		return new WINDOW(width, height, display_mode, display_monitor, refresh_rate, window_name, v_sync); TuranAPI::LOG_STATUS("Window: " + window_name + " is created!");
	}
	void* GFX_Core::Get_Window_EditableGPUContext(WINDOW* window) { return window->GPU_CONTEXT; }


	//RENDER OPERATIONs

	//Start to record new frame's render calls, handle current resources and rendergraphs for new frame!
	void GFX_Core::New_Frame() {
		for (WINDOW* window : ONSCREEN_Windows) {
			APICommander::SELF->Clear_WindowBuffer_Immediately(window, vec3(1));
		}

		RENDERER->New_Frame();
	}
	//Render the recorded render calls!
	void GFX_Core::Render() { TuranAPI::LOG_NOTCODED("GFX_API::Render() isn't coded!", false); }
	//Swap buffers of all windows to see latest changes of window contents!
	void GFX_Core::Show_ThisFrame_onWindows() {
		for (WINDOW* window : ONSCREEN_Windows) {
			APICommander::SELF->Swap_WindowBuffers_Immediately(window);
		}
	
	
	}
}