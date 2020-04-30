#pragma once
#include "TuranAPI/API_includes.h"

enum class RESOURCETYPEs : char;

namespace TuranAPI {
	//Each resource is inherited from this!
	//For FileList resources, ID is 0!
	class TURANAPI Resource_Type
	{
	protected:
		friend class FileSystem;
		enum class RESOURCETYPEs TYPE;
	public:
		Resource_Type(enum class RESOURCETYPEs type);
		String PATH;
		unsigned int ID;
		//Name to show in Content Browser
		String NAME;

		enum class RESOURCETYPEs Get_Type();

		virtual void Write_ToDisk(bool Verify_Data) = 0;
		//Loads an asset! If you want to write a override function, first you have to be sure if flatbuffer and Resource_Type is related
		//Giving an unrelated flatbuffer's data doesn't do anyting. 
		virtual void Load_FromDisk(void* flatbuffer_data, unsigned int ID, const char* path, IAllocator* Allocator) = 0;
		virtual bool Verify_Resource_Data() = 0;
	};
}