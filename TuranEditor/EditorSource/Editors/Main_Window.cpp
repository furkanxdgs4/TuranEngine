#include "Main_Window.h"

#include "Content_Browser.h"
#include "IMGUI_DEFAULT_WINDOWs.h"
#include "Scene_Editor.h"
#include "Profiling_Window.h"
#include "GFXSource/GFX_Core.h"
#include "EditorSource/RenderContext/Game_RenderGraph.h"
#include "GFXSource/GFX_Core.h"

#include <string>

using namespace TuranAPI;

namespace TuranEditor {

	Main_Window::Main_Window(Game_RenderGraph* game_rendergraph) : IMGUI_WINDOW("Main_Window"), GameRenderGraph(game_rendergraph) {
		IMGUI_REGISTERWINDOW(this);
	}

	//Create main menubar for the Editor's main window!
	void Main_Menubar_of_Editor();

	void Main_Window::Run_Window() {
		if (!Is_Window_Open) {
			IMGUI->Is_IMGUI_Open = false;
			IMGUI_DELETEWINDOW(this);
			return;
		}
		if (!IMGUI->Create_Window("Main Window", Is_Window_Open, true)) {
			IMGUI->End_Window();
			return;
		}
		//Successfully created the window!
		Main_Menubar_of_Editor();

		IMGUI->Text("Main Window is not coded! If you want to see a Scene, you should go to Scene Editor");
		String Allocated_Size = "Total Allocated: ";
		Allocated_Size.append(std::to_string(LASTUSEDALLOCATOR->Get_AllocatedMemorySize()).c_str());
		IMGUI->Text(Allocated_Size);
		String Used_Size = "Total In-Use: ";
		Used_Size.append(std::to_string(LASTUSEDALLOCATOR->Get_NotFreeMemorySize()).c_str());
		IMGUI->Text(Used_Size);


		IMGUI->End_Window();
	}
	
	void Main_Menubar_of_Editor() {
		if (!IMGUI->Begin_Menubar()) {
			IMGUI->End_Menubar();
		}
		//Successfully created the main menu bar!
		
		if (IMGUI->Begin_Menu("View")) {
			if (IMGUI->Menu_Item("Game Content Browser")) { new GameContent_Browser; }
			if (IMGUI->Menu_Item("Scene Editor")) { new Scene_Editor; }
			if (IMGUI->Menu_Item("Profiler")) { new Profiling_Window; }
			if (IMGUI->Menu_Item("Demo Window")) { new IMGUI_DemoWindow; }
			if (IMGUI->Menu_Item("Metrics Window")) { new IMGUI_MetricsWindow; }
			IMGUI->End_Menu();
		}


		//End Main Menubar operations!
		IMGUI->End_Menubar();
	}
}