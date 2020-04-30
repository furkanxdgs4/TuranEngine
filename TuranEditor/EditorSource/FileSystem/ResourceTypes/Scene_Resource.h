#pragma once
#include "TuranAPI/API_includes.h"
#include "TuranAPI/FileSystem/Resource_Types/Resource_Type.h"
#include "TuranAPI/FileSystem/GameObjects/GameComponents/GameComponent.h"

enum class RESOURCETYPEs : char;

namespace TuranEditor {
	class Scene_Resource : public TuranAPI::Resource_Type {
	public:
		Scene_Resource();
		virtual bool Verify_Resource_Data();
		Vector<TuranAPI::GameComponent*> ADDED_COMPONENTs;
		virtual void Write_ToDisk(bool Verify_Data);
		//Loads an asset! If you want to write a override function, first you have to be sure if flatbuffer and Resource_Type is related
		//Giving an unrelated flatbuffer's data doesn't do anyting. 
		virtual void Load_FromDisk(void* flatbuffer_data, unsigned int ID, const char* path, IAllocator* Allocator);
	};
}