#include "GameComponent_Properties.h"
#include "TuranAPI/FileSystem/GameObjects/GameComponents/GameComponentTypes.h"
#include "EditorSource/FileSystem/GameObjects/Camera_Component.h"
#include "EditorSource/FileSystem/GameObjects/StaticModel_Component.h"
#include "EditorSource/FileSystem/EditorFileSystem_Core.h"

namespace TuranEditor {
	GameComponentProperties_Window::GameComponentProperties_Window(Resource_Identifier* scene, TuranAPI::GameComponent* gamecomponent) :
		IMGUI_WINDOW("Game Component Properties"), GAMECOMPONENT(gamecomponent), SCENE(scene) {
		IMGUI_REGISTERWINDOW(this);
	}
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
			Show_StaticModelComp_Properties();
			break;
		case GameComponentType::CAMERA_C:
			Show_CameraComp_Properties();
			break;
		default:
			std::cout << "ERROR: Intended Component Type isn't supported by Properties Window!\n";
			TuranAPI::Breakpoint();
		}


		IMGUI->End_Window();
	}

	void GameComponentProperties_Window::Show_StaticModelComp_Properties() {
		StaticModel_Component* COMP = (StaticModel_Component*)GAMECOMPONENT;
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
	void GameComponentProperties_Window::Show_CameraComp_Properties() {
		Camera_Component* COMP = (Camera_Component*)GAMECOMPONENT;
		string ComponentName = "Component Name: ";
		ComponentName.append(COMP->NAME);
		IMGUI->Text(ComponentName.c_str());
		vec3 Position = COMP->Get_Position();
		vec3 Target = COMP->Get_Target();
		int FOV_inAngle = COMP->Get_FOV_inAngle();
		vec2 Aspect_Width_and_Height = COMP->Get_Aspect_Width_and_Height();
		vec2 Near_and_FarPlane = COMP->Get_Near_and_FarPlane();

		if (IMGUI->Slider_Vec3("Position", &Position, -1000, 1000)) {
			COMP->Translate(Position);	SaveComponent = true;
		}
		if (IMGUI->Slider_Vec3("Target", &Target, -1000, 1000)) {
			COMP->Set_Camera_Target(Target);	SaveComponent = true;
		}
		if (IMGUI->Slider_Int("FOV in angle", &FOV_inAngle, 0, 179)) {
			COMP->Set_Camera_Properties(FOV_inAngle, Aspect_Width_and_Height.x, Aspect_Width_and_Height.y, Near_and_FarPlane.x, Near_and_FarPlane.y);
			SaveComponent = true;
		}
		if (IMGUI->Slider_Vec2("Aspect Width and Height", &Aspect_Width_and_Height, 0, 2000)) {
			COMP->Set_Camera_Properties(FOV_inAngle, Aspect_Width_and_Height.x, Aspect_Width_and_Height.y, Near_and_FarPlane.x, Near_and_FarPlane.y);
			SaveComponent = true;
		}
		if (IMGUI->Slider_Vec2("Near and Far Plane", &Near_and_FarPlane, 0, 100000)) {
			COMP->Set_Camera_Properties(FOV_inAngle, Aspect_Width_and_Height.x, Aspect_Width_and_Height.y, Near_and_FarPlane.x, Near_and_FarPlane.y);
			SaveComponent = true;
		}
		if (SaveComponent) {
			if (IMGUI->Button("Save")) {
				EDITOR_FILESYSTEM->Save_Resource(SCENE->ID);
				SaveComponent = false;
			}
		}
	}

}