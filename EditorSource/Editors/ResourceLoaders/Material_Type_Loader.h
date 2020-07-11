#pragma once
#include "EditorSource/Editor_Includes.h"
#include "GFXSource/IMGUI/IMGUI_WINDOW.h"
#include "EditorSource/FileSystem/ResourceTypes/Resource_Identifier.h"

namespace TuranEditor {


	class Material_Import_Window : public GFX_API::IMGUI_WINDOW {
		string MATERIALTYPE_VERTEX_PATH = "", MATERIALTYPE_FRAGMENT_PATH = "", OUTPUT_FOLDER = "", OUTPUT_NAME = "", VERTEX_SOURCE = "", FRAGMENT_SOURCE = "";
		bool is_Reading_Shaders = false;
		vector<GFX_API::Material_Uniform> Material_Uniforms;
		vector<int> selectlist_vector;
		vector<const char*> UNIFORM_VAR_TYPE_NAMEs;
	public:
		Material_Import_Window();
		virtual void Run_Window();
	};

	class Material_Type_Loader {
	public:
		static Resource_Identifier* Load_MaterialType(const char* vertex_path, const char* fragment_path, const char* output_path
			, vector<GFX_API::Material_Uniform>* material_inputs, string* compilation_status);
	};

}