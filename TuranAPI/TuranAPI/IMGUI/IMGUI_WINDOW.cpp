#include "IMGUI_WINDOW.h"
using namespace TuranAPI::IMGUI;


vector<IMGUI_WINDOW*> IMGUI_WINDOW::ALL_IMGUI_WINDOWs = vector<IMGUI_WINDOW*>{};

IMGUI_WINDOW::IMGUI_WINDOW(string name) : Window_Name(name) {
	for (IMGUI_WINDOW* WINDOW : ALL_IMGUI_WINDOWs) {
		if (WINDOW->Window_Name == Window_Name) {
			Window_Name = Window_Name + "  ";
		}
	}
	ALL_IMGUI_WINDOWs.push_back(this);

	cout << "Editor Window created: " << name << endl;
}

IMGUI_WINDOW::~IMGUI_WINDOW() {
	cout << "Editor Window is closed: " << Window_Name << endl;
	unsigned int window_vector_index = 0;
	for (window_vector_index = 0; window_vector_index < ALL_IMGUI_WINDOWs.size(); window_vector_index++) {
		IMGUI_WINDOW* WINDOW = ALL_IMGUI_WINDOWs[window_vector_index];
		if (WINDOW == this) {
			break;
		}
	}
	ALL_IMGUI_WINDOWs.erase(ALL_IMGUI_WINDOWs.begin() + window_vector_index);
}

void IMGUI_WINDOW::Run_IMGUI_WINDOWs() {
	for (IMGUI_WINDOW* WINDOW : ALL_IMGUI_WINDOWs) {
		WINDOW->Run_Window();
	}
}

bool IMGUI_WINDOW::Get_Is_Window_Open() {
	return Is_Window_Open;
}