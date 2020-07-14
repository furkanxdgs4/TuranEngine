#pragma once
#include "EditorSource/Editor_Includes.h"
#include "GFXSource/IMGUI/IMGUI_WINDOW.h"
#include "EditorSource/FileSystem/ResourceTypes/Resource_Identifier.h"
#include "GFXSource/GFX_FileSystem/Resource_Type/Material_Type_Resource.h"

namespace TuranEditor {


	class Material_Import_Window : public GFX_API::IMGUI_WINDOW {
		string OUTPUT_FOLDER = "", OUTPUT_NAME = "";
		bool is_Reading_Shaders = false;
		vector<Resource_Identifier*> SHADERSOURCEs;
		Resource_Identifier* VS, *FS;
		vector<GFX_API::Material_Uniform> Material_Uniforms;
		vector<const char*> UNIFORM_VAR_TYPE_NAMEs;
		vector<string> vssource_names, fssource_names;

		vector<int> typeindex_peruniform,
			vssource_indexes, fssource_indexes;		//This indexes stores which elements of SHADERSOURCEs vector are Vertex Shader or Fragment Shader
		unsigned int selected_vsindex, selected_fsindex;
	public:
		Material_Import_Window();
		virtual void Run_Window();
	};

	class Material_Type_Loader {
	public:
		static GFX_API::Material_Type* Load_MaterialType(unsigned int VertexShader_ID, unsigned int FragmentShader_ID
			, vector<GFX_API::Material_Uniform>* material_inputs, string* compilation_status);
	};

}