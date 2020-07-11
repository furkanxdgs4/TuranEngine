#pragma once
#include "TuranAPI/API_includes.h"
#include "TuranAPI/FileSystem/GameObjects/GameComponents/GameComponent.h"

enum class RESOURCETYPEs : char;

namespace TuranEditor {
	class Scene_Resource {
	public:
		Scene_Resource();
		vector<TuranAPI::GameComponent*> ADDED_COMPONENTs;
	};
}