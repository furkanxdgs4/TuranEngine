#pragma once
#include "TuranAPI/API_includes.h"
#include "Resource_Type.h"
#include "Model_Resource.h"

namespace TuranAPI {
	namespace File_System {

		struct TURANAPI Material_Uniform {
			string VARIABLE_NAME = "";
			TuranAPI_ENUMs VARIABLE_TYPE;
			void* DATA = nullptr;
			Material_Uniform(string variable_name, TuranAPI_ENUMs variable_type);
			bool Verify_UniformData();
			Material_Uniform();
		};



		class TURANAPI Material_Type_Resource : public Resource_Type {
		public:
			Material_Type_Resource();
			virtual TuranAPI::TuranAPI_ENUMs Get_Resource_Type();
			virtual bool Verify_Resource_Data();

			static vector<Material_Type_Resource*> ALL_MATERIAL_TYPEs;

			//The GFX_API, Vertex and Fragment sources are written with.
			TuranAPI_ENUMs GFX_API;
			unsigned int PROGRAM_ID, VERTEX_ID, FRAGMENT_ID;
			string VERTEX_SOURCE, FRAGMENT_SOURCE;
			vector<TuranAPI::File_System::Material_Uniform> UNIFORMs;
		};





		//Don't forget, data handling for each uniform type is the responsibility of the user!
		class TURANAPI Material_Instance : public Resource_Type {
		public:
			Material_Instance();

			//ALL Material Instances!
			static vector<Material_Instance*> ALL_Material_Instances;
			static Material_Instance* Find_MaterialInstance_byID(unsigned int ID);
			static Material_Instance* Find_MaterialInstance_byName(string name_of_resource);



			Material_Type_Resource* Material_Type;

			//Uniforms won't change at run-time because we are defining uniforms at compile-time, but it is an easier syntax for now!
			//This list will be defined per material type (for example: Surface_PBR, Surface_Phong, Texture View etc.)
			vector<Material_Uniform> UNIFORM_LIST;
			

			virtual TuranAPI_ENUMs Get_Resource_Type();
			virtual bool Verify_Resource_Data();
			unsigned int Find_Uniform_byName(string uniform_name);
		public:
			void Set_Uniform_Data(string uniform_name, void* pointer_to_data);
		};
	}
}
