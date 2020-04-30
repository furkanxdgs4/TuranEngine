#include "Profiling_Window.h"
#include "GFXSource/GFX_Core.h"
using namespace TuranAPI;

namespace TuranEditor {

	Profiling_Window::Profiling_Window() : IMGUI_WINDOW("Profiler"){
		IMGUI_REGISTERWINDOW(this);
	}

	void Profiling_Window::Run_Window() {
		if (!Is_Window_Open) {
			IMGUI_DELETEWINDOW(this);
			return;
		}
		if (!IMGUI->Create_Window(Window_Name, Is_Window_Open, false)) {
			IMGUI->End_Window();
			return;
		}
		IMGUI->Input_Text("Profiling Data File Path", &PATH);
		if (IMGUI->Button("Load the file")) {
			Profiled_Session = TuranAPI::Load_a_Profiling_Resource(PATH);
		}

		if (Profiled_Session != nullptr) {
			std::cout << "Profiled session is created!\n";
			std::cout << "Profiled scope number: " << Profiled_Session->PROFILED_SCOPEs_VECTOR->size() << std::endl;
			if (IMGUI->Begin_Tree(PATH)) {
				for (unsigned int i = 0; i < Profiled_Session->PROFILED_SCOPEs_VECTOR->size(); i++) {
					Profiled_Scope* Scope = &(*Profiled_Session->PROFILED_SCOPEs_VECTOR)[i];
					if (IMGUI->Begin_Tree(Scope->NAME)) {
						IMGUI->Text("Start Point: number->string isn't supported yet!");
						IMGUI->Text("End Point: number->string isn't supported yet!");
						IMGUI->Text("Duration: number->string isn't supported yet!");
						if (Scope->DURATION < 1000) {
							IMGUI->Text("Duration: number->string isn't supported yet!");
						}
						else {
							IMGUI->Text("Duration: number->string isn't supported yet!");
						}
						IMGUI->Text("Thread ID: number->string isn't supported yet!");
						IMGUI->End_Tree();
					}
				}
				IMGUI->End_Tree();
			}
		}

		IMGUI->End_Window();
	}
}