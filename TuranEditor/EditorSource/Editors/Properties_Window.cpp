#include "Properties_Window.h"
#include "TuranAPI/FileSystem/GameObjects/GameComponents/GameComponentTypes.h"
#include "EditorSource/FileSystem/GameObjects/Camera_Component.h"
#include "EditorSource/FileSystem/GameObjects/StaticModel_Component.h"
#include "TuranAPI/FileSystem/Resource_Types/ResourceTYPEs.h"
#include "GFXSource/GFX_FileSystem/Resource_Type/Texture_Resource.h"
#include "GFXSource/GFX_Core.h"
#include "EditorSource/FileSystem/EditorFileSystem_Core.h"

#include "EditorSource/RenderContext/Game_RenderGraph.h"
#include "EditorSource/RenderContext/Draw Passes/Draw_Passes.h"

#include <string>

using namespace TuranAPI;

namespace TuranEditor {
	ResourceProperties_Window::ResourceProperties_Window(TuranAPI::Resource_Type* resource) : IMGUI_WINDOW(resource->NAME) {
		RESOURCE = resource;
		IMGUI_REGISTERWINDOW(this);

		if (RESOURCE->Get_Type() == RESOURCETYPEs::EDITOR_STATICMODEL) {
			RenderGraph = new Game_RenderGraph;
			Static_Model_Data* MODEL = (Static_Model_Data*)RESOURCE;
			Vector<unsigned int> MESHIDs = MODEL->Upload_toGPU();
			for (unsigned int i = 0; i < MESHIDs.size(); i++) {
				GFX_API::DrawCall Call;
				Call.JoinedDrawPasses = Main_DrawPass::Get_BitMaskFlag();
				Call.MeshBuffer_ID = MESHIDs[i];
				Call.ShaderInstance_ID = 0;
				RenderGraph->Register_DrawCall(Call);
			}
		}
	}
	void Show_MaterialType_Properties(TuranAPI::Resource_Type* resource);
	void Show_Model_Properties(TuranAPI::Resource_Type* resource);
	void Show_MaterialInstance_Properties(TuranAPI::Resource_Type* resource);
	void Show_Texture_Properties(TuranAPI::Resource_Type* resource);

	void ResourceProperties_Window::Run_Window() {
		if (!Is_Window_Open) {
			IMGUI_DELETEWINDOW(this);
			return;
		}
		if (!IMGUI->Create_Window(Window_Name, Is_Window_Open, false)) {
			IMGUI->End_Window();
			return;
		}
		switch (RESOURCE->Get_Type()) {
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
		default:
			TuranAPI::LOG_ERROR("This type's properties can't be shown by Properties Window!");
			SLEEP_THREAD(5);
		}
		IMGUI->End_Window();
	}

	void Show_MaterialType_Properties(TuranAPI::Resource_Type* resource) {
		GFX_API::Material_Type_Resource* material_type_resource = (GFX_API::Material_Type_Resource*)resource;
		String MatTypeName = "Material Type Name: ";
		MatTypeName.append(material_type_resource->NAME);
		IMGUI->Text(MatTypeName);


		if (IMGUI->Begin_Tree("Uniform List")) {
			for (unsigned int i = 0; i < material_type_resource->UNIFORMs.size(); i++) {
				GFX_API::Material_Uniform* UNIFORM = &material_type_resource->UNIFORMs[i];

				if (IMGUI->Begin_Tree("number->string isn't supported yet!")) {
					String UniformName = "Uniform Name: ";
					UniformName.append(UNIFORM->VARIABLE_NAME);
					IMGUI->Text(UniformName);
					IMGUI->Text(("Uniform Variable Type: " + std::string(Find_UNIFORM_VARTYPE_Name(UNIFORM->VARIABLE_TYPE))).c_str());
					IMGUI->End_Tree();
				}
			}
			IMGUI->End_Tree();
		}
	}

	void Show_MaterialInstance_Properties(TuranAPI::Resource_Type* resource) {
		GFX_API::Material_Instance* material_instance_resource = (GFX_API::Material_Instance*)resource;
		String MatInstName = "Material Instance Name: ";
		MatInstName.append(material_instance_resource->NAME);
		IMGUI->Text(MatInstName);
		String MatTypeName = "Material Type Name: ";
		MatTypeName.append(material_instance_resource->Material_Type->NAME);
		IMGUI->Text(MatTypeName);


		if (IMGUI->Begin_Tree("Uniform List")) {
			for (unsigned int i = 0; i < material_instance_resource->UNIFORM_LIST.size(); i++) {
				GFX_API::Material_Uniform* UNIFORM = &material_instance_resource->UNIFORM_LIST[i];

				if (IMGUI->Begin_Tree("number->string isn't supported yet!")) {
					String UniformName = "Uniform Name: ";
					UniformName.append(UNIFORM->VARIABLE_NAME);
					IMGUI->Text(UniformName);
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


	void Show_Texture_Properties(TuranAPI::Resource_Type* resource) {
		GFX_API::Texture_Resource* TEXTURE = (GFX_API::Texture_Resource*)resource;
		std::cout << "ID: " << TEXTURE->ID << std::endl;
		IMGUI->Text(GFX_API::Find_UNIFORM_VARTYPE_Name(TEXTURE->Properties.VALUE_TYPE));
		IMGUI->Display_Texture(&TEXTURE->ID, TEXTURE->WIDTH, TEXTURE->HEIGHT);
	}

	void Show_Model_Properties(TuranAPI::Resource_Type* resource) {
		Static_Model_Data* model_data_resource = (Static_Model_Data*)resource;
		String ModelName = "Model Name: ";
		ModelName.append(model_data_resource->NAME);
		IMGUI->Text(ModelName);
		String Mesh_Number = "Mesh Number: ";
		Mesh_Number.append(std::to_string(model_data_resource->MESHes.size()).c_str());
		IMGUI->Text(Mesh_Number);

	}






	void Show_StaticModelComp_Properties(TuranAPI::GameComponent* Component);
	void Show_CameraComp_Properties(TuranAPI::GameComponent* Component);

	GameComponentProperties_Window::GameComponentProperties_Window(TuranAPI::GameComponent* gamecomponent) : IMGUI_WINDOW("Game Component Properties"), GAMECOMPONENT(gamecomponent) {}
	void GameComponentProperties_Window::Run_Window() {
		if (!Is_Window_Open) {
			IMGUI_DELETEWINDOW(this);
			return;
		}
		if (!IMGUI->Create_Window(Window_Name, Is_Window_Open, false)) {
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
		String ComponentName = "Component Name: ";
		ComponentName.append(COMP->NAME);
		IMGUI->Text(ComponentName);
		String ModelName = "Model Name: ";
		ModelName.append(COMP->MODEL->NAME);
		IMGUI->Text(ModelName);
		if (IMGUI->Begin_Tree("Material Instances")) {
			for (unsigned int i = 0; i < COMP->MATERIALs.size(); i++) {
				GFX_API::Material_Instance* MatInst = COMP->MATERIALs[i];
				if (IMGUI->Begin_Tree("number->string isn't supported yet!")) {
					IMGUI->Text(MatInst->NAME);
					IMGUI->End_Tree();
				}
			}
			IMGUI->End_Tree();
		}

	}
	void Show_CameraComp_Properties(TuranAPI::GameComponent* Component) {
		Camera_Component* COMP = (Camera_Component*)Component;
		String ComponentName = "Component Name: ";
		ComponentName.append(COMP->NAME);
		IMGUI->Text(ComponentName);
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