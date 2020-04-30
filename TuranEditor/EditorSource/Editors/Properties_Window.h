#pragma once
#include "EditorSource/Editor_Includes.h"
#include "GFXSource/IMGUI/IMGUI_WINDOW.h"
#include "TuranAPI/FileSystem/Resource_Types/Resource_Type.h"
#include "TuranAPI/FileSystem/GameObjects/GameComponents/GameComponent.h"

namespace TuranEditor {
	class ResourceProperties_Window : public GFX_API::IMGUI_WINDOW {
	public:
		TuranAPI::Resource_Type* RESOURCE = nullptr;
		GFX_API::RenderGraph* RenderGraph = nullptr;

		ResourceProperties_Window(TuranAPI::Resource_Type* resource);
		virtual void Run_Window();
	};

	class GameComponentProperties_Window : public GFX_API::IMGUI_WINDOW {
		TuranAPI::GameComponent* GAMECOMPONENT;
	public:
		GameComponentProperties_Window(TuranAPI::GameComponent* gamecomponent);
		virtual void Run_Window() override;
	};
}