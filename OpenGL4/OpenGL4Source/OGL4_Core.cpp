#include "OGL4_Core.h"
#include "IMGUI/IMGUI_OGL4.h"
#include "OGL4_FileSystem.h"
#include "Renderer/OGL4_APICommands.h"
using namespace TuranAPI::IMGUI;

namespace OpenGL4 {
	OpenGL4_Core::OpenGL4_Core() : GFX::GFX_Core() {
		//Set static GFX_API variable to created OGL3_SYS, because there will only one GFX_API in run-time
		//And we will use this GFX_API_OBJ to give commands to GFX_API in window callbacks
		SELF = this;
		TuranAPI::LOG_STATUS("OpenGL 4 systems are starting!");

		Initialization();
		Save_Monitors();

		//Create renderer class and renderer window
		Create_Renderer();

		//Create main window for Turan Engine
		Create_Window("Turan Engine");

		OpenGL4_FILESYSTEM = new OGL4_FileSystem;
		OpenGL4_FILESYSTEM->Load_FileListContents_fromDisk();

		IMGUI::GFX_IMGUI = new IMGUI_OGL4;
		IMGUI::Create_Context(GFX_Core::Get_Window_EditableGPUContext(ONSCREEN_Windows[0]));
		TuranAPI::LOG_STATUS("OpenGL 4 systems are started!");
	}


	//WINDOW OPERATIONs

	void OpenGL4_Core::Change_Window_Resolution(GFX::WINDOW* window, unsigned int width, unsigned int height) {
		window->Change_Width_Height(width, height);
		glViewport(0, 0, width, height);
	}

	void OpenGL4_Core::Set_Window_Focus(GFX::WINDOW* window, bool is_focused) {
		if (is_focused) {
			for (unsigned int i = 0; i < ONSCREEN_Windows.size(); i++) {
				if (window->Get_Window_ConstGPUContext() == ONSCREEN_Windows[i]->Get_Window_ConstGPUContext()) {
					FOCUSED_WINDOW_index = i;
					return;
				}
			}
			cout << "Error: Intended focus window can't be found! That means there is a bug in the system!\n";
		}
		else {

		}
	}

	void OpenGL4_Core::Close_Window(GFX::WINDOW* window) {
		//First, destroy window and its resources with glfwDestroyWindow
		glfwDestroyWindow((GLFWwindow*)window->Get_Window_ConstGPUContext());

		//Then delete it from global GFX_WINDOW vector
		int window_list_index;
		for (int i = 0; i < ONSCREEN_Windows.size(); i++) {
			if (window == ONSCREEN_Windows[i])
				window_list_index = i;
		}
		ONSCREEN_Windows.erase(ONSCREEN_Windows.begin() + window_list_index);
	}

	void OpenGL4_Core::GFX_Error_Callback(int error_code, const char* description) {
		TuranAPI::LOG_CRASHING("GLFW ERROR CODE: " + to_string(error_code) + "\nDescription: " + description, true);
	}

