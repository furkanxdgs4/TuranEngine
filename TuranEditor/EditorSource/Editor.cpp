#include "Editor.h"


int main() {
	TuranAPI::LOG_STATUS("Editor is running!");
	TuranEngine::Start_Engine();
	EDITOR_FILESYSTEM->Start_EditorFileSystem();
	EDITOR_FILESYSTEM->Load_FileListContents_fromDisk();

	Main_Window* main_window = new Main_Window;

	//GameEngine.ShouldApplication_Close() doesn't work properly for now!
	while (!TuranEngine::ShouldEngine_Close()) {
		TuranAPI::LOG_STATUS("New Loop!");

		//Notify systems that we are in new Frame!
		TuranAPI::IMGUI::IMGUI::New_Frame();
		TuranEngine::GFX->New_Frame();

		//Editor is just a pack of IMGUI windows, so run them!
		TuranAPI::IMGUI::IMGUI_WINDOW::Run_IMGUI_WINDOWs();

		//In Editor, the resources that are send to Renderer will be rendered! Renderer will gather the data and render.
		TuranEngine::GFX->Render();
		//IMGUI can use main renderer's contents, so render IMGUI later!
		TuranEngine::GFX->Render_IMGUI();

		//Swap buffers for all windows to display latest changes on windows (Geometry Viewports and IMGUIs)!
		TuranEngine::GFX->Show_ThisFrame_onWindows();

		//Take inputs by GFX API specific library that supports input (For now, just GLFW for OpenGL3) and send it to Engine!
		//In final product, directly OS should be used to take inputs!
		TuranEngine::GFX->Take_Inputs();
	}
	TuranAPI::LOG_STATUS("Application exited from the frame loop, because: " + TuranEngine::Why_Engine_Closed());
	TuranAPI::WRITE_LOGs_toFILEs();
	cout << "Application is closing in 1 seconds!\n";
	this_thread::sleep_for(chrono::seconds(1));
	
	return 1;
}