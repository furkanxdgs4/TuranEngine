#include "Material_Type_Resource.h"
#include "GFXSource/GFX_FileSystem/DataFormats/GFXAPI_Resources_generated.h"
#include "TuranAPI/FileSystem/Resource_Types/ResourceTYPEs.h"

namespace GFX_API {

	GFXAsset::SHADER_LANGUAGES ConvertShaderLanguage_toFB(SHADER_LANGUAGEs language) {
		switch (language) {
		case SHADER_LANGUAGEs::GLSL:
			return GFXAsset::SHADER_LANGUAGES::SHADER_LANGUAGES_GLSL;
		default:
			TuranAPI::LOG_CRASHING("Material Type's GFX_API can't be converted to File_List::GFX_APIs! OpenGL 3 is returned!");
			return GFXAsset::SHADER_LANGUAGES::SHADER_LANGUAGES_TSL;
		}
	}
	SHADER_LANGUAGEs GetShaderLanguage_fromFB(GFXAsset::SHADER_LANGUAGES language) {
		switch (language) {
		case GFXAsset::SHADER_LANGUAGES::SHADER_LANGUAGES_GLSL:
			return SHADER_LANGUAGEs::GLSL;
		default:
			TuranAPI::Breakpoint("Material Type's SHADER_LANGUAGE type can't be converted to in engine type!");
		}
	}
	SHADER_STAGE GetShaderStageType_fromFB(GFXAsset::SHADERSTAGEs stagetype) {
		switch (stagetype) {
		case GFXAsset::SHADERSTAGEs::SHADERSTAGEs_VERTEX_STAGE:
			return SHADER_STAGE::VERTEXSTAGE;
		case GFXAsset::SHADERSTAGEs::SHADERSTAGEs_FRAGMENT_STAGE:
			return SHADER_STAGE::FRAGMENTSTAGE;
		case GFXAsset::SHADERSTAGEs::SHADERSTAGEs_COMPUTE:
			return SHADER_STAGE::COMPUTE;
		default:
			TuranAPI::Breakpoint("Shader Source's Shader Stage type can't be converted to in engine type!");
		}
	}
	GFXAsset::SHADERSTAGEs ConvertShaderStageType_toFB(SHADER_STAGE stagetype) {
		switch (stagetype) {
		case SHADER_STAGE::VERTEXSTAGE:
			return GFXAsset::SHADERSTAGEs::SHADERSTAGEs_VERTEX_STAGE;
		case SHADER_STAGE::FRAGMENTSTAGE:
			return GFXAsset::SHADERSTAGEs::SHADERSTAGEs_FRAGMENT_STAGE;
		case SHADER_STAGE::COMPUTE:
			return GFXAsset::SHADERSTAGEs::SHADERSTAGEs_COMPUTE;
		default:
			TuranAPI::Breakpoint("Shader Source's Shader Stage type can't be converted to Flatbuffer type!");
		}
	}
	Material_Type_Resource* Find_MaterialType_byID(unsigned int ID) {
		/*
		for (unsigned int i = 0; i < Material_Type_Resource::ALL_MATERIALTYPEs.size(); i++) {
			Material_Type_Resource* material_type = Material_Type_Resource::ALL_MATERIALTYPEs[i];
			if (material_type->Get_ID() == ID) {
				return material_type;
			}
		}*/
		return nullptr;
	}
	UNIFORMTYPE Convert_to_TuranAPIEnums_UniformVarType(GFXAsset::MATERIAL_INSTANCE_UNIFORM uniform_var_type) {
		switch (uniform_var_type) {
		case GFXAsset::MATERIAL_INSTANCE_UNIFORM_UINT32:
			return UNIFORMTYPE::VAR_UINT32;
		case GFXAsset::MATERIAL_INSTANCE_UNIFORM_INT32:
			return UNIFORMTYPE::VAR_INT32;
		case GFXAsset::MATERIAL_INSTANCE_UNIFORM_FLOAT:
			return UNIFORMTYPE::VAR_FLOAT32;
		case GFXAsset::MATERIAL_INSTANCE_UNIFORM_VEC2:
			return UNIFORMTYPE::VAR_VEC2;
		case GFXAsset::MATERIAL_INSTANCE_UNIFORM_VEC3:
			return UNIFORMTYPE::VAR_VEC3;
		case GFXAsset::MATERIAL_INSTANCE_UNIFORM_VEC4:
			return UNIFORMTYPE::VAR_VEC4;
		case GFXAsset::MATERIAL_INSTANCE_UNIFORM_Mat4x4:
			return UNIFORMTYPE::VAR_MAT4x4;
		case GFXAsset::MATERIAL_INSTANCE_UNIFORM_TEXTURE2D:
			return UNIFORMTYPE::TEXTURE2D;
		default:
			std::cout << "Error: Uniform Var type couldn't be converted to TuranAPIEnums in Convert_to_TuranAPIEnums_UniformVarType() in Load_Material_Type.cpp!\n";
			SLEEP_THREAD(10);
		}
	}
	String Get_UniformName_fromFlatbuffer(const GFXAsset::MATERIAL_INSTANCE_UNIFORM_TABLE* flatbuffer_uniform, UNIFORMTYPE uniform_var_type) {
		switch (uniform_var_type) {
		case UNIFORMTYPE::VAR_UINT32:
			return flatbuffer_uniform->UNIFORM_as_UINT32()->NAME()->str().c_str();
		case UNIFORMTYPE::VAR_INT32:
			return flatbuffer_uniform->UNIFORM_as_INT32()->NAME()->str().c_str();
		case UNIFORMTYPE::VAR_FLOAT32:
			return flatbuffer_uniform->UNIFORM_as_FLOAT()->NAME()->str().c_str();
		case UNIFORMTYPE::VAR_VEC2:
			return flatbuffer_uniform->UNIFORM_as_VEC2()->NAME()->str().c_str();
		case UNIFORMTYPE::VAR_VEC3:
			return flatbuffer_uniform->UNIFORM_as_VEC3()->NAME()->str().c_str();
		case UNIFORMTYPE::VAR_VEC4:
			return flatbuffer_uniform->UNIFORM_as_VEC4()->NAME()->str().c_str();
		case UNIFORMTYPE::VAR_MAT4x4:
			return flatbuffer_uniform->UNIFORM_as_Mat4x4()->NAME()->str().c_str();
		case UNIFORMTYPE::TEXTURE2D:
			return flatbuffer_uniform->UNIFORM_as_TEXTURE2D()->NAME()->str().c_str();
		default:
			std::cout << "Error: Uniform Var Type isn't supported in Get_UniformName_fromFlatbuffer() in Load_Material_Type.cpp!\n";
			SLEEP_THREAD(10);
		}
	}

