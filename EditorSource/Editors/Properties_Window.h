#pragma once
#include "EditorSource/Editor_Includes.h"
#include "GFXSource/IMGUI/IMGUI_WINDOW.h"
#include "TuranAPI/FileSystem/GameObjects/GameComponents/GameComponent.h"
#include "EditorSource/FileSystem/ResourceTypes/Resource_Identifier.h"

namespace TuranEditor {
	class ResourceProperties_Window : public GFX_API::IMGUI_WINDOW {
	public:
		Resource_Identifier* RESOURCE = nullptr;
		GFX_API::RenderGraph* RenderGraph = nullptr;

		ResourceProperties_Window(Resource_Identifier* resource);
		virtual void Run_Window();
	};

	class GameComponentProperties_Window : public GFX_API::IMGUI_WINDOW {
		TuranAPI::GameComponent* GAMECOMPONENT;
	public:
		GameComponentProperties_Window(TuranAPI::GameComponent* gamecomponent);
		virtual void Run_Window() override;
	};
}