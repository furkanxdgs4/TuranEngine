#include "Editor.h"
using namespace TuranEditor;
#include <vector>

int main() {
	TuranEditor::TE_DependentSystems DependentSystem_Creation;		//This class will hold up pointers to all of the system used in Turan Editor.
	TuranEditor::EditorSystems EditorSYSTEMs;


	Game_RenderGraph Scene;
	Main_Window* main_window = new Main_Window(&Scene);
	while (!TuranEngine->ShouldEngine_Close()) {
		IMGUI->New_Frame();
		IMGUI_RUNWINDOWS();
		GFX->Swapbuffers_ofMainWindow();
		//IMGUI can use main renderer's contents, so render IMGUI later!
		IMGUI->Render_Frame();

		//Take inputs by GFX API specific library that supports input (For now, just GLFW for OpenGL4) and send it to Engine!
		//In final product, directly OS should be used to take inputs!
		TuranEngine->Take_Inputs();
		TuranAPI::WRITE_LOGs_toFILEs();
	}
	std::cout << "Application is closing in 1 seconds!\n";
	SLEEP_THREAD(1);

	return 1;
}