#include "Properties_Window.h"
#include "TuranAPI/FileSystem/GameObjects/GameComponents/GameComponentTypes.h"
#include "EditorSource/FileSystem/GameObjects/Camera_Component.h"
#include "EditorSource/FileSystem/GameObjects/StaticModel_Component.h"
#include "EditorSource/FileSystem/ResourceTypes/ResourceTYPEs.h"
#include "GFXSource/GFX_FileSystem/Resource_Type/Texture_Resource.h"
#include "EditorSource/FileSystem/ResourceTypes/Model_Resource.h"
#include "GFXSource/GFX_Core.h"
#include "EditorSource/FileSystem/EditorFileSystem_Core.h"

#include "EditorSource/RenderContext/Game_RenderGraph.h"
#include "EditorSource/RenderContext/Draw Passes/Draw_Passes.h"

#include "Status_Window.h"
#include "Scene_Editor.h"

#include <string>

using namespace TuranAPI;

namespace TuranEditor {
	ResourceProperties_Window::ResourceProperties_Window(Resource_Identifier* resource) : IMGUI_WINDOW(resource->Get_Name().c_str()) {
		RESOURCE = resource;
		IMGUI_REGISTERWINDOW(this);

		if (RESOURCE->TYPE == RESOURCETYPEs::EDITOR_STATICMODEL) {
			RenderGraph = new Game_RenderGraph;
			Static_Model* MODEL = (Static_Model*)RESOURCE->DATA;
			vector<unsigned int> MESHIDs = MODEL->Upload_toGPU();
			for (unsigned int i = 0; i < MESHIDs.size(); i++) {
				GFX_API::DrawCall Call;
				Call.JoinedDrawPasses = Main_DrawPass::Get_BitMaskFlag();
				Call.MeshBuffer_ID = MESHIDs[i];
				Call.ShaderInstance_ID = 0;
				RenderGraph->Register_DrawCall(Call);
			}
		}
	}
	void Show_MaterialType_Properties(Resource_Identifier* resource);
	void Show_Model_Properties(Resource_Identifier* resource);
	void Show_MaterialInstance_Properties(Resource_Identifier* resource);
	void Show_Texture_Properties(Resource_Identifier* resource);

	void ResourceProperties_Window::Run_Window() {
		if (!Is_Window_Open) {
			IMGUI_DELETEWINDOW(this);
			return;
		}
		if (!IMGUI->Create_Window(Window_Name.c_str(), Is_Window_Open, false)) {
			IMGUI->End_Window();
			return;
		}
		switch (RESOURCE->TYPE) {
		case RESOURCETYPEs::EDITOR_STATICMODEL:
			Show_Model_Properties(RESOURCE);
			break;
		case RESOURCETYPEs::GFXAPI_TEXTURE:
			Show_Texture_Properties(RESOURCE);
			break;
		case RESOURCETYPEs::GFXAPI_MATTYPE:
			Show_MaterialType_Properties(RESOURCE);
			break;
		case RESOURCETYPEs::GFXAPI_MATINST:
			Show_MaterialInstance_Properties(RESOURCE);
			break;
		case RESOURCETYPEs::EDITOR_SCENE:
			new Scene_Editor(RESOURCE);
			Is_Window_Open = false;
			IMGUI_DELETEWINDOW(this);
			break;
		default:
			new Status_Window("This type's properties can't be shown by Properties Window!");
			Is_Window_Open = false;
		}
		IMGUI->End_Window();
	}

