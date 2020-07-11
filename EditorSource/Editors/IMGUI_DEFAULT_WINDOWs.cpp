#include "IMGUI_DEFAULT_WINDOWs.h"
#include "GFXSource/GFX_Core.h"

using namespace TuranAPI;

namespace TuranEditor {
	IMGUI_DemoWindow::IMGUI_DemoWindow() : IMGUI_WINDOW("Demo Window") {
		IMGUI_REGISTERWINDOW(this);
	}

	void IMGUI_DemoWindow::Run_Window() {
		if (!Is_Window_Open) {
			IMGUI_DELETEWINDOW(this);
			return;
		}
		else {
			Is_Window_Open = IMGUI->Show_DemoWindow();
		}
	}


	IMGUI_MetricsWindow::IMGUI_MetricsWindow() : IMGUI_WINDOW("Metrics Window") {
		IMGUI_REGISTERWINDOW(this);
	}

	void IMGUI_MetricsWindow::Run_Window() {
		if (!Is_Window_Open) {
			IMGUI_DELETEWINDOW(this);
			return;
		}
		else {
			Is_Window_Open = IMGUI->Show_MetricsWindow();
		}
	}
}