	void OpenGL4_Core::Initialization() {
		//Set error callback to handle all glfw errors (including initialization error)!
		glfwSetErrorCallback(OpenGL4_Core::GFX_Error_Callback);

		//Initialize GLFW with OpenGL3
		int status = glfwInit();
		if (!status) {
			cout << "GLFW initialization error!\n";
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	}

	void OpenGL4_Core::Check_Computer_Specs() {
		GPU_Vendor = (const char*)glGetString(GL_VENDOR);
		GPU_Model = (const char*)glGetString(GL_RENDERER);
		GL_Version = (const char*)glGetString(GL_VERSION);

	}

	void OpenGL4_Core::Save_Monitors() {
		int monitor_count;
		GLFWmonitor** monitors = glfwGetMonitors(&monitor_count);
		cout << "Detected monitors count: " << monitor_count << endl;
		for (unsigned int i = 0; i < monitor_count; i++) {
			GLFWmonitor* monitor = monitors[i];

			//Get monitor name provided by OS! It is a driver based name, so it maybe incorrect!
			string monitor_name = glfwGetMonitorName(monitor);
			GFX::MONITOR* gfx_monitor = new GFX::MONITOR(monitor, monitor_name);

			//Get videomode to detect at which resolution the OS is using the monitor
			const GLFWvidmode* monitor_vid_mode = glfwGetVideoMode(monitor);
			gfx_monitor->Set_Monitor_VidMode(monitor_vid_mode->width, monitor_vid_mode->height, monitor_vid_mode->blueBits, monitor_vid_mode->refreshRate);

			//Get monitor's physical size, developer may want to use it!
			glfwGetMonitorPhysicalSize(monitor, &gfx_monitor->PHYSICAL_WIDTH, &gfx_monitor->PHYSICAL_HEIGHT);

			CONNECTED_Monitors.push_back(gfx_monitor);
		}
	}

	//Create window in windowed mode, in final production: A window context should be known (Which screen mode, which GUIs to render etc.) and create this window according to it!s
	void OpenGL4_Core::Create_Window(string name) {
		cout << "Creating Window: " << name << endl;

		//Create window as it will share resources with Renderer Context to get display texture!
		GLFWwindow* window_id = glfwCreateWindow(1280, 720, name.c_str(), NULL, (GLFWwindow*)RENDERER->Renderer_Context);
		GFX::WINDOW* gfx_window = GFX_Core::Create_WindowOBJ(1280, 720, GFX::WINDOWED, CONNECTED_Monitors[0], CONNECTED_Monitors[0]->REFRESH_RATE, name, GFX::V_SYNC_OFF);
		gfx_window->Set_Window_GPUContext(window_id);
		glfwSetWindowMonitor(window_id, NULL, 0, 0, gfx_window->Get_Window_Mode().x, gfx_window->Get_Window_Mode().y, gfx_window->Get_Window_Mode().z);

		//Check and Report if GLFW fails
		if (window_id == NULL) {
			cout << "Error: We failed to create the window: " << name << " because of GLFW!\n";
			glfwTerminate();
		}

		glfwMakeContextCurrent(window_id);
		glfwSwapInterval(1);
		//GLAD is already loaded, so send Quad Mesh to context because we will render the texture given by Renderer in GFX->Refresh_Windows()!
		((OpenGL4_Renderer*)RENDERER)->Send_Quad_to_GPU();

		ONSCREEN_Windows.push_back(gfx_window);
	}

	void OpenGL4_Core::Create_Renderer() {
		//Create renderer class!
		RENDERER = new OpenGL4_Renderer;
		RENDERER->SELF = RENDERER;

		//Create offscreen renderer window to render everything!
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
		RENDERER->Renderer_Context = glfwCreateWindow(1920, 1080, "Offscreen_Renderer_Window", 0, 0);
		glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);

		//Check and Report if GLFW fails
		if (RENDERER->Renderer_Context == NULL) {
			cout << "We failed to create the window because of GLFW" << endl;
			glfwTerminate();
		}

		glfwMakeContextCurrent((GLFWwindow*)RENDERER->Renderer_Context);
		glfwSwapInterval(0);

		//Check and Report if GLAD fails
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			cout << "We failed to create the window because of GLAD" << endl;
		}


		Check_Computer_Specs();
		GFX::APICommander::SELF = new OpenGL4::APICommander;

		TuranAPI::LOG_STATUS("GPU Name: " + GPU_Vendor + " " + GPU_Model);
		TuranAPI::LOG_STATUS("OpenGL Version: " + GL_Version);

