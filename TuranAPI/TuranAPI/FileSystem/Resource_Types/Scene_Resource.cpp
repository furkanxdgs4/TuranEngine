#include "Scene_Resource.h"
using namespace TuranAPI::File_System;


vector<Scene_Resource*> Scene_Resource::ALL_SCENEs = vector<Scene_Resource*>{};

TuranAPI::TuranAPI_ENUMs Scene_Resource::Get_Resource_Type() {
	return TuranAPI::TuranAPI_ENUMs::SCENE_RESOURCE;
}


bool Scene_Resource::Verify_Resource_Data() {
	if (NAME != "" && PATH != "") {
		for (Game_Object::GameComponent* COMPONENT : ADDED_COMPONENTs) {
			if (COMPONENT == nullptr) {
				TuranAPI::Breakpoint("Scene Resource isn't verified because a Game Component is nullptr!");
				return false;
			}
		}
		return true;
	}
	TuranAPI::Breakpoint("Scene Resource isn't verified because it has invalid Name or Path!\n"
		+ string("NAME: ") + NAME + string("\n PATH: ") + PATH + string("\n"));
	return false;
}

Scene_Resource::Scene_Resource() {
	ALL_SCENEs.push_back(this);
}