#include "Profiling_Window.h"
#include "TuranAPI/API_IMGUI.h"
#include "TuranAPI/API_Profiler.h"
using namespace TuranAPI::IMGUI;
using namespace TuranAPI::Profiler;

Profiling_Window::Profiling_Window() : IMGUI_WINDOW("Profiler") {

}

void Profiling_Window::Run_Window() {
	if (!Is_Window_Open) {
		delete this;
		return;
	}
	if (!IMGUI::Create_Window(Window_Name, Is_Window_Open, false)) {
		IMGUI::End_Window();
		return;
	}
	IMGUI::Input_Text("Profiling Data File Path", &PATH);
	if (IMGUI::Button("Load the file")) {
		Profiled_Session = TuranAPI::Profiler::Load_a_Profiling_Resource(PATH);
	}

	if (Profiled_Session != nullptr) {
		cout << "Profiled session is created!\n";
		cout << "Profiled scope number: " << Profiled_Session->PROFILED_SCOPEs_ARRAY.size() << endl;
		if (IMGUI::Begin_Tree(PATH)) {
			for (unsigned int i = 0; i < Profiled_Session->PROFILED_SCOPEs_ARRAY.size(); i++) {
				Profiled_Scope* Scope = &Profiled_Session->PROFILED_SCOPEs_ARRAY[i];
				if (IMGUI::Begin_Tree(Scope->NAME)) {
					IMGUI::Text("Start Point: " + to_string(Scope->START_POINT));
					IMGUI::Text("End Point: " + to_string(Scope->END_POINT));
					IMGUI::Text("Duration: " + to_string(Scope->DURATION) + "");
					if (Scope->DURATION < 1000) {
						IMGUI::Text("Duration: " + to_string(Scope->DURATION) + " milliseconds");
					}
					else {
						IMGUI::Text("Duration: " + to_string((Scope->DURATION) / 1000) + " seconds");
					}
					IMGUI::Text("Thread ID: " + to_string(Scope->THREAD_ID));
					IMGUI::End_Tree();
				}
			}
			IMGUI::End_Tree();
		}
	}

	IMGUI::End_Window();
}