#pragma once
#include "GFXSource/GFX_Includes.h"
#include "GFXSource/GFX_ENUMs.h"
#include "TuranAPI/FileSystem/Resource_Types/Resource_Type.h"

enum class RESOURCETYPEs : char;

namespace GFX_API {
	struct GFXAPI Material_Uniform {
		String VARIABLE_NAME;
		UNIFORMTYPE VARIABLE_TYPE;
		void* DATA = nullptr;
		Material_Uniform(const char* variable_name, UNIFORMTYPE variable_type);
		bool Verify_UniformData();
		Material_Uniform();
	};



	class GFXAPI ShaderSource : public TuranAPI::Resource_Type {
	public:
		ShaderSource();
		SHADER_STAGE STAGE;
		String SOURCE_CODE;
		SHADER_LANGUAGEs LANGUAGE;

		virtual void Write_ToDisk(bool Verify_Data) override;
		//Loads an asset! If you want to write a override function, first you have to be sure if flatbuffer and Resource_Type is related
		//Giving an unrelated flatbuffer's data doesn't do anyting. 
		virtual void Load_FromDisk(void* flatbuffer_data, unsigned int ID, const char* path, IAllocator* Allocator) override;
		virtual bool Verify_Resource_Data() override;
	};


	class GFXAPI Material_Type_Resource : public TuranAPI::Resource_Type {
	public:
		Material_Type_Resource();


		unsigned int VERTEXSOURCE_ID, FRAGMENTSOURCE_ID;

		Vector<Material_Uniform> UNIFORMs;

		virtual void Write_ToDisk(bool Verify_Data);
		//Loads an asset! If you want to write a override function, first you have to be sure if flatbuffer and Resource_Type is related
		//Giving an unrelated flatbuffer's data doesn't do anyting. 
		virtual void Load_FromDisk(void* flatbuffer_data, unsigned int ID, const char* path, IAllocator* Allocator);
		virtual bool Verify_Resource_Data();
	};


	class GFXAPI Shader_Instance {
	public:
	};
	

	//Don't forget, data handling for each uniform type is the responsibility of the user!
	class GFXAPI Material_Instance : public TuranAPI::Resource_Type {
	public:
		Material_Instance();

		//Uniforms won't change at run-time because we are defining uniforms at compile-time, but it is an easier syntax for now!
		//This list will be defined per material type (for example: Surface_PBR, Surface_Phong, Texture View etc.)
		Material_Type_Resource* Material_Type;
		Vector<Material_Uniform> UNIFORM_LIST;

		Shader_Instance* GPU_REPRESENTATION;

		virtual bool Verify_Resource_Data() override;
		virtual void Write_ToDisk(bool Verify_Data) override;
		//Loads an asset! If you want to write a override function, first you have to be sure if flatbuffer and Resource_Type is related
		//Giving an unrelated flatbuffer's data doesn't do anyting. 
		virtual void Load_FromDisk(void* flatbuffer_data, unsigned int ID, const char* path, IAllocator* Allocator) override;
		unsigned int Find_Uniform_byName(const char* uniform_name);
	public:
		void Set_Uniform_Data(const char* uniform_name, void* pointer_to_data);
	};
}
