#pragma once
#include "OGL4_ENUMs.h"
#include "Renderer/OGL4_Renderer_Core.h"

#include "OGL4_Display.h"

namespace OpenGL4 {
	class OGL4_API OpenGL4_Core : public GFX_API::GFX_Core {
		virtual void Initialization() override;
		virtual void Check_Computer_Specs() override;
		virtual void Save_Monitors() override;
		virtual void Create_Renderer() override;
		void Create_MainWindow();
		static void GFX_Error_Callback(int error_code, const char* description);
	public:
		OpenGL4_Core();
		~OpenGL4_Core();

		//Window Operations
		virtual void Change_Window_Resolution(GFX_API::WINDOW* window, unsigned int width, unsigned int height) override;

		//Window Callbacks
		static void window_focus_callback(GLFWwindow* window, int focused);
		static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
		static void window_close_callback(GLFWwindow* window);

		//Renderer Operations
		void Show_Texture_on_Window(GFX_API::Texture_Resource* TEXTURE);
		void Check_GL_Errors(const char* status);
		virtual void Load_GFX_Files() override;
		virtual void Swapbuffers_ofMainWindow() override;

		//Input (Keyboard-Controller) Operations
		virtual void Take_Inputs() override;

		//Resource Destroy Operations
		virtual void Destroy_GFX_Resources() override;
	};

	GFX_API::GFX_Core OGL4_API* Start_OGL4Systems(TuranAPI::TAPI_Systems* TAPISystems);
	OGL4_API void Close_OGL4DLL();
}