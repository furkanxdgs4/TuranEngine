#pragma once
#include "EditorSource/Editor_Includes.h"
#include "GFXSource/IMGUI/IMGUI_WINDOW.h"
#include "EditorSource/FileSystem/ResourceTypes/Resource_Identifier.h"

namespace TuranEditor {

	class Texture_Import_Window : public GFX_API::IMGUI_WINDOW {
		string TEXTURE_PATH = "", OUTPUT_FOLDER = "", OUTPUT_NAME = "";
	public:
		Texture_Import_Window();
		virtual void Run_Window();
	};


	class Texture_Loader {
	public:
		static GFX_API::Texture_Resource* Import_Texture(const char* PATH,
			const bool& flip_vertically = false, const GFX_API::Texture_Properties& Properties = GFX_API::Texture_Properties());
	};

}