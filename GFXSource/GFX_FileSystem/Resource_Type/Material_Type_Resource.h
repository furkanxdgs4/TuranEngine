#pragma once
#include "GFXSource/GFX_Includes.h"
#include "GFXSource/GFX_ENUMs.h"

enum class RESOURCETYPEs : char;

namespace GFX_API {
	struct GFXAPI Material_Uniform {
		string VARIABLE_NAME;
		UNIFORMTYPE VARIABLE_TYPE;
		void* DATA = nullptr;
		Material_Uniform(const char* variable_name, UNIFORMTYPE variable_type);
		bool Verify_UniformData();
		Material_Uniform();
	};



	class GFXAPI ShaderSource_Resource {
	public:
		ShaderSource_Resource();
		SHADER_STAGE STAGE;
		string SOURCE_CODE;
		SHADER_LANGUAGEs LANGUAGE;
	};


	class GFXAPI Material_Type {
	public:
		Material_Type();


		unsigned int VERTEXSOURCE_ID, FRAGMENTSOURCE_ID;

		vector<Material_Uniform> UNIFORMs;
	};
	

	//Don't forget, data handling for each uniform type is the responsibility of the user!
	class GFXAPI Material_Instance {
	public:
		Material_Instance();

		//Uniforms won't change at run-time because we are defining uniforms at compile-time, but it is an easier syntax for now!
		//This list will be defined per material type (for example: Surface_PBR, Surface_Phong, Texture View etc.)
		unsigned int Material_Type;
		vector<Material_Uniform> UNIFORM_LIST;

		unsigned int Find_Uniform_byName(const char* uniform_name);
	public:
		void Set_Uniform_Data(const char* uniform_name, void* pointer_to_data);
	};
}