	GFXAsset::MATERIAL_INSTANCE_UNIFORM Convert_to_Flatbufer_Uniform_Type(UNIFORMTYPE uniform_type) {
		switch (uniform_type) {
		case UNIFORMTYPE::VAR_UINT32:
			return GFXAsset::MATERIAL_INSTANCE_UNIFORM_UINT32;
		case UNIFORMTYPE::VAR_INT32:
			return GFXAsset::MATERIAL_INSTANCE_UNIFORM_INT32;
		case UNIFORMTYPE::VAR_FLOAT32:
			return GFXAsset::MATERIAL_INSTANCE_UNIFORM_FLOAT;
		case UNIFORMTYPE::VAR_VEC2:
			return GFXAsset::MATERIAL_INSTANCE_UNIFORM_VEC2;
		case UNIFORMTYPE::VAR_VEC3:
			return GFXAsset::MATERIAL_INSTANCE_UNIFORM_VEC3;
		case UNIFORMTYPE::VAR_VEC4:
			return GFXAsset::MATERIAL_INSTANCE_UNIFORM_VEC4;
		case UNIFORMTYPE::VAR_MAT4x4:
			return GFXAsset::MATERIAL_INSTANCE_UNIFORM_Mat4x4;
		case UNIFORMTYPE::TEXTURE2D:
			return GFXAsset::MATERIAL_INSTANCE_UNIFORM_TEXTURE2D;
		default:
			std::cout << "There is a problem with Convert_to_Flatbuffer_Uniform_Type!\n";
			std::this_thread::sleep_for(std::chrono::seconds(10));
		}
	}
	//Create uniforms with no data!
	flatbuffers::Offset<void> Create_a_Uniform(flatbuffers::FlatBufferBuilder* builder, const Material_Uniform* material_uniform) {
		auto UNIFORM_NAME = builder->CreateString(material_uniform->VARIABLE_NAME);
		switch (material_uniform->VARIABLE_TYPE) {
		case UNIFORMTYPE::VAR_UINT32:
			return GFXAsset::CreateUNIFORM_UINT32(*builder, UNIFORM_NAME).Union();
		case UNIFORMTYPE::VAR_INT32:
			return GFXAsset::CreateUNIFORM_UINT32(*builder, UNIFORM_NAME).Union();
		case UNIFORMTYPE::VAR_FLOAT32:
			return GFXAsset::CreateUNIFORM_FLOAT(*builder, UNIFORM_NAME).Union();
		case UNIFORMTYPE::VAR_VEC2:
			return GFXAsset::CreateUNIFORM_VEC2(*builder, UNIFORM_NAME).Union();
		case UNIFORMTYPE::VAR_VEC3:
			return GFXAsset::CreateUNIFORM_VEC3(*builder, UNIFORM_NAME).Union();
		case UNIFORMTYPE::VAR_VEC4:
			return GFXAsset::CreateUNIFORM_VEC4(*builder, UNIFORM_NAME).Union();
		case UNIFORMTYPE::VAR_MAT4x4:
			return GFXAsset::CreateUNIFORM_MAT4x4(*builder, UNIFORM_NAME).Union();
		case UNIFORMTYPE::TEXTURE2D:
			return GFXAsset::CreateUNIFORM_TEXTURE2D(*builder, UNIFORM_NAME).Union();
		default:
			std::cout << "There is a problem in Create_a_Uniform!\n";
			TuranAPI::Breakpoint();
		}
	}
	GFXAsset::Mat4x4 Create_a_Flatbuffer_Matrix4x4(flatbuffers::FlatBufferBuilder* builder, mat4x4 read_matrix) {
		GFXAsset::Vec4 first_row(read_matrix[0][0], read_matrix[1][0], read_matrix[2][0], read_matrix[3][0]);
		GFXAsset::Vec4 second_row(read_matrix[0][1], read_matrix[1][1], read_matrix[2][1], read_matrix[3][1]);
		GFXAsset::Vec4 third_row(read_matrix[0][2], read_matrix[1][2], read_matrix[2][2], read_matrix[3][2]);
		GFXAsset::Vec4 fourth_row(read_matrix[0][3], read_matrix[1][3], read_matrix[2][3], read_matrix[3][3]);
		return GFXAsset::Mat4x4(first_row, second_row, third_row, fourth_row);
	}

