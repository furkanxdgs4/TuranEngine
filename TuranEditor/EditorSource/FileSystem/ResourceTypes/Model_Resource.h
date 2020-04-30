#pragma once
#include "EditorSource/Editor_Includes.h"
#include "TuranAPI/FileSystem/Resource_Types/Resource_Type.h"

enum class RESOURCETYPEs : char;

namespace TuranEditor {
	struct Static_Mesh_Data {
	public:
		Static_Mesh_Data(unsigned char Attribute_Number);
		~Static_Mesh_Data();

		void* DATA;
		unsigned int DATA_SIZE, VERTEX_NUMBER = 0;
		unsigned int Material_Index = 0;

		GFX_API::VertexAttributeLayout DataLayout;

		bool Verify_Mesh_Data();
	};


	class Static_Model_Data : public TuranAPI::Resource_Type {
	public:
		Static_Model_Data(size_t Numberof_Meshes);

		//Meshes will be stored as pointers in an array, so point to that "Pointer Array"
		Vector<Static_Mesh_Data*> MESHes;
		unsigned char Material_Number;

		virtual bool Verify_Resource_Data() override;
		virtual void Write_ToDisk(bool Verify_Data);
		//Loads an asset! If you want to write a override function, first you have to be sure if flatbuffer and Resource_Type is related
		//Giving an unrelated flatbuffer's data doesn't do anyting. 
		virtual void Load_FromDisk(void* flatbuffer_data, unsigned int ID, const char* path, IAllocator* Allocator);
		//Return Vector contains MeshBufferIDs of the Model!
		Vector<unsigned int> Upload_toGPU();
	};
}