		//Set IMGUI's main renderer to IMGUI_OGL3!
		//All of the IMGUI contexts will use IMGUI's functionality to specify their rendering data!
		//But IMGUI will always use this class to render these datas!
		//So, GFX_IMGUI shouldn't be Window Context specific!
		IMGUI::IMGUI::GFX_IMGUI = new IMGUI_OGL4;
		if (!IMGUI::IMGUI::Check_IMGUI_Version()) {
			cout << "Error: IMGUI version check error!\n";
		}
	}

	//Delete this function!
	void OpenGL4_Core::Set_Window_Callbacks() {

		for (const GFX::WINDOW* WINDOW : Get_Window_List()) {
			GLFWwindow* window = (GLFWwindow*)(WINDOW->Get_Window_ConstGPUContext());

			glfwMakeContextCurrent(window);
			//glfwSetFramebufferSizeCallback(window, OGL3_SYS::framebuffer_size_callback);
			//glfwSetWindowFocusCallback(window, OGL3_SYS::window_focus_callback);
			//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			cout << "Callbacks set for window: " << WINDOW->Get_Window_Name() << endl;
		}

	}


	//WINDOW CALLBACKs

	//Set true if user uses window, otherwise false
	void OpenGL4_Core::window_focus_callback(GLFWwindow* window, int focused) {
		GFX::WINDOW* WINDOW = Get_Window_byID(window);
		SELF->Set_Window_Focus(WINDOW, focused);
	}

	//Set window and framebuffer size when windowed window is resized
	void OpenGL4_Core::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
		GFX::WINDOW* WINDOW = Get_Window_byID(window);
		SELF->Change_Window_Resolution(WINDOW, width, height);
		cout << "Callback Width: " << width << endl;
		cout << "Callback Height: " << height << endl;
	}

	void OpenGL4_Core::window_close_callback(GLFWwindow* window) {
		cout << "Window close callback is called!\n";
		GFX::WINDOW* WINDOW = Get_Window_byID(window);
		SELF->Close_Window(WINDOW);
		cout << "Leaving close callback!\n";
	}
	void OpenGL4_Core::Bind_Window_Context(GFX::WINDOW* window) {
		glfwMakeContextCurrent((GLFWwindow*)window->Get_Window_ConstGPUContext());
	}


	//RENDERING OPERATIONs

	void OpenGL4_Core::Render_IMGUI() {
		glfwMakeContextCurrent((GLFWwindow*)ONSCREEN_Windows[0]->Get_Window_ConstGPUContext());
		IMGUI::IMGUI::Render_Frame();
		IMGUI::IMGUI::Platform_Settings();
	}

	//Input (Keyboard-Controller) Operations

	//1) Send keys status' from the last frame to the Engine!
	//2) Take this frame's key status' from GLFW! But it will be used next frame!
	void OpenGL4_Core::Take_Inputs() {
		/*
		//For each key, set key pressed status with GLFW check!
		//This sends keys' status from last frame!
		if (true)
		{
			//KEYBOARD INPUT HANDLING

			for (KEYBOARD_KEYs key : Keyboard_Controller::Return_ALL_Keys()) {
				if (glfwGetKey(glfwGetCurrentContext(), Convert_Key_to_GLFW_Key(key)) == GLFW_PRESS) {
					Keyboard_Controller::Set_Key_is_Pressed(key, true);
					cout << "Key: " << key << " is pressed!\n";
				}
				//If Key isn't pressed this frame, set it as false because Maybe at previous frame it was pressed!
				else {
					Keyboard_Controller::Set_Key_is_Pressed(key, false);
				}
			}

			//MOUSE INPUT HANDLING

			switch (Mouse_Controller::CURSOR_MODE) {
			case MOUSE_INPUT_UNLIMITED:
				glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				break;
			case MOUSE_INPUT_HIDDEN:
				glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
				break;
			case MOUSE_INPUT_NORMAL:
				glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				break;

			default:
				cout << "Unsupported Cursor Mode in Mouse_Controller class!\n";
			}
			for (MOUSE_BUTTONs button : Mouse_Controller::Return_ALL_Buttons()) {
				if (glfwGetMouseButton(glfwGetCurrentContext(), Convert_MouseButton_to_GLFW_Key(button)) == GLFW_PRESS) {
					Mouse_Controller::Set_Button_is_Pressed(button, true);
					cout << "Mouse button: " << button << " is pressed!\n";
				}
				//If Mouse button isn't pressed this frame, set it as false because maybe at previous frame it was pressed!
				else {
					Mouse_Controller::Set_Button_is_Pressed(button, false);
				}
			}


			double xpos, ypos;
			glfwGetCursorPos(glfwGetCurrentContext(), &xpos, &ypos);
			Mouse_Controller::MOUSE_CURRENT_POSITION = vec2(xpos, ypos);

		}
		*/
		//Take this frame's key inputs!
		glfwPollEvents();
	}


	//GFX Resource Destroy Operations
	void OpenGL4_Core::Destroy_GFX_Resources() {
		for (GFX::MONITOR* monitor : CONNECTED_Monitors) {
			delete monitor;
		}
		for (GFX::WINDOW* window : ONSCREEN_Windows) {
			Close_Window(window);
			delete window;
		}
		IMGUI::IMGUI::Destroy_IMGUI_Resources();
		RENDERER->~Renderer();
		glfwTerminate();
	}

	void OpenGL4_Core::Load_GFX_Files() {
		cout << "ERROR: Define how to load and compile GFX_Files!\n";
		cout << "We are loading after initializing the Renderer, because we want to compile everything at start!\n";
		TuranAPI::Breakpoint();
	}



	void OpenGL4_Core::Show_Texture_on_Window(TuranAPI::File_System::Texture_Resource* TEXTURE) {
		glfwMakeContextCurrent((GLFWwindow*)ONSCREEN_Windows[0]->Get_Window_ConstGPUContext());
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TEXTURE->GL_ID, 0);

	}

	void OpenGL4_Core::Check_GL_Errors(const string& status) {
		int error = glGetError();
		//If there is a error, first print the status!
		if (error != NULL) {
			TuranAPI::LOG_ERROR("GL in status: " + status);
		}
		else {
			return;
		}

		//Print the error!
		if (error == GL_INVALID_OPERATION) {
			TuranAPI::LOG_ERROR("GL_INVALID_OPERATION!");
		}
		if (error == GL_INVALID_ENUM) {
			TuranAPI::LOG_ERROR("GL_INVALID_ENUM!");
		}
		if (error == GL_INVALID_VALUE) {
			TuranAPI::LOG_ERROR("GL_INVALID_VALUE!");
		}
		//I'm using NSight for graphic debugging for now, so I don't need this!
		//But if I won't use NSight for a time, I should activate this see real-usage graphic bugs!
		//SLEEP_THREAD(10);
	}
}