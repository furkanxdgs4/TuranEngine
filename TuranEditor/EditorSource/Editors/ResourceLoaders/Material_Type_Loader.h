#pragma once
#include "EditorSource/Editor_Includes.h"
#include "GFXSource/IMGUI/IMGUI_WINDOW.h"

class Material_Import_Window : public GFX_API::IMGUI_WINDOW {
	String MATERIALTYPE_VERTEX_PATH = "", MATERIALTYPE_FRAGMENT_PATH = "", OUTPUT_FOLDER = "", OUTPUT_NAME = "", VERTEX_SOURCE = "", FRAGMENT_SOURCE = "";
	bool is_Reading_Shaders = false;
	Vector<GFX_API::Material_Uniform> Material_Uniforms;
	Vector<int> selectlist_vector;
	Vector<const char*> UNIFORM_VAR_TYPE_NAMEs;
public:
	Material_Import_Window();
	virtual void Run_Window();
};

class Material_Type_Loader {
public:
	static TuranAPI::Resource_Type* Load_MaterialType(const char* vertex_path, const char* fragment_path, const char* output_path
		, Vector<GFX_API::Material_Uniform>* material_inputs, String* compilation_status);
};

