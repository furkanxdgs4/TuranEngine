#include "Properties_Window.h"
#include "TuranAPI/API_IMGUI.h"
#include "TuranAPI/API_FileSystem.h"

using namespace TuranAPI::Game_Object;
using namespace TuranAPI::IMGUI;
using namespace TuranAPI::File_System;

ResourceProperties_Window::ResourceProperties_Window(TuranAPI::File_System::Resource_Type* resource) : IMGUI_WINDOW(resource->NAME) {
	RESOURCE = resource;
}

vector<string> UNIFORM_VAR_TYPE_NAMEs = { "Unsigned Integer 32-bit", "Integer 32-bit", "Float 32-bit", "Vec2 (2 float)", "Vec3 (3 float)", "Vec4 (4 float)", "Matrix 4x4", "Texture 2D" };
string Find_UNIFORM_VARTYPE_Name(TuranAPI::TuranAPI_ENUMs uniform_var_type);
void Show_MaterialType_Properties(TuranAPI::File_System::Resource_Type* resource);
void Show_Model_Properties(TuranAPI::File_System::Resource_Type* resource);
void Show_MaterialInstance_Properties(TuranAPI::File_System::Resource_Type* resource);
void Show_Texture_Properties(TuranAPI::File_System::Resource_Type* resource);

void ResourceProperties_Window::Run_Window() {
	if (!Is_Window_Open) {
		delete this;
		return;
	}
	if (!IMGUI::Create_Window(Window_Name, Is_Window_Open, false)) {
		IMGUI::End_Window();
		return;
	}
	switch (RESOURCE->Get_Resource_Type()) {
	case TuranAPI::TuranAPI_ENUMs::MATERIAL_TYPE_RESOURCE:
		Show_MaterialType_Properties(RESOURCE);
		break;
	case TuranAPI::TuranAPI_ENUMs::TEXTURE_RESOURCE:
		Show_Texture_Properties(RESOURCE);
		break;
	case TuranAPI::TuranAPI_ENUMs::MATERIAL_INSTANCE_RESOURCE:
		Show_MaterialInstance_Properties(RESOURCE);
		break;
	case TuranAPI::TuranAPI_ENUMs::STATIC_MODEL_RESOURCE:
		Show_Model_Properties(RESOURCE);
		break;
	default:
		cout << "This type's properties can't be shown by Properties Window!\n";
		this_thread::sleep_for(chrono::seconds(10));
	}
	IMGUI::End_Window();
}

void Show_MaterialType_Properties(TuranAPI::File_System::Resource_Type* resource) {
	TuranAPI::File_System::Material_Type_Resource* material_type_resource = (TuranAPI::File_System::Material_Type_Resource*)resource;
	
	IMGUI::Text("Material Type Name: " + material_type_resource->NAME);
	IMGUI::Text("Vertex Source:");
	IMGUI::Text(material_type_resource->VERTEX_SOURCE);
	IMGUI::Text("Fragment Source:");
	IMGUI::Text(material_type_resource->FRAGMENT_SOURCE + "\n\n");


	if (IMGUI::IMGUI::Begin_Tree("Uniform List")) {
		for (unsigned int i = 0; i < material_type_resource->UNIFORMs.size(); i++) {
			TuranAPI::File_System::Material_Uniform* UNIFORM = &material_type_resource->UNIFORMs[i];

			if (IMGUI::IMGUI::Begin_Tree(to_string(i))) {
				IMGUI::IMGUI::Text("Uniform Name: " + UNIFORM->VARIABLE_NAME);
				IMGUI::IMGUI::Text("Uniform Variable Type: " + Find_UNIFORM_VARTYPE_Name(UNIFORM->VARIABLE_TYPE));
				IMGUI::IMGUI::End_Tree();
			}
		}
		IMGUI::IMGUI::End_Tree();
	}
}

