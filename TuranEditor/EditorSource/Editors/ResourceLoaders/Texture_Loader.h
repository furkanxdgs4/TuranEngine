#pragma once
#include "EditorSource/Editor_Includes.h"
#include "GFXSource/IMGUI/IMGUI_WINDOW.h"

namespace TuranEditor {

	class Texture_Import_Window : public GFX_API::IMGUI_WINDOW {
		String TEXTURE_PATH = "", OUTPUT_FOLDER = "", OUTPUT_NAME = "";
	public:
		Texture_Import_Window();
		virtual void Run_Window();
	};


	class Texture_Loader {
	public:
		static TuranAPI::Resource_Type* Import_Texture(const char* PATH, const char* output_path,
			const bool& flip_vertically = false, const GFX_API::Texture_Properties& Properties = GFX_API::Texture_Properties());
	};

}