	/*
	mat4x4 Read_a_Flatbuffer_Mat4x4(GFXAsset::Mat4x4* read_matrix) {
		mat4x4 return_matrix;
		return_matrix[0][0] = read_matrix->ROW_1().x;	return_matrix[1][0] = read_matrix->ROW_1().y;	return_matrix[2][0] = read_matrix->ROW_1().z;	return_matrix[3][0] = read_matrix->ROW_1().w;
		return_matrix[0][1] = read_matrix->ROW_2().x;	return_matrix[1][1] = read_matrix->ROW_2().y;	return_matrix[2][1] = read_matrix->ROW_2().z;	return_matrix[3][1] = read_matrix->ROW_2().w;
		return_matrix[0][2] = read_matrix->ROW_3().x;	return_matrix[1][2] = read_matrix->ROW_3().y;	return_matrix[2][2] = read_matrix->ROW_3().z;	return_matrix[3][2] = read_matrix->ROW_3().w;
		return_matrix[0][3] = read_matrix->ROW_4().x;	return_matrix[1][3] = read_matrix->ROW_4().y;	return_matrix[2][3] = read_matrix->ROW_4().z;	return_matrix[3][3] = read_matrix->ROW_4().w;
		return return_matrix;
	}*/







	//MATERIAL UNIFORM FUNCTIONs