	void Show_MaterialType_Properties(Resource_Identifier* resource) {
		GFX_API::Material_Type* material_type_resource = (GFX_API::Material_Type*)resource->DATA;
		string MatTypeName = "Material Type Name: ";
		MatTypeName.append(resource->Get_Name());
		IMGUI->Text(MatTypeName.c_str());

		GFX_API::Material_Uniform* UNIFORM = nullptr;
		if (IMGUI->Begin_Tree("Uniform List")) {
			for (unsigned int i = 0; i < material_type_resource->UNIFORMs.size(); i++) {
				UNIFORM = &material_type_resource->UNIFORMs[i];

				if (IMGUI->Begin_Tree("number->string isn't supported yet!")) {
					string UniformName = "Uniform Name: ";
					UniformName.append(UNIFORM->VARIABLE_NAME);
					IMGUI->Text(UniformName.c_str());
					IMGUI->Text(("Uniform Variable Type: " + std::string(Find_UNIFORM_VARTYPE_Name(UNIFORM->VARIABLE_TYPE))).c_str());
					IMGUI->End_Tree();
				}
			}
			IMGUI->End_Tree();
		}
	}

	void Show_MaterialInstance_Properties(Resource_Identifier* resource) {
		GFX_API::Material_Instance* material_instance_resource = (GFX_API::Material_Instance*)resource->DATA;
		string MatInstName = "Material Instance Name: ";
		MatInstName.append(resource->Get_Name());
		IMGUI->Text(MatInstName.c_str());
		string MatTypeName = "Material Type Name: ";
		MatTypeName.append(
			EDITOR_FILESYSTEM->Find_ResourceIdentifier_byID(material_instance_resource->Material_Type)->Get_Name()
		);
		IMGUI->Text(MatTypeName.c_str());

		GFX_API::Material_Uniform* UNIFORM = nullptr;
		if (IMGUI->Begin_Tree("Uniform List")) {
			for (unsigned int i = 0; i < material_instance_resource->UNIFORM_LIST.size(); i++) {
				UNIFORM = &material_instance_resource->UNIFORM_LIST[i];

				if (IMGUI->Begin_Tree("number->string isn't supported yet!")) {
					string UniformName = "Uniform Name: ";
					UniformName.append(UNIFORM->VARIABLE_NAME);
					IMGUI->Text(UniformName.c_str());
					IMGUI->Text(("Uniform Variable Type: " + std::string(Find_UNIFORM_VARTYPE_Name(UNIFORM->VARIABLE_TYPE))).c_str());
					if (UNIFORM->VARIABLE_TYPE == GFX_API::UNIFORMTYPE::TEXTURE2D && UNIFORM->DATA) {
						IMGUI->Text("Texture ID: number->string isn't supported yet!");
					}
					IMGUI->End_Tree();
				}
			}
			IMGUI->End_Tree();
		}
	}


	void Show_Texture_Properties(Resource_Identifier* resource) {
		GFX_API::Texture_Resource* TEXTURE = (GFX_API::Texture_Resource*)resource->DATA;
		if (!TEXTURE) {
			EDITOR_FILESYSTEM->Load_Resource(resource->ID);
		}
		IMGUI->Text(("ID: " + to_string(resource->ID)).c_str());
		IMGUI->Text(GFX_API::Find_UNIFORM_VARTYPE_Name(TEXTURE->Properties.VALUE_TYPE));
		GFXContentManager->Upload_Texture(TEXTURE, resource->ID, false);
		IMGUI->Display_Texture(resource->ID, TEXTURE->WIDTH, TEXTURE->HEIGHT);
	}

	void Show_Model_Properties(Resource_Identifier* resource) {
		Static_Model* model_data_resource = (Static_Model*)resource->DATA;
		string ModelName = "Model Name: ";
		ModelName.append(resource->Get_Name());
		IMGUI->Text(ModelName.c_str());
		IMGUI->Text(("ID: " + to_string(resource->ID)).c_str());
		string Mesh_Number = "Mesh Number: ";
		Mesh_Number.append(std::to_string(model_data_resource->MESHes.size()).c_str());
		IMGUI->Text(Mesh_Number.c_str());

	}






	void Show_StaticModelComp_Properties(TuranAPI::GameComponent* Component);
	void Show_CameraComp_Properties(TuranAPI::GameComponent* Component);