void Show_MaterialInstance_Properties(TuranAPI::File_System::Resource_Type* resource) {
	TuranAPI::File_System::Material_Instance* material_instance_resource = (TuranAPI::File_System::Material_Instance*)resource;

	IMGUI::Text("Material Instance Name: " + material_instance_resource->NAME);
	IMGUI::Text("Material Type Name: " + material_instance_resource->Material_Type->NAME);
	IMGUI::Text("Vertex Source:");
	IMGUI::Text(material_instance_resource->Material_Type->VERTEX_SOURCE);
	IMGUI::Text("Fragment Source:");
	IMGUI::Text(material_instance_resource->Material_Type->FRAGMENT_SOURCE + "\n\n");


	if (IMGUI::IMGUI::Begin_Tree("Uniform List")) {
		for (unsigned int i = 0; i < material_instance_resource->UNIFORM_LIST.size(); i++) {
			TuranAPI::File_System::Material_Uniform* UNIFORM = &material_instance_resource->UNIFORM_LIST[i];

			if (IMGUI::IMGUI::Begin_Tree(to_string(i))) {
				IMGUI::IMGUI::Text("Uniform Name: " + UNIFORM->VARIABLE_NAME);
				IMGUI::IMGUI::Text("Uniform Variable Type: " + Find_UNIFORM_VARTYPE_Name(UNIFORM->VARIABLE_TYPE));
				if (UNIFORM->VARIABLE_TYPE == TuranAPI::TuranAPI_ENUMs::API_TEXTURE_2D
					&& UNIFORM->DATA) {
						IMGUI::IMGUI::Text("Texture ID: " + *(unsigned int*)UNIFORM->DATA);
				}
				IMGUI::IMGUI::End_Tree();
			}
		}
		IMGUI::IMGUI::End_Tree();
	}
}


void Show_Texture_Properties(TuranAPI::File_System::Resource_Type* resource) {
	TuranAPI::File_System::Texture_Resource* TEXTURE = (TuranAPI::File_System::Texture_Resource*)resource;
	IMGUI::Display_Texture(&TEXTURE->GL_ID, 1024, 1024);
}

void Show_Model_Properties(TuranAPI::File_System::Resource_Type* resource) {
	TuranAPI::File_System::Static_Model_Data* model_data_resource = (TuranAPI::File_System::Static_Model_Data*)resource;

	IMGUI::Text("Model Name: " + model_data_resource->NAME);
	IMGUI::Text("Mesh Number: " + to_string(model_data_resource->Get_Mesh_Number()));
}

string Find_UNIFORM_VARTYPE_Name(TuranAPI::TuranAPI_ENUMs uniform_var_type) {
	switch (uniform_var_type) {
	case TuranAPI::TuranAPI_ENUMs::VAR_UINT32:
		return UNIFORM_VAR_TYPE_NAMEs[0];
	case TuranAPI::TuranAPI_ENUMs::VAR_INT32:
		return UNIFORM_VAR_TYPE_NAMEs[1];
	case TuranAPI::TuranAPI_ENUMs::VAR_FLOAT32:
		return UNIFORM_VAR_TYPE_NAMEs[2];
	case TuranAPI::TuranAPI_ENUMs::VAR_VEC2:
		return UNIFORM_VAR_TYPE_NAMEs[3];
	case TuranAPI::TuranAPI_ENUMs::VAR_VEC3:
		return UNIFORM_VAR_TYPE_NAMEs[4];
	case TuranAPI::TuranAPI_ENUMs::VAR_VEC4:
		return UNIFORM_VAR_TYPE_NAMEs[5];
	case TuranAPI::TuranAPI_ENUMs::VAR_MAT4x4:
		return UNIFORM_VAR_TYPE_NAMEs[6];
	case TuranAPI::TuranAPI_ENUMs::API_TEXTURE_2D:
		return UNIFORM_VAR_TYPE_NAMEs[7];
	default:
		return "Error, Uniform_Var_Type isn't supported by Find_UNIFORM_VARTYPE_Name!\n";
	}
}