	Material_Uniform::Material_Uniform(const char* variable_name, UNIFORMTYPE variable_type) : VARIABLE_NAME(variable_name), VARIABLE_TYPE(variable_type) {}
	Material_Uniform::Material_Uniform() {}
	//Please keep in mind that: This doesn't verifies if data matches with variable type!
	bool Material_Uniform::Verify_UniformData() {
		if (DATA != nullptr && VARIABLE_NAME != "") {
			return true;
		}
		return false;
	}




	//SHADER SOURCE FUNCTIONs

	ShaderSource::ShaderSource() : Resource_Type(RESOURCETYPEs::GFXAPI_SHADERSOURCE) {

	}
	void ShaderSource::Write_ToDisk(bool Verify_Data) {
		if (Verify_Data) {
			Verify_Resource_Data();
		}
		flatbuffers::FlatBufferBuilder builder(1024);

		auto FINISHED_SHADERSOURCE = GFXAsset::CreateSHADERSOURCE(builder, ConvertShaderStageType_toFB(STAGE), builder.CreateString(SOURCE_CODE), ConvertShaderLanguage_toFB(LANGUAGE));
		auto FINISHED_RESOURCE_DATA = GFXAsset::CreateResource(builder, GFXAsset::Resource_Type_Shader_Source, FINISHED_SHADERSOURCE.Union());
		builder.Finish(FINISHED_RESOURCE_DATA);

		unsigned int data_size = builder.GetSize();

		std::cout << "Compiled Material Type Resource to Flatbuffer type!\n";
		void* data_ptr = builder.GetBufferPointer();

		//Check if the data is complete!
		flatbuffers::Verifier verifier((uint8_t*)data_ptr, data_size);
		if (!GFXAsset::VerifyResourceBuffer(verifier)) {
			std::cout << "Data isn't verified for name: " << NAME << std::endl;
			assert(false && "Error while compiling data to disk!");
		}

		std::cout << "Exporting resource as a .mattypecont: " << PATH << std::endl;
		TAPIFILESYSTEM::Overwrite_BinaryFile(PATH, data_ptr, data_size);

		std::cout << "Resource is successfully added to the Content_List.enginecont\n";

	}
	void ShaderSource::Load_FromDisk(void* flatbuffer_data, unsigned int ID, const char* path, IAllocator* Allocator) {
		auto RESOURCE_typeless = GFXAsset::GetResource(flatbuffer_data);
		if (RESOURCE_typeless == nullptr) {
			std::cout << "Error: Loading failed! Shader Source isn't a valid resource!\n";
			return;
		}
		std::cout << "Loading Shader Source ID: " << ID << std::endl;
		auto RESOURCE = RESOURCE_typeless->TYPE_as_Shader_Source();

		if (!RESOURCE) {
			std::cout << "Type isn't Shader Source, type definition is wrong!\n";
			return;
		}

		LANGUAGE = GetShaderLanguage_fromFB(RESOURCE->LANGUAGE());
		STAGE = GetShaderStageType_fromFB(RESOURCE->STAGE());
		SOURCE_CODE = RESOURCE->CODE()->c_str();
	}
	bool ShaderSource::Verify_Resource_Data() {
		if (SOURCE_CODE.length() != 0 && NAME.length() != 0 && PATH.length() != 0) {
			return true;
		}
		else {
			std::cout << "Shader Source isn't verified because of LANGUAGE TYPE or SOURCE CODE or Resource Name or Resource Path!";
			TuranAPI::Breakpoint();
			return false;
		}
		return false;
	}


	//MATERIAL TYPE FUNCTIONs

