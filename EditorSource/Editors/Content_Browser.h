#pragma once
#include "EditorSource/Editor_Includes.h"
#include"GFXSource/IMGUI/IMGUI_WINDOW.h"

namespace TuranEditor {
	class GameContent_Browser : public GFX_API::IMGUI_WINDOW {
		Bitset Asset_CheckList;
		vector<string> item_names;
		bool GameContent_EditMode = false;
	public:
		GameContent_Browser();
		virtual ~GameContent_Browser();
		virtual void Run_Window();
	};

}