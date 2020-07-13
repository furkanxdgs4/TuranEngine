#pragma once
#include "EditorSource/Editor_Includes.h"
#include "GFXSource/IMGUI/IMGUI_WINDOW.h"
#include "EditorSource/FileSystem/ResourceTypes/Resource_Identifier.h"

namespace TuranEditor {

	class MaterialInstance_CreationWindow : public GFX_API::IMGUI_WINDOW {
		Resource_Identifier* selected_materialtype = nullptr;
		string OUTPUT_FOLDER, OUTPUT_NAME, UNIFORM_NAME;
	public:
		MaterialInstance_CreationWindow();
		virtual void Run_Window() override;
	};

}