	Material_Type_Resource::Material_Type_Resource() : Resource_Type(RESOURCETYPEs::GFXAPI_MATTYPE), UNIFORMs(LASTUSEDALLOCATOR, 2, 4) {

	}
	bool Material_Type_Resource::Verify_Resource_Data() {
		if (NAME.length() != 0 && PATH.length() != 0) {
			for (unsigned int uniform_index = 0; uniform_index < UNIFORMs.size(); uniform_index++) {
				Material_Uniform* UNIFORM = &UNIFORMs[uniform_index];
				if (!
					(UNIFORM->VARIABLE_NAME != "")) {
					//If Material Type's uniform isn't verified!
					//There should be log!
					//A Material Type's uniform doesn't hold any data, so UNIFORM->Verify_UniformData is meaningless!

					std::cout << "Material Type isn't verified because of a uniform has a uninitialized Variable Type or Variable Name!";
					TuranAPI::Breakpoint();
					return false;
				}
			}
			//Material type's every uniform is verified!
			return true;
		}
		else {
			std::cout << "Material Type isn't verified because of GFX_API or Vertex Source or Fragment Source or Resource Name or Resource Path!";
			TuranAPI::Breakpoint();
			return false;
		}
		return false;
	}
	//Load material resource from the data provided by FileList.enginecont
	void Material_Type_Resource::Load_FromDisk(void* data, unsigned int id, const char* path, IAllocator* Allocator) {
		auto RESOURCE_typeless = GFXAsset::GetResource(data);
		if (RESOURCE_typeless == nullptr) {
			std::cout << "Error: Loading failed! Material Type isn't a valid resource!\n";
			return;
		}
		std::cout << "Loading Material Type ID: " << id << std::endl;
		auto RESOURCE = RESOURCE_typeless->TYPE_as_Material_Type();

		if (!RESOURCE) {
			std::cout << "Type isn't Material Type, type definition is wrong!\n";
			return;
		}

		VERTEXSOURCE_ID = RESOURCE->STAGEs()->Get(0);
		FRAGMENTSOURCE_ID = RESOURCE->STAGEs()->Get(1);
		PATH = path;
		ID = id;

		//Uniform loading progress!
		for (unsigned int i = 0; i < RESOURCE->MATERIAL_INPUTs()->size(); i++) {
			Material_Uniform uniform;
			auto UNIFORM_typeless = RESOURCE->MATERIAL_INPUTs()->Get(i);
			uniform.VARIABLE_TYPE = Convert_to_TuranAPIEnums_UniformVarType(UNIFORM_typeless->UNIFORM_type());
			uniform.VARIABLE_NAME = Get_UniformName_fromFlatbuffer(UNIFORM_typeless, uniform.VARIABLE_TYPE);
			UNIFORMs.push_back(uniform);
		}

		std::string strNAME = path;
		strNAME = strNAME.substr(strNAME.find_last_of('/') + 1);
		strNAME = strNAME.substr(0, strNAME.find_last_of('.'));
		NAME = strNAME.c_str();
	}
	//Create flatbuffer data to export resource into a .gamecont
	void Material_Type_Resource::Write_ToDisk(bool Verify_Data) {
		if (Verify_Data) {
			Verify_Resource_Data();
		}
		flatbuffers::FlatBufferBuilder builder(1024);


		std::vector<flatbuffers::Offset<GFXAsset::MATERIAL_INSTANCE_UNIFORM_TABLE>> MATERIAL_UNIFORMs;
		//Add uniforms to Flatbuffer data!
		for (unsigned int i = 0; i < UNIFORMs.size(); i++) {
			Material_Uniform& material_uniform = UNIFORMs[i];
			std::cout << "Uniform is being proccessed: " << material_uniform.VARIABLE_NAME << std::endl;
			MATERIAL_UNIFORMs.push_back(
				GFXAsset::CreateMATERIAL_INSTANCE_UNIFORM_TABLE(builder,
					Convert_to_Flatbufer_Uniform_Type(material_uniform.VARIABLE_TYPE),
					Create_a_Uniform(&builder, &material_uniform)
				)
			);

		}
		auto finished_UNIFORMs_VECTOR = builder.CreateVector(MATERIAL_UNIFORMs);

		Vector<unsigned int> shadersources(LASTUSEDALLOCATOR, 0, 2);
		shadersources.push_back(VERTEXSOURCE_ID);
		shadersources.push_back(FRAGMENTSOURCE_ID);

		auto MATERIAL_TYPE_f = GFXAsset::CreateMATERIAL_TYPE(builder, 
			builder.CreateVector(shadersources.data(), shadersources.size()), finished_UNIFORMs_VECTOR
		);

		auto FINISHED_RESOURCE_DATA_f = GFXAsset::CreateResource(builder, GFXAsset::Resource_Type_Material_Type, MATERIAL_TYPE_f.Union());
		builder.Finish(FINISHED_RESOURCE_DATA_f);

		unsigned int data_size = builder.GetSize();

		std::cout << "Compiled Material Type Resource to Flatbuffer type!\n";
		void* data_ptr = builder.GetBufferPointer();

		//Check if the data is complete!
		flatbuffers::Verifier verifier((uint8_t*)data_ptr, data_size);
		if (!GFXAsset::VerifyResourceBuffer(verifier)) {
			std::cout << "Data isn't verified for name: " << NAME << std::endl;
			assert(false && "Error while compiling data to disk!");
		}

		std::cout << "Exporting resource as a .mattypecont: " << PATH << std::endl;
		TAPIFILESYSTEM::Overwrite_BinaryFile(PATH, data_ptr, data_size);

		std::cout << "Resource is successfully added to the Content_List.enginecont\n";
	}









