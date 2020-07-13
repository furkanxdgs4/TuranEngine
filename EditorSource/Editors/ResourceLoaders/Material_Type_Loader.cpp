#include "Material_Type_Loader.h"
#include "EditorSource/FileSystem/EditorFileSystem_Core.h"
//To show import status
#include "EditorSource/Editors/Status_Window.h"
//To compile shader program!
#include "GFXSource/GFX_Core.h"
#include "EditorSource/FileSystem/ResourceTypes/ResourceTYPEs.h"

using namespace GFX_API;

namespace TuranEditor {

	Material_Import_Window::Material_Import_Window()
		: IMGUI_WINDOW("Material Type Import")
	{
		IMGUI_REGISTERWINDOW(this);
		std::initializer_list<const char*> list{ "Unsigned Integer 32-bit", "Integer 32-bit", "Float 32-bit", "Vec2 (2 float)", "Vec3 (3 float)", "Vec4 (4 float)", "Matrix 4x4", "Texture 2D" };
		UNIFORM_VAR_TYPE_NAMEs = list;
	}

	GFX_API::UNIFORMTYPE Find_in_Uniform_VarTypes(const int& var_type);

	void Material_Import_Window::Run_Window() {
		if (!Is_Window_Open) {
			IMGUI_DELETEWINDOW(this);
			return;
		}
		if (!IMGUI->Create_Window(Window_Name.c_str(), Is_Window_Open, false)) {
			IMGUI->End_Window();
		}
		/*
		std::cout << "Material Type Import screen is active!\n";
		IMGUI->Input_Text("Vertex Resource Path", &MATERIALTYPE_VERTEX_PATH);
		IMGUI->Input_Text("Fragment Resource Path", &MATERIALTYPE_FRAGMENT_PATH);
		IMGUI->Input_Text("Output Folder", &OUTPUT_FOLDER);
		IMGUI->Input_Text("Output Name", &OUTPUT_NAME);

		if (IMGUI->Button("Import")) {
			string status;
			string PATH = OUTPUT_FOLDER;
			PATH.append(OUTPUT_NAME);

			//Check if this resource is already loaded to Content_List!
			for (size_t i = 0; i < TuranEditor::EDITOR_FILESYSTEM->Get_AssetList().size(); i++) {
				Resource_Identifier* RESOURCE = TuranEditor::EDITOR_FILESYSTEM->Get_AssetList()[i];
				if (PATH == RESOURCE->PATH) {
					status = "Resource is already loaded and is in the Resource List!";
					TuranEditor::Status_Window* error_window = new TuranEditor::Status_Window(status.c_str());
					return;
				}
			}


			//A little check for each uniform! Only variable names are checked.
			GFX_API::Material_Uniform* UNIFORM = nullptr;
			for (unsigned int i = 0; i < Material_Uniforms.size(); i++) {
				UNIFORM = &Material_Uniforms[i];
				if (UNIFORM->VARIABLE_NAME == "") {
					std::cout << "A uniform's name isn't set, please check all uniform names!\n";
					SLEEP_THREAD(8);
					IMGUI->End_Window();
					return;
				}
			}
			Material_Type* imported_resource = Material_Type_Loader::Load_MaterialType(MATERIALTYPE_VERTEX_PATH.c_str(), MATERIALTYPE_FRAGMENT_PATH.c_str(), &Material_Uniforms, &status);
			TuranEditor::Status_Window* error_window = new TuranEditor::Status_Window(status.c_str());
			if (imported_resource) {
				Resource_Identifier* RESOURCE = new Resource_Identifier;
				RESOURCE->DATA = imported_resource;
				RESOURCE->PATH = PATH;
				RESOURCE->TYPE = RESOURCETYPEs::GFXAPI_MATTYPE;
				EDITOR_FILESYSTEM->Add_anAsset_toFileList(RESOURCE);
				//Add_anAsset_toFileList gave an ID to Resource, so we can save it!
				EDITOR_FILESYSTEM->Save_Resource(RESOURCE->ID);
			}

			Material_Uniforms.clear();
			selectlist_vector.clear();


			//Finish the window here! No status checking for now!
			Is_Window_Open = false;
			IMGUI->End_Window();
			return;
		}

		IMGUI->Same_Line();
		if (IMGUI->Button("Add Uniform")) {
			Material_Uniforms.push_back(GFX_API::Material_Uniform());
		}
		IMGUI->Same_Line();
		if (IMGUI->Button("Read Material Type")) {
			VERTEX_SOURCE = *TuranAPI::FileSystem::Read_TextFile(MATERIALTYPE_VERTEX_PATH.c_str(), LASTUSEDALLOCATOR);
			FRAGMENT_SOURCE = *TuranAPI::FileSystem::Read_TextFile(MATERIALTYPE_FRAGMENT_PATH.c_str(), LASTUSEDALLOCATOR);
			is_Reading_Shaders = true;
		}

		//selectlist_vector.resize(Material_Uniforms.size());
		GFX_API::Material_Uniform* UNIFORM = nullptr;
		if (IMGUI->Begin_Tree("Uniform List")) {
			for (unsigned int i = 0; i < Material_Uniforms.size(); i++) {
				UNIFORM = &Material_Uniforms[i];

				if (IMGUI->Begin_Tree(std::to_string(i).c_str())) {
					IMGUI->Input_Text("Uniform Name", &UNIFORM->VARIABLE_NAME);
					unsigned int selected = selectlist_vector[i];
					if (IMGUI->SelectList_OneLine("Uniform Variable Type", &selected, &UNIFORM_VAR_TYPE_NAMEs)) {
						UNIFORM->VARIABLE_TYPE = Find_in_Uniform_VarTypes(selectlist_vector[i]);
					}
					IMGUI->End_Tree();
				}
			}
			IMGUI->End_Tree();
		}
		if (is_Reading_Shaders) {
			IMGUI->Text("Vertex Source:");
			IMGUI->Text(VERTEX_SOURCE.c_str());
			IMGUI->Text("Fragment Source:");
			IMGUI->Text(FRAGMENT_SOURCE.c_str());
		}

		*/
		IMGUI->End_Window();
	}