	GameComponentProperties_Window::GameComponentProperties_Window(TuranAPI::GameComponent* gamecomponent) : IMGUI_WINDOW("Game Component Properties"), GAMECOMPONENT(gamecomponent) {}
	void GameComponentProperties_Window::Run_Window() {
		if (!Is_Window_Open) {
			IMGUI_DELETEWINDOW(this);
			return;
		}
		if (!IMGUI->Create_Window(Window_Name.c_str(), Is_Window_Open, false)) {
			IMGUI->End_Window();
			return;
		}
		switch (GAMECOMPONENT->TYPE) {
		case GameComponentType::STATIC_MODEL_C:
			Show_StaticModelComp_Properties(GAMECOMPONENT);
			break;
		case GameComponentType::CAMERA_C:
			Show_CameraComp_Properties(GAMECOMPONENT);
			break;
		default:
			std::cout << "ERROR: Intended Component Type isn't supported by Properties Window!\n";
			TuranAPI::Breakpoint();
		}


		IMGUI->End_Window();
	}

	void Show_StaticModelComp_Properties(TuranAPI::GameComponent* Component) {
		StaticModel_Component* COMP = (StaticModel_Component*)Component;
		string ComponentName = "Component Name: ";
		ComponentName.append(COMP->NAME);
		IMGUI->Text(ComponentName.c_str());
		string ModelName = "Model Name: ";
		ModelName.append(COMP->MODEL->Get_Name());
		IMGUI->Text(ModelName.c_str());
		if (IMGUI->Begin_Tree("Material Instances")) {
			for (unsigned int i = 0; i < COMP->MATERIALs.size(); i++) {
				GFX_API::Material_Instance* MatInst = (GFX_API::Material_Instance*)COMP->MATERIALs[i]->DATA;
				if (IMGUI->Begin_Tree("number->string isn't supported yet!")) {
					IMGUI->Text(COMP->MATERIALs[i]->Get_Name().c_str());
					IMGUI->End_Tree();
				}
			}
			IMGUI->End_Tree();
		}

	}
	void Show_CameraComp_Properties(TuranAPI::GameComponent* Component) {
		Camera_Component* COMP = (Camera_Component*)Component;
		string ComponentName = "Component Name: ";
		ComponentName.append(COMP->NAME);
		IMGUI->Text(ComponentName.c_str());
		vec3 Position = COMP->Get_Position();
		vec3 Target = COMP->Get_Target();
		int FOV_inAngle = COMP->Get_FOV_inAngle();
		vec2 Aspect_Width_and_Height = COMP->Get_Aspect_Width_and_Height();
		vec2 Near_and_FarPlane = COMP->Get_Near_and_FarPlane();

		if (IMGUI->Slider_Vec3("Position", &Position, -1000, 1000)) {
			COMP->Translate(Position);
		}
		if (IMGUI->Slider_Vec3("Target", &Target, -1000, 1000)) {
			COMP->Set_Camera_Target(Target);
		}
		if (IMGUI->Slider_Int("FOV in angle", &FOV_inAngle, 0, 179)) {
			COMP->Set_Camera_Properties(FOV_inAngle, Aspect_Width_and_Height.x, Aspect_Width_and_Height.y, Near_and_FarPlane.x, Near_and_FarPlane.y);
		}
		if (IMGUI->Slider_Vec2("Aspect Width and Height", &Aspect_Width_and_Height, 0, 2000)) {
			COMP->Set_Camera_Properties(FOV_inAngle, Aspect_Width_and_Height.x, Aspect_Width_and_Height.y, Near_and_FarPlane.x, Near_and_FarPlane.y);
		}
		if (IMGUI->Slider_Vec2("Near and Far Plane", &Near_and_FarPlane, 0, 100000)) {
			COMP->Set_Camera_Properties(FOV_inAngle, Aspect_Width_and_Height.x, Aspect_Width_and_Height.y, Near_and_FarPlane.x, Near_and_FarPlane.y);
		}
	}

}