#pragma once
#include "OpenGL4_Includes.h"
#include "Renderer/OGL4_Renderer_Core.h"

namespace OpenGL4 {
	class OGL4_API OpenGL4_Core : public GFX::GFX_Core {
		friend class TuranEngine;
		friend class SYSTEM_INSPECTOR;

		OpenGL4_Core();

		virtual void Initialization() override;
		virtual void Check_Computer_Specs() override;
		virtual void Save_Monitors() override;
		virtual void Create_Renderer() override;
		static void GFX_Error_Callback(int error_code, const char* description);

	public:
		//Window Operations
		virtual void Create_Window(string name) override;
		virtual void Close_Window(GFX::WINDOW* window) override;
		virtual void Change_Window_Resolution(GFX::WINDOW* window, unsigned int width, unsigned int height) override;
		virtual void Set_Window_Focus(GFX::WINDOW* window, bool is_focused) override;
		virtual void Set_Window_Callbacks() override;
		virtual void Bind_Window_Context(GFX::WINDOW* window) override;

		//Window Callbacks
		static void window_focus_callback(GLFWwindow* window, int focused);
		static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
		static void window_close_callback(GLFWwindow* window);

		//Renderer Operations
		virtual void Render_IMGUI() override;
		static void Show_Texture_on_Window(TuranAPI::File_System::Texture_Resource* TEXTURE);
		virtual void Check_GL_Errors(const string& status) override;
		virtual void Load_GFX_Files() override;

		//Input (Keyboard-Controller) Operations
		virtual void Take_Inputs() override;

		//Resource Destroy Operations
		virtual void Destroy_GFX_Resources() override;
	};
}