	//Use this function to import a shader stage first time! Not for reloading a shader!
	//Output Path defines DIRECTORY + NAME! Like "C:/dev/Content/First_Material". Every MatType has .mattypecont extension!
	GFX_API::Material_Type* Material_Type_Loader::Load_MaterialType(unsigned int VertexShader_ID, unsigned int FragmentShader_ID, vector<GFX_API::Material_Uniform>* material_inputs, string* compilation_status) {
		/*
		string vertexshader_compilationstatus;
		ShaderSource_Resource* vertex_shader = new ShaderSource_Resource;
		vertex_shader->LANGUAGE = SHADER_LANGUAGEs::GLSL;
		vertex_shader->STAGE = SHADER_STAGE::VERTEXSTAGE;
		vertex_shader->SOURCE_CODE = *TuranAPI::FileSystem::Read_TextFile(vertex_path, LASTUSEDALLOCATOR);


		string fragmentshader_compilationstatus;
		ShaderSource_Resource* fragment_shader = new ShaderSource_Resource;
		fragment_shader->LANGUAGE = SHADER_LANGUAGEs::GLSL;
		fragment_shader->STAGE = SHADER_STAGE::VERTEXSTAGE;
		fragment_shader->SOURCE_CODE = *TuranAPI::FileSystem::Read_TextFile(fragment_path, LASTUSEDALLOCATOR);
		GFXContentManager->Compile_ShaderSource(vertex_shader, &fragmentshader_compilationstatus);*/



		string link_status;
		GFX_API::Material_Type* MATERIAL = new GFX_API::Material_Type;
		MATERIAL->VERTEXSOURCE_ID = VertexShader_ID;
		MATERIAL->FRAGMENTSOURCE_ID = FragmentShader_ID;
		GFXContentManager->Link_MaterialType(MATERIAL, &link_status);


		//Add Material Uniforms to compiled resource
		GFX_API::Material_Uniform* material_uniform = nullptr;
		for (unsigned int i = 0; i < material_inputs->size(); i++) {
			material_uniform = &(*material_inputs)[i];
			MATERIAL->UNIFORMs.push_back(*material_uniform);
		}

		compilation_status->append("Succesfully compiled and saved the resource to disk!");
		return MATERIAL;
	}


	GFX_API::UNIFORMTYPE Find_in_Uniform_VarTypes(const int& var_type) {
		switch (var_type) {
		case 0:
			return GFX_API::UNIFORMTYPE::VAR_UINT32;
		case 1:
			return GFX_API::UNIFORMTYPE::VAR_INT32;
		case 2:
			return GFX_API::UNIFORMTYPE::VAR_FLOAT32;
		case 3:
			return GFX_API::UNIFORMTYPE::VAR_VEC2;
		case 4:
			return GFX_API::UNIFORMTYPE::VAR_VEC3;
		case 5:
			return GFX_API::UNIFORMTYPE::VAR_VEC4;
		case 6:
			return GFX_API::UNIFORMTYPE::VAR_MAT4x4;
		case 7:
			return GFX_API::UNIFORMTYPE::TEXTURE2D;
		default:
			TuranAPI::LOG_CRASHING("Uniform Variable Type isn't supported in Material Type Loader! Returns INT");
			return GFX_API::UNIFORMTYPE::VAR_INT32;
		}
	}
}