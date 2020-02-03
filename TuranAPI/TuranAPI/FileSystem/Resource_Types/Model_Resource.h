#pragma once
#include "TuranAPI/API_includes.h"
#include "Resource_Type.h"

namespace TuranAPI {
	namespace File_System {
		//Non-animated mesh data!
		//Stores vertex attribute pointers!
		struct TURANAPI Static_Mesh_Data {
		public:
			~Static_Mesh_Data();

			vec3* POSITIONs = nullptr, * NORMALs = nullptr, * TANGENTs = nullptr, * BITANGENTs = nullptr;
			unsigned int* INDICEs = nullptr;
			vec2* TEXTCOORDs = nullptr;
			int VERTEX_NUMBER = 0, INDICEs_LENGTH = 0;
			void* GFXI_MESH = nullptr;
			unsigned int Material_Index = 0;

			const vec3* Get_Positions() const;
			const unsigned int* Get_Indices() const;
			const vec3* Get_Normals() const;
			const vec2* Get_TextCoords() const;
			const vec3* Get_Tangents() const;
			const vec3* Get_Bitangents() const;
			int Get_Vertex_Number() const;
			int Get_Indice_Number() const;
			void Set_GFXI_Mesh(void* gpu_mesh);
			bool Verify_Mesh_Data();
		};


		//Non-animated model data!
		//Stores only pointers for the contained Meshes' vertex attribute datas!
		class TURANAPI Static_Model_Data : public Resource_Type {
		public:
			//Meshes will be stored as pointers in an array, so point to that "Pointer Array"
			Static_Mesh_Data** MESH_ARRAY_PTR = nullptr;
			unsigned int MESH_NUMBER = 0;

			static vector<Static_Model_Data*> ALL_MODEL_DATAs;
			static Static_Model_Data* Find_Model_byID(unsigned int ID);

			unsigned int Get_Mesh_Number();
			Static_Mesh_Data* Get_Mesh_byIndex(unsigned int index);

			virtual TuranAPI::TuranAPI_ENUMs Get_Resource_Type() override;
			virtual bool Verify_Resource_Data() override;
			Static_Model_Data(Static_Mesh_Data** mesh_array, unsigned int mesh_number, unsigned int id, string name);
			void Set_ID_and_NAME(unsigned int id, string name);
			Static_Model_Data();
		};
	}
}