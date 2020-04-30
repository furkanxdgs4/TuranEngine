#pragma once
#include "TuranAPI/API_includes.h"

//Resource Types to present data in API!
#include "Resource_Types/Resource_Type.h"

/*
	1) This class is to add data types to Engine!
	2) An added data type should be type of .gamecont or .enginecont
*/
namespace TuranAPI {

	/*
	Handles basic IO operations and provides an interface for writing a File_System in a DLL
	Because there is a lack of Data Model, every filesystem should handle its own ID generation
	So that means, there may some collisions between assets because of FileSystems' ID generation systems.
	*/
	class TURANAPI FileSystem {
	public:
		static void* Read_BinaryFile(const char* path, unsigned int& size, IAllocator* allocator);
		static void Write_BinaryFile(const char* path, void* data, unsigned int size);
		static void Overwrite_BinaryFile(const char* path, void* data, unsigned int size);
		static void Delete_File(const char* path);


		static String* Read_TextFile(const char* path, TuranAPI::MemoryManagement::IAllocator* Allocator);
		static void Write_TextFile(const char* text, const char* path, bool write_to_end);
		static void Write_TextFile(const String* text, const char* path, bool write_to_end);
	};
}
#define TAPIFILESYSTEM TuranAPI::FileSystem