	//MATERIAL INSTANCE FUNCTIONs

	Material_Instance::Material_Instance() : Resource_Type(RESOURCETYPEs::GFXAPI_MATINST), UNIFORM_LIST(LASTUSEDALLOCATOR, 10, 10) {

	}
	void Material_Instance::Load_FromDisk(void* data, unsigned int id, const char* path, IAllocator* Allocator) {
		auto RESOURCE_typeless = GFXAsset::GetResource(data);
		if (RESOURCE_typeless == nullptr) {
			std::cout << "Error: Loading failed! Material Instance isn't a valid resource!\n";
			TuranAPI::Breakpoint();
			return;
		}
		std::cout << "Loading Material Instance (Shader Stage) ID: " << id << std::endl;
		auto RESOURCE = RESOURCE_typeless->TYPE_as_Material_Instance();
		if (!RESOURCE) {
			std::cout << "Error: Loading failed! Material Instance isn't verified as Material Instance Type!\n";
			TuranAPI::Breakpoint();
			return;
		}
		//If application arrives here, loading the file is successful!
		Material_Type = Find_MaterialType_byID(RESOURCE->MATERIAL_TYPE_ID());
		ID = id;
		PATH = path;
		std::string strNAME = path;
		strNAME = strNAME.substr(strNAME.find_last_of('/') + 1);
		strNAME = strNAME.substr(0, strNAME.find_last_of('.'));
		NAME = strNAME.c_str();
		for (unsigned int i = 0; i < RESOURCE->UNIFORMs()->Length(); i++) {
			Material_Uniform uniform;
			auto UNIFORM_typeless = RESOURCE->UNIFORMs()->Get(i);
			uniform.VARIABLE_TYPE = Convert_to_TuranAPIEnums_UniformVarType(UNIFORM_typeless->UNIFORM_type());
			uniform.VARIABLE_NAME = Get_UniformName_fromFlatbuffer(UNIFORM_typeless, uniform.VARIABLE_TYPE);
			//Please add here data loading!
			UNIFORM_LIST.push_back(uniform);
		}
	}
	void Material_Instance::Write_ToDisk(bool Verify_Data) {
		if (Verify_Data) {
			if (!Verify_Resource_Data()) {
				TuranAPI::LOG_ERROR("Resource isn't written to disk, because it's not verified!");
			}
		}
		flatbuffers::FlatBufferBuilder builder(1024);

		std::vector<flatbuffers::Offset<GFXAsset::MATERIAL_INSTANCE_UNIFORM_TABLE>> MATERIAL_UNIFORMs;
		//Add uniforms to Flatbuffer data!
		for (unsigned int i = 0; i < UNIFORM_LIST.size(); i++) {
			Material_Uniform& material_uniform = UNIFORM_LIST[i];
			std::cout << "Uniform is being proccessed: " << material_uniform.VARIABLE_NAME << std::endl;
			MATERIAL_UNIFORMs.push_back(
				GFXAsset::CreateMATERIAL_INSTANCE_UNIFORM_TABLE(builder,
					Convert_to_Flatbufer_Uniform_Type(material_uniform.VARIABLE_TYPE),
					Create_a_Uniform(&builder, &material_uniform)
				)
			);
		}
		auto finished_UNIFORMs_VECTOR = builder.CreateVector(MATERIAL_UNIFORMs);

		auto finished_MaterialInstance = GFXAsset::CreateMATERIAL_INSTANCE(builder, Material_Type->ID, finished_UNIFORMs_VECTOR);
		auto finished_RESOURCE = GFXAsset::CreateResource(builder, GFXAsset::Resource_Type_Material_Instance, finished_MaterialInstance.Union());
		builder.Finish(finished_RESOURCE);

		unsigned int data_size = builder.GetSize();

		std::cout << "Compiled Material Instance Resource to Flatbuffer type!\n";
		void* data_ptr = builder.GetBufferPointer();

		//Check if the data is complete!
		flatbuffers::Verifier verifier((uint8_t*)data_ptr, data_size);
		if (!GFXAsset::VerifyResourceBuffer(verifier)) {
			std::cout << "Data isn't verified for name: " << NAME << std::endl;
			assert(false && "Error while compiling data to disk!");
		}

		std::cout << "Exporting resource as a .matinstcont: " << PATH << std::endl;
		TAPIFILESYSTEM::Overwrite_BinaryFile(PATH, data_ptr, data_size);

		std::cout << "Resource is successfully added to the Content_List.enginecont\n";
	}
	bool Material_Instance::Verify_Resource_Data() {
		if (Material_Type != nullptr && NAME != "" && PATH != "") {
			if (Material_Type->Verify_Resource_Data()) {
				if (UNIFORM_LIST.size() == Material_Type->UNIFORMs.size()) {
					for (unsigned int uniform_index = 0; uniform_index < UNIFORM_LIST.size(); uniform_index++) {
						Material_Uniform* UNIFORM = &UNIFORM_LIST[uniform_index];

						//Note: There should be uniform data storing for Material Instances
					}
				}
				else {
					std::cout << "Material Instance isn't verified because Uniform lists isn't match between Material Type and Instance!";
					TuranAPI::Breakpoint();
					return false;
				}
			}
			else {
				std::cout << "Material Instance isn't verified because Material Type isn't verified!";
				TuranAPI::Breakpoint();
				return false;
			}
		}
		else {
			std::cout << "Material Instance isn't verified because of Material Type or Resource Name or Resource Path!";
			TuranAPI::Breakpoint();
			return false;
		}
		return true;
	}

	void Material_Instance::Set_Uniform_Data(const char* uniform_name, void* pointer_to_data) {
		Material_Uniform* uniform = &UNIFORM_LIST[Find_Uniform_byName(uniform_name)];
		if (pointer_to_data == nullptr) {
			std::cout << "Error: Couldn't set GPU uniform data, because data is nullptr!\n";
			TuranAPI::Breakpoint();
			return;
		}
		if (uniform == nullptr) {
			std::cout << "Error: Found uniform is nullptr!\n";
			TuranAPI::Breakpoint();
			return;
		}
		uniform->DATA = pointer_to_data;
	}

	unsigned int Material_Instance::Find_Uniform_byName(const char* uniform_name) {
		for (unsigned int i = 0; i < UNIFORM_LIST.size(); i++) {
			if (UNIFORM_LIST[i].VARIABLE_NAME == uniform_name)
				return i;
		}
		std::cout << "Error: Intended uniform variable: " << uniform_name << " can't be found in Material Type: " << Material_Type->NAME << std::endl;
		TuranAPI::Breakpoint();
		return -1;
	}

}