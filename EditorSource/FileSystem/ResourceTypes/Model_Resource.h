#pragma once
#include "EditorSource/Editor_Includes.h"

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


	class Static_Model {
	public:
		Static_Model();

		//Meshes will be stored as pointers in an array, so point to that "Pointer Array"
		vector<Static_Mesh_Data*> MESHes;
		unsigned char Material_Number;

		//Return vector contains MeshBufferIDs of the Model!
		vector<unsigned int> Upload_toGPU();
	};
}