#pragma once
#include "EditorSource/Editor_Includes.h"
#include "GFXSource/IMGUI/IMGUI_WINDOW.h"
#include "EditorSource/FileSystem/ResourceTypes/Model_Resource.h"

namespace TuranEditor {

	class Model_Import_Window : public GFX_API::IMGUI_WINDOW {
		string MODEL_IMPORT_PATH, OUTPUT_FOLDER, OUTPUT_NAME;
		//This is used to find which Content List we are importing the model to! For example GFX_Contents, Engine_Contents, GameContents... 
	public:
		Model_Import_Window();
		virtual ~Model_Import_Window();
		virtual void Run_Window() override;
	};

	class Model_Loader {
	public:
		static Static_Model* Import_Model(const char* PATH, const char* output_path, string* compilation_status);
	};
}