void Show_StaticModelComp_Properties(TuranAPI::Game_Object::GameComponent* Component);
void Show_CameraComp_Properties(TuranAPI::Game_Object::GameComponent* Component);

GameComponentProperties_Window::GameComponentProperties_Window(TuranAPI::Game_Object::GameComponent* gamecomponent) : IMGUI_WINDOW("Game Component Properties"), GAMECOMPONENT(gamecomponent) {}
void GameComponentProperties_Window::Run_Window() {
	if (!Is_Window_Open) {
		delete this;
		return;
	}
	if (!IMGUI::Create_Window(Window_Name, Is_Window_Open, false)) {
		IMGUI::End_Window();
		return;
	}
	switch (GAMECOMPONENT->Get_Component_Type()) {
	case TuranAPI::TuranAPI_ENUMs::STATIC_MODEL_COMP:
		Show_StaticModelComp_Properties(GAMECOMPONENT);
		break;
	case TuranAPI::TuranAPI_ENUMs::CAMERA_COMP:
		Show_CameraComp_Properties(GAMECOMPONENT);
		break;
	default:
		cout << "ERROR: Intended Component Type isn't supported by Properties Window!\n";
		TuranAPI::Breakpoint();
	}


	IMGUI::End_Window();
}

void Show_StaticModelComp_Properties(TuranAPI::Game_Object::GameComponent* Component) {
	StaticModel_Component* COMP = (StaticModel_Component*)Component;
	IMGUI::Text("Component Name: " + COMP->NAME);
	IMGUI::Text("Model Name: " + COMP->MODEL->NAME);
	if (IMGUI::Begin_Tree("Material Instances")) {
		for (unsigned int i = 0; i < COMP->MATERIALs.size(); i++) {
			Material_Instance* MatInst = COMP->MATERIALs[i];
			if (IMGUI::Begin_Tree(to_string(i))) {
				IMGUI::Text(MatInst->NAME);
				IMGUI::End_Tree();
			}
		}
		IMGUI::End_Tree();
	}

}
void Show_CameraComp_Properties(TuranAPI::Game_Object::GameComponent* Component) {
	
	Camera_Component* COMP = (Camera_Component*)Component;
	IMGUI::Text("Component Name: " + COMP->NAME);
	vec3 Position = COMP->Get_Position();
	vec3 Target = COMP->Get_Target();
	int FOV_inAngle = COMP->Get_FOV_inAngle();
	vec2 Aspect_Width_and_Height = COMP->Get_Aspect_Width_and_Height();
	vec2 Near_and_FarPlane = COMP->Get_Near_and_FarPlane();

	if (IMGUI::Slider_Vec3("Position", &Position, -1000, 1000)) {
		COMP->Translate(Position);
	}
	if (IMGUI::Slider_Vec3("Target", &Target, -1000, 1000)) {
		COMP->Set_Camera_Target(Target);
	}
	if (IMGUI::Slider_Int("FOV in angle", &FOV_inAngle, 0, 179)) {
		COMP->Set_Camera_Properties(FOV_inAngle, Aspect_Width_and_Height.x, Aspect_Width_and_Height.y, Near_and_FarPlane.x, Near_and_FarPlane.y);
	}
	if (IMGUI::Slider_Vec2("Aspect Width and Height", &Aspect_Width_and_Height, 0, 2000)) {
		COMP->Set_Camera_Properties(FOV_inAngle, Aspect_Width_and_Height.x, Aspect_Width_and_Height.y, Near_and_FarPlane.x, Near_and_FarPlane.y);
	}
	if (IMGUI::Slider_Vec2("Near and Far Plane", &Near_and_FarPlane, 0, 100000)) {
		COMP->Set_Camera_Properties(FOV_inAngle, Aspect_Width_and_Height.x, Aspect_Width_and_Height.y, Near_and_FarPlane.x, Near_and_FarPlane.y);
	}
}