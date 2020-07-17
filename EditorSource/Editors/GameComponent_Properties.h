#pragma once
#include "EditorSource/Editor_Includes.h"
#include "GFXSource/IMGUI/IMGUI_WINDOW.h"
#include "TuranAPI/FileSystem/GameObjects/GameComponents/GameComponent.h"
#include "EditorSource/FileSystem/ResourceTypes/Resource_Identifier.h"


namespace TuranEditor {
	class GameComponentProperties_Window : public GFX_API::IMGUI_WINDOW {
		TuranAPI::GameComponent* GAMECOMPONENT;
		Resource_Identifier* SCENE;
		bool SaveComponent;
	public:
		GameComponentProperties_Window(Resource_Identifier* scene, TuranAPI::GameComponent* gamecomponent);
		virtual void Run_Window() override;

		void Show_StaticModelComp_Properties();
		void Show_CameraComp_Properties();
	};
}