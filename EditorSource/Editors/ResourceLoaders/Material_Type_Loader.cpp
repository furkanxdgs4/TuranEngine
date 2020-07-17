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
			return;
		}

		if (SHADERSOURCEs.size() != EDITOR_FILESYSTEM->Get_SpecificAssetType(RESOURCETYPEs::GFXAPI_SHADERSOURCE).size()) {
			SHADERSOURCEs.clear();	vssource_indexes.clear(); vssource_names.clear();
			fssource_indexes.clear();	fssource_names.clear();
			SHADERSOURCEs = EDITOR_FILESYSTEM->Get_SpecificAssetType(RESOURCETYPEs::GFXAPI_SHADERSOURCE);
			for (unsigned int i = 0; i < SHADERSOURCEs.size(); i++) {
				ShaderSource_Resource* SHADERSOURCE = (ShaderSource_Resource*)SHADERSOURCEs[i]->DATA;
				switch (SHADERSOURCE->STAGE) {
				case SHADER_STAGE::VERTEXSTAGE:
					vssource_indexes.push_back(i);
					vssource_names.push_back(SHADERSOURCEs[i]->Get_Name());
					break;
				case SHADER_STAGE::FRAGMENTSTAGE:
					fssource_indexes.push_back(i);
					fssource_names.push_back(SHADERSOURCEs[i]->Get_Name());
					break;
				default:
					break;
				}
			}
		}

		IMGUI->SelectList_OneLine("Vertex Shader", &selected_vsindex, &vssource_names);
		IMGUI->SelectList_OneLine("Fragment Shader", &selected_fsindex, &fssource_names);
		IMGUI->Input_Text("Output Folder", &OUTPUT_FOLDER);
		IMGUI->Input_Text("Output Name", &OUTPUT_NAME);

		if (IMGUI->Button("Import")) {
			string status;
			string PATH = OUTPUT_FOLDER;
			PATH.append(OUTPUT_NAME);
			PATH.append(".mattypecont");

			//Check if this resource is already loaded to Content_List!
			for (size_t i = 0; i < TuranEditor::EDITOR_FILESYSTEM->Get_AssetList().size(); i++) {
				Resource_Identifier* RESOURCE = TuranEditor::EDITOR_FILESYSTEM->Get_AssetList()[i];
				if (PATH == RESOURCE->PATH) {
					status = "Resource is already loaded and is in the Resource List!";
					TuranEditor::Status_Window* error_window = new TuranEditor::Status_Window(status.c_str());
					IMGUI->End_Window();
					return;
				}
			}


			//A little check for each uniform! Only variable names are checked.
			GFX_API::Material_Uniform* UNIFORM = nullptr;
			for (unsigned int i = 0; i < Material_Uniforms.size(); i++) {
				UNIFORM = &Material_Uniforms[i];
				if (UNIFORM->VARIABLE_NAME == "") {
					TuranAPI::LOG_WARNING("A uniform's name isn't set, please check all uniform names!");
					SLEEP_THREAD(8);
					IMGUI->End_Window();
					return;
				}
			}

			VS = SHADERSOURCEs[vssource_indexes[selected_vsindex]];
			FS = SHADERSOURCEs[fssource_indexes[selected_fsindex]];

			GFX_API::Material_Type* MATERIAL = new GFX_API::Material_Type;

			Resource_Identifier* RESOURCE = new Resource_Identifier;
			RESOURCE->PATH = PATH;
			RESOURCE->DATA = MATERIAL;
			RESOURCE->TYPE = RESOURCETYPEs::GFXAPI_MATTYPE;
			EDITOR_FILESYSTEM->Add_anAsset_toFileList(RESOURCE);

			MATERIAL->VERTEXSOURCE_ID = VS->ID;
			MATERIAL->FRAGMENTSOURCE_ID = FS->ID;
			//Add Material Uniforms to compiled resource
			GFX_API::Material_Uniform* material_uniform = nullptr;
			for (unsigned int i = 0; i < Material_Uniforms.size(); i++) {
				material_uniform = &Material_Uniforms[i];
				MATERIAL->UNIFORMs.push_back(*material_uniform);
			}

			GFXContentManager->Link_MaterialType(MATERIAL, RESOURCE->ID, &status);
			if ("Succesfully linked!" == status) {
				EDITOR_FILESYSTEM->Save_Resource(RESOURCE->ID);
			}
			else {
				GFXContentManager->Delete_MaterialType(RESOURCE->ID);
				EDITOR_FILESYSTEM->Delete_anAsset_fromFileList(RESOURCE->ID);
			}

			TuranEditor::Status_Window* error_window = new TuranEditor::Status_Window(status.c_str());

			Material_Uniforms.clear();	typeindex_peruniform.clear(); 
			vssource_indexes.clear(); vssource_names.clear();
			fssource_indexes.clear(); fssource_names.clear();


			//Finish the window here!
			Is_Window_Open = false;
			IMGUI->End_Window();
			return;
		}

		IMGUI->Same_Line();
		if (IMGUI->Button("Add Uniform")) {
			Material_Uniforms.push_back(GFX_API::Material_Uniform());
		}
		IMGUI->Same_Line();
		if (IMGUI->Button("Read Shader Sources")) {
			is_Reading_Shaders = true;
			VS = SHADERSOURCEs[vssource_indexes[selected_vsindex]];
			FS = SHADERSOURCEs[fssource_indexes[selected_fsindex]];
		}

		typeindex_peruniform.resize(Material_Uniforms.size());
		GFX_API::Material_Uniform* UNIFORM = nullptr;
		if (IMGUI->Begin_Tree("Uniform List")) {
			for (unsigned int i = 0; i < Material_Uniforms.size(); i++) {
				UNIFORM = &Material_Uniforms[i];

				if (IMGUI->Begin_Tree(std::to_string(i).c_str())) {
					IMGUI->Input_Text("Uniform Name", &UNIFORM->VARIABLE_NAME);
					unsigned int selected = typeindex_peruniform[i];
					if (IMGUI->SelectList_OneLine("Uniform Variable Type", &selected, &UNIFORM_VAR_TYPE_NAMEs)) {
						UNIFORM->VARIABLE_TYPE = Find_in_Uniform_VarTypes(typeindex_peruniform[i]);
					}
					IMGUI->End_Tree();
				}
			}
			IMGUI->End_Tree();
		}
		if (is_Reading_Shaders) {
			IMGUI->Text("Vertex Source:");
			IMGUI->Text(((ShaderSource_Resource*)VS->DATA)->SOURCE_CODE.c_str());
			IMGUI->Text("Fragment Source:");
			IMGUI->Text(((ShaderSource_Resource*)FS->DATA)->SOURCE_CODE.c_str());
		}

		IMGUI->End_Window();
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